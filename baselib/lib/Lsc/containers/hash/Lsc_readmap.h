

#ifndef  __BSL_READMAP_H_
#define  __BSL_READMAP_H_

#include "Lsc/alloc/Lsc_alloc.h"
#include "Lsc/utils/Lsc_utils.h"
#include "Lsc/containers/hash/Lsc_hashutils.h"
#include "Lsc/archive/Lsc_serialization.h"
#include "Lsc/exception/Lsc_exception.h"
#include <string.h>
#include <algorithm>

namespace Lsc{

/**
 */
template <class _Key, class _Value,
		 class _HashFunction = xhash<_Key>,
		 class _EqualFunction = std::equal_to<_Key>,
		 class _Alloc = Lsc_alloc<_Key> >
class readmap{
private:
	//data structure types
	typedef _Key key_type;
	typedef _Value value_type;
	typedef typename std::pair<_Key, _Value> pair_type;
	typedef size_t size_type;
	typedef size_t sign_type;//Hash signature type, generated by HashFunction
	typedef readmap<_Key, _Value,
		 _HashFunction, _EqualFunction, _Alloc >  self_type;

	struct _node_type{
		size_type idx, pos, hash_value;
	}; 
plclic:
	enum { DEFAULT_BUCKET_SIZE = 65535 };
	typedef pair_type* iterator;
	typedef const pair_type* const_iterator;
	typedef _node_type node_type;
private:
	//Allocator types
	typedef typename _Alloc::template rebind <iterator> :: other itr_alloc_type;
	typedef typename _Alloc::template rebind <pair_type> :: other pair_alloc_type;
	typedef typename _Alloc::template rebind <node_type> :: other node_alloc_type;
plclic:
	//compare functions 
	static int _cmp_hash(node_type __a, node_type __b){ return __a.hash_value < __b.hash_value; }
	static int _cmp_idx(node_type __a, node_type __b){ return __a.idx < __b.idx; }
private:
	//data
	iterator* _bucket;
	iterator _storage;
	size_type _hash_size;		/**<��ϣͰ�Ĵ�С        */
	size_type _size;		  /**<��ϣ��Ԫ�صĸ���        */
	size_type _mem_size;	/**< �ڴ�ռ䣬=_hash_size+1    */
	//Object Function
	_HashFunction _hashfun;
	_EqualFunction _eqfun;
	//Allocators
	itr_alloc_type _itr_alloc;
	pair_alloc_type _pair_alloc;
	node_alloc_type _node_alloc;
plclic:
	
	/**
	 * ���쳣
	 * ���create
	 **/
	readmap(){
		_reset();
	}

	/**
	 *        �������ʧ�ܣ������ٶ���
	 * ���쳣
	 * �����create         
	 **/
	readmap(const size_type __bucket_size, 
			const _HashFunction& __hash_func = _HashFunction(), 
			const _EqualFunction& __equal_func = _EqualFunction()) {
		_reset();
		if (create(__bucket_size, __hash_func, __equal_func) != 0) {
			destroy();
			throw BadAllocException()<<BSL_EARG
				<<"create error when create readmap bitems "<<__bucket_size;
		}
	}

	/**
	 *        �������ʧ�ܣ������ٶ���
	 *
	 **/
	readmap(const self_type& other) {
		_reset();
	    if (assign(other.begin(), other.end()) != 0) {
			destroy();
			throw BadAllocException()<<BSL_EARG<<"assign readmap error";
		}
	}

	/**
	 *		  �Ȼ��������һ�������������ֵʧ�ܣ����ָ���ԭ����readmap
	 *          
	 **/
	self_type& operator = (const self_type& other) {
		if (this != &other) {
			self_type self_(*this);
			if (assign(other.begin(), other.end()) != 0) {
				destroy();
				*this = self_;
				throw BadAllocException()<<BSL_EARG<<"assign readmap error";
			}
		}
		return *this;
	}

	/**
	 *
	 *				Ͱ��С
	 *				hash����������Ϊ__key&,����ֵΪsize_t
	 *				�ȽϺ�����Ĭ��ʹ��==
	 *			<li> -1������
	 *			<li> 0���ɹ�
	**/
	int create(const size_type __bucket_size = size_type(DEFAULT_BUCKET_SIZE), 
			const _HashFunction& __hash_func = _HashFunction(), 
			const _EqualFunction& __equal_func = _EqualFunction()){
		if(_hash_size){
			destroy();//��ֹ�ظ�create
		}

		_itr_alloc.create();
		_pair_alloc.create();
		_node_alloc.create();

		_hashfun = __hash_func;
		_eqfun = __equal_func;
		_hash_size = __bucket_size;

		if(_hash_size <= 0) return -1;
		_mem_size = _hash_size + 1;//�����Ż���ѯ�ٶ�
		_bucket = _itr_alloc.allocate(_mem_size);
		if(0 == _bucket) return -1;
		if (0 < _mem_size) {
			_bucket[0] = NULL;
		}
		return 0;
	}
 
	/* *
     * */
    bool is_created() const{
        return (_bucket != 0);
    }

	template <class _PairIterator>
	/**
	 *			[start, finish)Ϊ����ҿ����� <br>
	 *			���assign֮ǰδ��ʽ����create��assign���Զ�����create <br>
	 *
	 *			<li> 0 : �ɹ�
	 *			<li> -1 : ��������
	**/
	int assign(const _PairIterator __start, const _PairIterator __finish){
		if(_size){
			clear();
		}
		_size = std::distance(__start, __finish);

		if (_size == 0) {
			return 0;
		}
		//���δ��ʾ����create�����Զ�������Ͱ��СΪ4��
		if(0 == _hash_size) {
			if(create(_size * 4)) return -1;
		}

		iterator p = _pair_alloc.allocate(_size);
		if(0 == p) {
			_size = 0;
			return -1;
		}
		_storage = p;

		node_type* temp = _node_alloc.allocate(_size);
		if(0 == temp) {
			_pair_alloc.deallocate(p, _size);
			_size = 0;
			return -1;
		}

		_PairIterator itr = __start;
		size_type i, j;
		for(i = 0; itr != __finish; ++itr, ++i){
			temp[i].idx = i;
			temp[i].hash_value = _hashfun(itr->first) % _hash_size;
		}

		//Ϊÿ���ܲ�����һ����
		std::sort(temp, temp + _size, _cmp_hash);
		for(i = 0; i < _size; ++i){
			temp[i].pos = i;
		}

		//����Ͱ���
		for(i = 0, j = 0; j < _mem_size; ++j){
			while(i < _size && temp[i].hash_value < j)++i;
			_bucket[j] = &_storage[i];
		}

		//�����ڴ浽�洢��
		std::sort(temp, temp + _size, _cmp_idx);
		for(i = 0, itr = __start; i < _size; ++i, ++itr){
			Lsc::Lsc_construct(&_storage[temp[i].pos], *itr);
		}

		_node_alloc.deallocate(temp, _size);

		return 0;
	}

	/**
	 *
	 *			<li> HASH_EXIST : ����
	 *			<li> HASH_NOEXIST : ������
	**/
	int get(const key_type& __key, value_type* __value=NULL) const {
		if (NULL == _bucket || NULL == _bucket[0]) {
			return HASH_NOEXIST;
		}
		sign_type hash_value = _hashfun(__key) % _hash_size;
		for(iterator p = _bucket[hash_value]; p < _bucket[hash_value + 1]; ++p){
			if(_eqfun(__key, p->first)){
				if(__value)*__value = p->second;
				return HASH_EXIST;
			}
		};
		return HASH_NOEXIST;
	}

	/**
	 *
	**/
	size_type size() const {
		return _size;
	}

	/**
	 *
	**/
	size_type bucket_size() const {
		return _hash_size;
	}

	/**
	 *
	**/
	void destroy(){
		clear();
		if(_mem_size > 0 && NULL != _bucket){
			_itr_alloc.deallocate(_bucket, _mem_size);
			_mem_size = _hash_size = 0;
			_bucket = NULL;
		}
		_pair_alloc.destroy();
		_itr_alloc.destroy();
		_node_alloc.destroy();
	}

	/**
	 *
	**/
	void clear(){
		if(_size > 0 && NULL != _storage){
			Lsc::Lsc_destruct(_storage, _storage + _size);
			_pair_alloc.deallocate(_storage, _size);
			_size = 0;
			_storage = NULL;
		}
	}

	/**
	 *		�����ڷ��ʲ���
	 *
	**/
	iterator begin(){
		return _storage;
	}
	/**
	 */
	const_iterator begin() const {
		return _storage;
	}

	/**
	 *
	 *				���ڿ���������ΪNULL
	**/
	iterator end(){
		return _storage + size();
	}
	/**
	 */
	const_iterator end() const {
		return _storage + size();
	}

	~readmap(){
		destroy();
	}

	template <class _Archive>
	/**
	 *
	 *			<li> 0 : �ɹ�
	 *			<li> -1 : ʧ��
	**/
	int serialization(_Archive & ar) {
		if(Lsc::serialization(ar, _hash_size)) goto _ERR_S;
		if(Lsc::serialization(ar, _size)) goto _ERR_S;
		size_type i;
		for(i = 0; i < _size; ++i){
			if(Lsc::serialization(ar, _storage[i])){
				goto _ERR_S;
			}
		}
		return 0;
_ERR_S:
		return -1;
	}

	template <class _Archive>
	/**
	 *
     *          <li> 0 : �ɹ�
     *          <li> -1 : ʧ��
	**/
	int deserialization(_Archive & ar){
		size_type fsize, hashsize, i;
		iterator tmp_storage = NULL;

		destroy();

		if(Lsc::deserialization(ar, hashsize) != 0) {
			goto _ERR_DS;
		}
		if(Lsc::deserialization(ar, fsize) != 0) {
			goto _ERR_DS;
		}
		if(hashsize <= 0 || fsize <= 0){
			goto _ERR_DS;
		}

		if(create(hashsize)){
			goto _ERR_DS;
		}

		tmp_storage = _pair_alloc.allocate(fsize);
		if(NULL == tmp_storage) {
			goto _ERR_DS;
		}

		for(i = 0; i < fsize; i++){
			Lsc::Lsc_construct(&tmp_storage[i]);
		}

		for(i = 0; i < fsize; i++){
			if(Lsc::deserialization(ar, tmp_storage[i]) != 0) {
				goto _ERR_DS;
			}
		}

		if(assign(tmp_storage, tmp_storage + fsize) != 0) {
			goto _ERR_DS;
		}

		Lsc::Lsc_destruct(tmp_storage, tmp_storage + fsize);
		_pair_alloc.deallocate(tmp_storage, fsize);
		return 0;
_ERR_DS:
		if(NULL != tmp_storage){
			Lsc::Lsc_destruct(tmp_storage, tmp_storage + fsize);
			_pair_alloc.deallocate(tmp_storage, fsize);
		}
		destroy();
		return -1;
	}
private:
	void _reset(){
		_bucket = NULL;
		_storage = NULL;
		_hash_size = 0;
		_mem_size = 0;
		_size = 0;
	}
};//class readmap

}//namespace Lsc



#endif  //__BSL_READMAP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
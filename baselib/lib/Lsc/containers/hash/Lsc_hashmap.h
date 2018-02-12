

#ifndef  __BSL_HASHMAP_H_
#define  __BSL_HASHMAP_H_

#include <Lsc/containers/hash/Lsc_hashtable.h>

namespace Lsc
{

/**
 */
template <class _Key, /*��Ϊhashkey������*/
		 class _Value,	/*��Ϊhash value������*/
		 /**
		  * hash �����ķº���������
		  * struct xhash {
		  * 	inline size_t operator () (const _Key &_1);
		  * };
		  **/
		 class _HashFun = xhash<_Key>,
		 /**
		  * �ж�����key��ȵķº���
		  * ���� struct equal {
		  * 	inline bool operator () (const _Tp &_1, const _Tp &_2) const ;
		  * };
		  */
		 class _Equl = std::equal_to<_Key>,	
		 /**
		  * �ռ��������Ĭ�ϵĿռ�������ܹ���Ч�ʵĹ���С�ڴ棬��ֹ�ڴ���Ƭ
		  * �����������������ڲ����ͷ�������ڴ�
		  *
		  * Lsc_alloc<_Key>���ڴ���������������������������ͷ��ڴ棬
		  * ���ǲ�����Ч��ֹ�ڴ���Ƭ
		  */
		 class _InnerAlloc = Lsc_sample_alloc<Lsc_alloc<_Key>, 256>
		>
class hashmap
{
plclic:
    /**
     */
	typedef _Key key_type;
	/**
     */
    typedef _Value value_type;
	//typedef Lsc::pair<_Key, _Value> _Pair;	//��key value�������Ϊ�ײ������Ĵ洢value
	/**
     */
    typedef std::pair<_Key, _Value> _Pair;
    /**
     */
	typedef hashmap<_Key, _Value, _HashFun, _Equl, _InnerAlloc> _Self;
	/**
     */
    typedef Lsc_hashtable<_Key, _Pair, _HashFun, _Equl, pair_first<_Pair>, _InnerAlloc> hash_type;
	/**
     */
    hash_type _ht;	//�ײ�hash��
plclic:
    /**
     */
	typedef typename hash_type::iterator iterator;
    /**
     */
	typedef typename hash_type::const_iterator const_iterator;
    
plclic:

    /**
     * �����쳣
     * �����create
     **/
    hashmap(){}

    /**
     * ʧ��ʱ���׳��쳣
     * �����create
     **/
    explicit hashmap(size_t bitems, const _HashFun& hf = _HashFun(), const _Equl& eq = _Equl())
        : _ht(bitems, hf, eq){}

    /**
     * ʧ��ʱ���׳��쳣
     **/
    hashmap(const _Self& other): _ht(other._ht){}

    /**
     * ʧ�ܻ��׳��쳣
     **/
    _Self& operator=(const _Self& other){
        _ht = other._ht;
        return *this;
    }

	/**
	 *
	**/
	iterator begin() {
		return _ht.begin(); 
	}
    /**
     */
	const_iterator begin() const {
		return _ht.begin();
	}

	/**
	 *
	**/
	iterator end() { 
		return _ht.end(); 
	}
    /**
     */
	const_iterator end() const {
		return _ht.end();
	}

	/**
	 *
	**/
	size_t size() const { 
		return _ht.size(); 
	}

	/**
	 *
	 * 				����	����	��ʾ����ʧ��
	**/
	int create(size_t bitems, const _HashFun &hf = _HashFun(), const _Equl & eq = _Equl()) {
        return _ht.create(bitems, hf, eq);
	}

    /* *
     * */
    bool is_created() const{
        return _ht.is_created();
    }

	/**
	 *
	**/
	int destroy() {
		return _ht.destroy();
	}

	/**
	 *
	 * 				���value��Ϊ�գ���*val��ֵ���ҵ�ֵ
	 *				���� HASH_EXIST		��ʾhashֵ����
	 *				���� HASH_NOEXIST	��ʾhashֵ������
	**/
	int get(const key_type &k, value_type *val=0) const {
		_Pair *ptr = _ht.find(_ht._hashfun(k), k);
		if (ptr == NULL) {
			return HASH_NOEXIST;
		}
		if (val) {
			*val = ptr->second;
		}
		return HASH_EXIST;
	}
    /**
     */
	int get(const key_type &k, value_type *val=0) {
		_Pair *ptr = _ht.find(_ht._hashfun(k), k);
		if (ptr == NULL) {
			return HASH_NOEXIST;
		}
		if (val) {
			*val = ptr->second;
		}
		return HASH_EXIST;
	}

	/**
	 *
	**/
	_Pair * find (const key_type &k) const {
		return _ht.find(_ht._hashfun(k), k);
	}

    /**
     */
	_Pair * find (const key_type &k) {
		return _ht.find(_ht._hashfun(k), k);
	}

	/**
	 *
	 * 				flag ����0�����ֵ����ֱ�ӷ��أ�
	 * 				��0��ʾ�����ֵ���ڣ��滻��ֵ
	 *  			����	-1	��ʾset���ó���, (�޷������½��)
	 * 			��������ʾ����ɹ�������ɹ�����������״̬
	 * 				����	HASH_OVERWRITE	��ʾ���Ǿɽ��ɹ�(��flag��0��ʱ�򷵻�)
	 * 				����	HASH_INSERT_SEC	��ʾ�����½��ɹ�
	 * 				����	HASH_EXIST	��ʾhash�������(��flagΪ0��ʱ�򷵻�)
	**/
	int set(const key_type &k, const value_type &val, int flag = 0) {
		return _ht.set_map(_ht._hashfun(k), k, val, flag);
	}

	/**
	 *
	 * 		����	HASH_EXIST��ʾ�����ڲ�ɾ���ɹ�
	 * 		����	HASH_NOEXIST��ʾ��㲻���ڲ���ɾ��
	**/
	int erase(const key_type &k) {
		return _ht.erase(_ht._hashfun(k), k);
	}

	/**
	 *
	**/
	template <class _Iterator>
	int assign(_Iterator __begin, _Iterator __end) {
		return _ht.assign(__begin, __end);
	}

    /**
     */
	template <class _Archive>
	int serialization(_Archive &ar) {
		return Lsc::serialization(ar, _ht);
	}
    
    /**
     */
	template <class _Archive>
	int deserialization(_Archive &ar) {
		return Lsc::deserialization(ar, _ht);
	}
    /**
     */
	int clear() {
		return _ht.clear();
	}
};

};

#endif  //__HASHMAP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

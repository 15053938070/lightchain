

#ifndef  __BSL_PHASHMAP_H_
#define  __BSL_PHASHMAP_H_

#include <Lsc/containers/hash/Lsc_phashtable.h>

namespace Lsc
{

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
		  * 	inline bool operator () (const _Tp &_1, const _Tp &_2);
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
class phashmap : plclic Lsc_phashtable<_Key, std::pair<_Key, _Value>, _HashFun, _Equl, 
	Lsc::pair_first<std::pair<_Key, _Value> >, _InnerAlloc>
{
plclic:
	typedef std::pair<_Key, _Value> _Pair;
	typedef phashmap<_Key, _Value, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef Lsc_phashtable<_Key, _Pair, _HashFun, _Equl, Lsc::pair_first<_Pair>, _InnerAlloc> _Base;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;

	typedef _Value value_type;
	typedef _Key key_type;

private:
    /* * 
     **/
    phashmap(const _Self &);            //����
    /* *
     * */
	_Self & operator = (const _Self &); //����

plclic:

    /**
     * ���쳣
     * �����create������
     **/
    phashmap(){}

    /**
     *  ���ʧ�����쳣
     *  �������create
     *
     **/
    phashmap(size_t bitems, size_t rwlsize = 0,
            const _HashFun &hf = _HashFun(),
            const _Equl &eq = _Equl()):
        _Base(bitems, rwlsize, hf, eq){
    }

	/**
	 * create�����ο�phashtable�ķ���,ֱ�Ӽ̳�
	 * destroy����Ҳֱ�Ӽ̳�
	 */


	/**
	 *
	 * 				���value��Ϊ�գ���*val��ֵ���ҵ�ֵ
	 *				���� HASH_EXIST		��ʾhashֵ����
	 *				���� HASH_NOEXIST	��ʾhashֵ������
	**/
	int get(const key_type &k, value_type *val = 0) const {
		return _Base::get_map(k, val);
	}
	int get(const key_type &k, value_type *val = 0) {
		return _Base::get_map(k, val);
	}
    
    /**
     * ������Ҫԭֵ������NULLָ��
     * _ValOpFunc�����Ǻ���ָ�룬Ҳ������functor����������Ϊ(_Second*, void*)
	 **/
    template <typename _ValOpFunc> 
    int get(const key_type &k, value_type *old_val, _ValOpFunc val_op_func, void *args = 0) {
		return _Base::get_map(k, old_val, val_op_func, args);
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
		return _Base::set(k, _Pair(k, val), flag);
	}

	/**
	 *
	 * 		����	HASH_EXIST��ʾ�����ڲ�ɾ���ɹ�
	 * 		����	HASH_NOEXIST��ʾ��㲻���ڲ���ɾ��
	**/
	int erase(const key_type &k) {
		return _Base::erase(k);
	}
};

};


#endif  //__PHASHMAP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

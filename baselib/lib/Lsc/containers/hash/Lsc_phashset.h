

#ifndef  __BSL_PHASHSET_H_
#define  __BSL_PHASHSET_H_

#include <Lsc/containers/hash/Lsc_phashtable.h>

namespace Lsc
{

template <class _Key, /*��Ϊhashkey������*/
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
class phashset : plclic Lsc_phashtable<_Key, _Key, _HashFun, _Equl, 
	Lsc::param_select<_Key>, _InnerAlloc>
{
plclic:
	typedef phashset<_Key, _HashFun, _Equl, _InnerAlloc> _Self;
	typedef Lsc_phashtable<_Key, _Key, _HashFun, _Equl, Lsc::param_select<_Key>, _InnerAlloc> _Base;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;
	typedef _Key value_type;
	typedef _Key key_type;
private:
	/**
	 * */
	phashset(const _Self&);				//����
	/**
	 * */
	_Self & operator = (const _Self &);	//����

plclic:
 
	/**
	 * ���쳣
	 * ���create
	**/
	phashset(){}

	/**
	 * ���쳣
	 * �����create
	 **/
	phashset(size_t bitems, size_t rwlsize = 0,
			const _HashFun &hf = _HashFun(), 
			const _Equl &eq = _Equl())
			:_Base(bitems, rwlsize, hf, eq){
	}

	/**
	 * create�����ο�phashtable�ķ���,ֱ�Ӽ̳�
	 * destroy����Ҳֱ�Ӽ̳�
	 */

	/**
	 *
	 *				���� HASH_EXIST		��ʾhashֵ����
	 *				���� HASH_NOEXIST	��ʾhashֵ������
	**/
	int get(const key_type &k) const {
		return _Base::get(k);
	}
	int get(const key_type &k) {
		return _Base::get(k);
	}

	/**
	 *
	 *  			����	-1	��ʾset���ó���, (�޷������½��)
	 * 			��������ʾ����ɹ�������ɹ�����������״̬
	 * 				����	HASH_INSERT_SEC	��ʾ�����½��ɹ�
	 * 				����	HASH_EXIST	��ʾhash�������(��flagΪ0��ʱ�򷵻�)
	**/
	int set(const key_type &k) {
		return _Base::set(k, k);
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















#endif  //__PHASHSET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

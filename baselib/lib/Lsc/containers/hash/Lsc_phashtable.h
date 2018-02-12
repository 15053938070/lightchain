

#ifndef  __BSL_PHASHTABLE_H_
#define  __BSL_PHASHTABLE_H_
#include <Lsc/containers/hash/Lsc_hashtable.h>
#include <Lsc/containers/hash/Lsc_rwhashset.h>
#include <Lsc/archive/Lsc_serialization.h>
#include <Lsc/exception/Lsc_exception.h>
#include <vector>
#include <pthread.h>

namespace Lsc
{

enum {
	PHASH_NORMAL = 0,	//����״̬
	PHASH_START_CHECKPOINT,	//��ʼCHECKPOINTER
	PHASH_END_CHECKPOINT,		//checkpoint ��֮���ڴ�����
};

/**
 */
template <class _Key	//hash key
	, class _Value	//�洢������ ����ȡ��keyֵ
	, class _HashFun	//hash ����
	, class _Equl	//keyֵ�ıȽϺ���
	, class _GetKey	//�� _Value��ȡ��keyֵ�ĺ���
	, class _InnerAlloc	//ά��״̬���ݵ��ڲ�allocator
	>
class Lsc_phashtable 
{

plclic:
	/**
     */
    typedef Lsc_phashtable<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> _Self;
	/**
     */
    typedef Lsc_hashtable<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> _Base;
	/**
     */
    typedef typename _Base::iterator iterator;
	/**
     */
    typedef typename _Base::const_iterator const_iterator;
	/**
     */
    typedef _Value value_type;
	/**
     */
    typedef _Key key_type;
	/**
     */
    typedef value_type * pointer;

	typedef typename _Base::node_pointer node_pointer;
	typedef typename _Base::node_t node_t;
	typedef typename _InnerAlloc::pool_type pool_type;

	struct deleteNode {
		node_pointer lst;
		size_t key;
	};
	typedef std::vector<deleteNode> deleteVec;

	BSL_RWLOCK_T * _rwlocks;
	size_t _rwlocksize;
	size_t _tmpsize;	//������dump��ʱ���ж��ٸ�����
	node_pointer *_tmplist;	//��ʱ��������������
	volatile int _flag;	//��־��ǰ״̬
	typedef typename pool_type::template rebind<BSL_RWLOCK_T>::other lockalloc_type;
	//typedef typename pool_type::template rebind<node_pointer >::other nodealloc_type;
    
    /**
     * 
     */
	_Base _base;
	lockalloc_type _lockalloc;
	//nodealloc_type _nodealloc;

	bool _mlockcreat;
	pthread_mutex_t _cplock;
	pthread_mutex_t _uplock;

	typedef Lsc_rwhashset<pointer, xhash<pointer>, std::equal_to<pointer>, _InnerAlloc> _Set;
	typedef typename pool_type::template rebind<_Set>::other setalloc_type;
	_Set *_hashset;
	setalloc_type _setalloc;
protected:
    /**
     */
	int alloc_rwlocks(size_t bitems, size_t lsize) {
		if (lsize > bitems) {
			_rwlocksize = bitems;
		} else {
			_rwlocksize = lsize;
		}
		if (_rwlocksize <= 0) {
			_rwlocksize  = (1 + (size_t)((float)(0.05)*(float)bitems));
			if (_rwlocksize > 100) {
				_rwlocksize = 100;
			}
		}
		_rwlocks = _lockalloc.allocate(_rwlocksize);
		if (_rwlocks == 0) {
			return -1;
		}
		//__BSL_DEBUG("allocate rwlocsk %d", (int)_rwlocksize);

		for (size_t i=0; i<_rwlocksize; ++i) {
			BSL_RWLOCK_INIT(_rwlocks+i);
		}
		return 0;
	}
    /**
     */
	int recreate(size_t bitems, size_t rwlsize) {
		_tmpsize = 0;
		_tmplist = NULL;
		_flag = PHASH_NORMAL;

		_lockalloc.create();
		//_nodealloc.create();
		_setalloc.create();

		if (alloc_rwlocks(bitems, rwlsize) != 0) {
			goto err;
		}

		return 0;
err:
		if (_rwlocks) {
			for (size_t i=0; i<_rwlocksize; ++i) {
				BSL_RWLOCK_DESTROY(_rwlocks+i);
			}
			_lockalloc.deallocate(_rwlocks, _rwlocksize);
		}
		_rwlocks = NULL;
		_rwlocksize = 0;
		_lockalloc.destroy();
		//_nodealloc.destroy();
		_setalloc.destroy();
		return -1;
	}

	void destruct_node(node_pointer node, size_t key) {
		BSL_RWLOCK_T &__lock = _rwlocks[key % _rwlocksize];
		BSL_RWLOCK_WRLOCK(&__lock);
		Lsc::Lsc_destruct(&(_base._sp_alloc.getp(node)->val));
		_base._sp_alloc.deallocate(node, 1);
		BSL_RWLOCK_UNLOCK(&__lock);
	}
private:
    /**
     */
    Lsc_phashtable(const _Self&);           //����
    /**
     */
    _Self & operator = (const _Self &);     //����
 
plclic:
    /**
     * ���쳣
     * ���create
     */
	Lsc_phashtable() {
        _reset();
	}
    /**
     *        �������ʧ�ܣ������ٶ��� 
     * 
     * ���쳣
     * �����create
     *
     **/
    Lsc_phashtable(size_t bitems, size_t rwlsize = 0,
                const _HashFun &hf = _HashFun(), 
                const _Equl &eq = _Equl()) {
        _reset();
        if (create(bitems,rwlsize,hf,eq) != 0) {
            destroy();
            throw BadAllocException()<<BSL_EARG
                <<"create error when create phashtable with bitems "<<bitems;
        }
    }
	
    ~Lsc_phashtable() {
		destroy();
	}

    /**
     */
	iterator begin() {
		return _base.begin();
	}

    /**
     */
	const_iterator begin() const {
		return _base.begin();
	}
    /**
     */
	iterator end() {
		return _base.end();
	}
    /**
     */
	const_iterator end() const {
		return _base.end();
	}
    /**
     */
	size_t size() const {
		return _tmpsize;
	}

	/**
	 *
	 * 		PHASH_NORMAL ��ʾĿǰ��������״̬
	 * 		PHASH_START_CHECKPOINT ��ʾĿǰ��ʼCHECKPOINTER
	 * 		PHASH_END_CHECKPOINT ��ʾcheckpoint��������ʼ��������
	**/
	int dumpstatus() const {
		return _flag;
	}

	/**
	 *
	**/
	int create(size_t bitems, size_t rwlsize = 0,
			const _HashFun &hf = _HashFun(), 
			const _Equl &eq = _Equl()) {
		//�ж����
		if (bitems >= size_t(-1) / sizeof(void *)) {
			__BSL_ERROR("too large bitems, overflower");
			return -1;
		}

        //�ж�hashͰ��
        if(0 == bitems){
            __BSL_ERROR("bitems == 0");
        }

		if (destroy() != 0) {
			__BSL_ERROR("destroy error when create hash bitems[%lu]", (unsigned long)bitems);
			return -1;
		}
		if (recreate(bitems, rwlsize) != 0) {
			__BSL_ERROR("recreate error when create hash bitems[%lu], rwlsize[%lu]", 
					(unsigned long)bitems, (unsigned long)rwlsize);
			return -1;
		}

		if (pthread_mutex_init(&_cplock, NULL) != 0) {
			int ret = destroy();
			__BSL_ERROR("init cplock err, destroy data[%d]", ret);
			return -1;
		}
		if (pthread_mutex_init(&_uplock, NULL) != 0) {
			int ret = destroy();
			__BSL_ERROR("init uplock err, destroy data[%d]", ret);
			pthread_mutex_destroy(&_cplock);
			return -1;
		}
		_mlockcreat = true;

		_hashset = NULL;
		return _base.create(bitems, hf, eq);
	}

    /* *
     * */
    bool is_created() const{
        return _base.is_created();
    }

	/**
	 *
	 * 			ʧ�ܵ��������������dump����
	**/
	int destroy() {
		if (_flag != PHASH_NORMAL) {
			return -1;
		}
		if (_rwlocks) {
			for (size_t i=0; i<_rwlocksize; ++i) {
				BSL_RWLOCK_DESTROY(_rwlocks + i);
			}
			//__BSL_DEBUG("release rwlocks[%ld]", _rwlocksize);
			_lockalloc.deallocate(_rwlocks, _rwlocksize);
		}
		_rwlocks = NULL;
		_rwlocksize = 0;

		if (_tmplist) {
			//_nodealloc.deallocate(_tmplist, _base._bitems);
			_base._np_alloc.deallocate(_tmplist, _base._bitems);
		}
		_tmplist = NULL;

		if (_hashset != NULL) {
			_hashset->destroy();
			Lsc::Lsc_destruct(_hashset);
			_setalloc.deallocate(_hashset, 1);
			_hashset = NULL;
		}

		_lockalloc.destroy();
		//_nodealloc.destroy();
		_setalloc.destroy();
		if (_mlockcreat) {
			pthread_mutex_destroy(&_cplock);
			pthread_mutex_destroy(&_uplock);
			_mlockcreat = false;
		}
		return _base.destroy();
	}

	
	/**
	 *
	 * 			�����������checkpoint״̬��clear����-1
	**/
	int clear() {
		if (!_mlockcreat) {//û�г�ʼ����
			return 0;
		}
		mutexlock_monitor q(&_uplock);
		if (_flag != PHASH_NORMAL) {
			return -1;
		}
		for (size_t i=0; i<_base._bitems; ++i) {
			node_pointer nd = 0;
			while (_base._bucket[i]) {
				nd = _base._bucket[i];
				_base._bucket[i] = _base._sp_alloc.getp(_base._bucket[i])->next;
				destruct_node(nd, i);
			}
		}
		_base._size = 0;
		_tmpsize = 0;
		return 0;
	}

	/**
	 *
	**/
	int make_checkpoint() {
		//����cp ����up��ֹ����
		//mutexlock_monitor l(&_cplock);
		//mutexlock_monitor q(&_uplock);
		if (pthread_mutex_trylock(&_cplock) != 0) {
			return -1;
		}
		if (pthread_mutex_lock(&_uplock) != 0) {
			pthread_mutex_unlock(&_cplock);
			return -1;
		}
		int ret = _make_checkpoint();
		pthread_mutex_unlock(&_uplock);
		pthread_mutex_unlock(&_cplock);

		return ret;
	}

    /**
	 * 	make_checkpoint����˲����ڴ潫����ס������Զ�����ڴ���������߳�����
	 * 	������������.
	 *
	**/
	int end_checkpoint() {
		mutexlock_monitor l(&_cplock);
		return _end_checkpoint();
	}

	/**
	 * 		����ar.write()�������л���ָ��λ��
	 * 		����ʵ�ֵ�����ν������������
	 *
	 * 				�������make_checkpoint����������֪�����л���Щ�ڴ�
	 * 				��Ϊ�����д�£�hash���״̬�Ǳ仯��,�������л��ڴ��������
	 * 				make_checkpoint˲����ڴ�ӳ�䣬�� serialization ��������֮ǰ��
	 * 				���еĸ��²����������ᱻ���л�
	 *
	 * 				�����û�е���checkpoint����ô serialization �������õ�ʱ�򣬻�
	 * 				�����������һ����dump��ǰ�ڴ�
	 *
	 * 				�̰߳�ȫ
	**/
	template <class _Archive>
	int serialization(_Archive & ar) {
		mutexlock_monitor l(&_cplock);
		int ret = -1;
		__BSL_DEBUG("this = 0x%lx", (long)(this));
		//��û����ȡ��������Ҫ����make_checkpoint��ȡ�� 
		{
			mutexlock_monitor q(&_uplock);
			if (_flag == PHASH_NORMAL) {
				ret = _make_checkpoint();
				if (ret != 0) {
					return -1;
				}
			}
			if (_flag != PHASH_START_CHECKPOINT) {
				return -1;
			}
		}
		{
			if (_base.serialization(ar) != 0) {
				_end_checkpoint();
				return -1;
			}
		}

		__BSL_DEBUG("_flag = %d", _flag);
		//checkpoint���֮�����β����
		return _end_checkpoint();
	}

	/**
	 *
	**/
	template <class _Archive>
	int deserialization(_Archive & ar) {
		size_t bitems = 0;
		if (Lsc::deserialization(ar, bitems) != 0) {
			__BSL_ERROR("deserialization bitems error");
			return -1;
		}
		__BSL_DEBUG("bitems = %ld", (long)bitems);

		//�Ѿ�����ʼ����
		if (_base._bucket) {
			if (clear() != 0) {
				__BSL_ERROR("clear phashtable error");
				return -1;
			}
		} else {	//û�г�ʼ����
			if (create(bitems) != 0) {
				__BSL_ERROR("create hashtable error");
				return -1;
			}
		}

		size_t __size = 0;
		if (Lsc::deserialization(ar, __size) != 0) {
			__BSL_ERROR("deserialization size error");
			return -1;
		}
		value_type val;
		for (size_t i=0; i<__size; ++i) {
			if (Lsc::deserialization(ar, val) != 0) {
				__BSL_ERROR("deserialization val at[%ld] error", (long)i);
				return -1;
			}
			if (set(_base._getkey(val), val) == -1) {
				__BSL_ERROR("deserialization set at[%ld] error", (long)i);
				return -1;
			}
		}

		return 0;
	}

	/**
	 *
	 * 				���value��Ϊ�գ���*val��ֵ���ҵ�ֵ
	 *				���� HASH_EXIST		��ʾhashֵ����
	 *				���� HASH_NOEXIST	��ʾhashֵ������
	**/
	int get(const key_type &k, value_type *val = 0) const {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[ key % _rwlocksize];
		BSL_RWLOCK_RDLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (val) {
				*val = *ptr;
			}
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}
    
    /**
     */
	int get(const key_type &k, value_type *val = 0) {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[ key % _rwlocksize];
		BSL_RWLOCK_RDLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (val) {
				*val = *ptr;
			}
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}
    
    /**
     * ������Ҫԭֵ������NULLָ��
     *  _ValOpFunc�����Ǻ�����functor������Ϊ(value_type*, void*)
     */
    template <typename _ValOpFunc>
	int get(const key_type &k, value_type *old_val, _ValOpFunc val_op_func, void *args = 0){
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[ key % _rwlocksize];
		BSL_RWLOCK_WRLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (old_val) {
				*old_val = *ptr;
			}
            val_op_func(ptr, args);
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}

    /**
	 *
	 *				���� HASH_EXIST		��ʾhashֵ����
	 *				���� HASH_NOEXIST	��ʾhashֵ������
	**/
	template <typename _Second>
	int get_map(const key_type &k, _Second *val = 0) const {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[key % _rwlocksize];
		BSL_RWLOCK_RDLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (val) {
				*val = ptr->second;
			}
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}
    
    /**
	**/
	template <typename _Second>
	int get_map(const key_type &k, _Second *val = 0) {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[key % _rwlocksize];
		BSL_RWLOCK_RDLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (val) {
				*val = ptr->second;
			}
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
	}
    
    /**
     * ������Ҫԭֵ������NULLָ��
     * _ValOpFunc�����Ǻ���ָ�룬Ҳ������functor����������Ϊ(_Second*, void*)
	 **/
	template <typename _Second, typename _ValOpFunc>
	int get_map(const key_type &k, _Second *old_val, _ValOpFunc val_op_func, void *args = 0) {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		BSL_RWLOCK_T &__lock = _rwlocks[key % _rwlocksize];
		BSL_RWLOCK_WRLOCK(&__lock);
		value_type *ptr = _get(key, k);
		int ret = HASH_NOEXIST;
		if (ptr) {
			ret = HASH_EXIST;
			if (old_val) {
				*old_val = ptr->second;
			}
            val_op_func(&(ptr->second), args);
		}
		BSL_RWLOCK_UNLOCK(&__lock);
		return ret;
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
        if(0 == _base._bitems){
            return -1;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		mutexlock_monitor q(&_uplock);
		int ret = _set(key, k, val, flag);
		return ret;
	}

	/**
	 *
	 * 		����	HASH_EXIST��ʾ�����ڲ�ɾ���ɹ�
	 * 		����	HASH_NOEXIST��ʾ��㲻���ڲ���ɾ��
	**/
	int erase(const key_type &k) {
        if(0 == _base._bitems){
            return HASH_NOEXIST;
        }
		size_t key = _base._hashfun(k) % _base._bitems;
		mutexlock_monitor q(&_uplock);
		int ret = _erase(key, k);
		return ret;
	}

	/**
	 *
	 * 		�ɹ����� 0
	 * 		�������û�е���create�������أ�1ʧ��
	 * 		�������cleanʧ�ܣ�������-1
	 * 		�����������checkpoint״̬�������أ�1ʧ��
	**/
	template <class _Iterator>
	int assign(_Iterator __begin, _Iterator __end) {
		if (clear() != 0) {
			__BSL_ERROR("inside assign clear error");
			return -1;
		}
		if (_base._bitems <= 0) {
			__BSL_ERROR("didn't create before assign");
			return -1;
		}
		//__BSL_DEBUG("start to phashtable assign");
		mutexlock_monitor q(&_uplock);
		//__BSL_DEBUG("inside lock");
		if (_flag == PHASH_NORMAL) {
			for (_Iterator iter = __begin; iter != __end; ++iter) {
				if ( _base.set(_base._hashfun(_base._getkey(*iter)), 
						_base._getkey(*iter), *iter) == -1 ) {
					__BSL_ERROR("set error in phashtable assign");
					return -1;
				}
				++ _tmpsize;
			}
			return 0;
		}
		return -1;
	}

plclic:
	value_type * _get(size_t key, const key_type &k) const {
		//���������
		if (_flag == PHASH_NORMAL) {
			return _base.find(key, k);
		} else {
			//�������ϲ��Ƿ�������
			node_pointer lst = _tmplist[key];
			while (lst) {
				if (_base._equl(_base._getkey(_base._sp_alloc.getp(lst)->val), k)) {
					return & _base._sp_alloc.getp(lst)->val;
				}
				lst = _base._sp_alloc.getp(lst)->next;
			}
			//������û�в鵽����
			//�����ϲ�������
			value_type *val = _base.find(key, k);
			//�鿴�Ƿ��־ɾ��
			if (_hashset->get(val) == HASH_EXIST) {
				return NULL;
			}
			return val;
		}
		return NULL;
	}
	value_type * _get(size_t key, const key_type &k) {
		//���������
		if (_flag == PHASH_NORMAL) {
			return _base.find(key, k);
		} else {
			//�������ϲ��Ƿ�������
			node_pointer lst = _tmplist[key];
			while (lst) {
				if (_base._equl(_base._getkey(_base._sp_alloc.getp(lst)->val), k)) {
					return & _base._sp_alloc.getp(lst)->val;
				}
				lst = _base._sp_alloc.getp(lst)->next;
			}
			//������û�в鵽����
			//�����ϲ�������
			value_type *val = _base.find(key, k);
			//�鿴�Ƿ��־ɾ��
			if (_hashset->get(val) == HASH_EXIST) {
				return NULL;
			}
			return val;
		}
		return NULL;
	}

	int _set(size_t key, const key_type &k, const value_type &val, int flag) {
		int ret = 0;
		switch (_flag) {
		case PHASH_START_CHECKPOINT:
			ret = _start_cp_set(key, k, val, flag);
			break;
		case PHASH_END_CHECKPOINT:
			ret = _end_cp_set(key, k, val, flag);
			break;
		default:
			ret = _normal_set(key, k, val, flag, _base._bucket);
			break;
		}
		if (ret == HASH_INSERT_SEC) {
			++ _tmpsize;
		}
		return ret;
	}

	int _end_cp_set(size_t key, const key_type &k, const value_type &val, int flag) {
#if 0
		value_type *ptr = _base.find(key, k);
		if (ptr) {	//�ײ�bucket�д��ڽ��
			//�鿴����Ƿ�ɾ��
			if (_hashset->get(ptr) != HASH_EXIST) {
				//��㲻����
				return _normal_set(key, k, val, flag, _base._bucket);
			}
		}
		return _normal_set(key, k, val, flag, _tmplist);
#endif
		value_type *ptr = _base.find(key, k);
		if (ptr) {	//�ײ�bucket�д��ڽ��
			//�鿴����Ƿ�ɾ��, �Ҷ���Ҳ�ǿյ�
			if (_hashset->get(ptr) != HASH_EXIST) {
				//���û�б�ɾ����
				if (_tmplist[key] == 0) {
					return _normal_set(key, k, val, flag, _base._bucket);
				} else {
					if (flag == 0) {
						return HASH_EXIST;
					} else {
						_normal_erase(key, k, _base._bucket);
						_normal_set(key, k, val, flag, _tmplist);
						return HASH_OVERWRITE;
					}
				}
			} else {
				return _normal_set(key, k, val, flag, _tmplist);
			}
		} else {
			if (_tmplist[key] == 0) {
				return _normal_set(key, k, val, flag, _base._bucket);
			} else {
				return _normal_set(key, k, val, flag, _tmplist);
			}
		}
		return 0;
	}
	int _start_cp_set(size_t key, const key_type &k, const value_type &val, int flag) {
		int tmp = 0;
		value_type *ptr = _base.find(key, k);
		if (ptr) {	//����
			if (flag) {
				//��־ɾ��
				if (_hashset->set(ptr) == HASH_INSERT_SEC) {
					tmp = 1;	//��ʾ�������Ԫ�أ�����Ч,���ڱ�־ɾ��
				}
			} else {
				//�������Ҫ�滻���鿴�Ƿ񱻱�־ɾ����
				if (_hashset->get(ptr) != HASH_EXIST) {
					//û�б���־ɾ������ֱ�ӷ���
					return HASH_EXIST;
				}
			}
		}
		//��Ҫ���������޸�����
		int ret = _normal_set(key, k, val, flag, _tmplist);
		if (ret != -1 && tmp == 1) {
			return HASH_OVERWRITE;
		}
		return ret;
	}

	int _normal_set(size_t key, const key_type &k, const value_type &val, 
			int flag, node_pointer * __bucket) {
		node_pointer lst = __bucket[key];
		node_pointer front = 0;
		int ret = -1;
		while (lst) {
			if (_base._equl(_base._getkey(_base._sp_alloc.getp(lst)->val), k)) {
				if (flag) {
					ret = HASH_OVERWRITE;
					break;
				}
				return HASH_EXIST;
			}
			front = lst;
			lst = _base._sp_alloc.getp(lst)->next;
		}
		node_pointer node = _base._sp_alloc.allocate(1);
		if (0 == node) {
			return -1;
		}

		Lsc::Lsc_construct(&(_base._sp_alloc.getp(node)->val), val);
        
        //insertion bug fixed.
        if (ret == HASH_OVERWRITE) {
            _base._sp_alloc.getp(node)->next = _base._sp_alloc.getp(lst)->next;  
            if (front) {
                 _base._sp_alloc.getp(front)->next = node;
            }else{
                 __bucket[key] = node;
            }
            destruct_node(lst, key);
            return ret;
        } else {
            _base._sp_alloc.getp(node)->next = __bucket[key];
            __bucket[key] = node;
        }

		return HASH_INSERT_SEC;
	}

	int _erase(size_t key, const key_type &k) {
		int ret = 0;
		switch (_flag) {
		case PHASH_START_CHECKPOINT:
			ret = _start_cp_erase(key, k);
			break;
		case PHASH_END_CHECKPOINT:
			ret = _end_cp_erase(key, k);
			break;
		default:
			ret = _normal_erase(key, k, _base._bucket);
			break;
		}
		if (ret == HASH_EXIST) {
			-- _tmpsize;
		}
		return ret;
	}

	int _start_cp_erase(size_t key, const key_type &k) {
		value_type *ptr = _base.find(key, k);
		if (ptr != NULL) {
			if (_hashset->set(ptr) == HASH_INSERT_SEC) {
				//���ڣ���־ɾ��
				return HASH_EXIST;
			}
		}
		return _normal_erase(key, k, _tmplist);
	}

	int _end_cp_erase(size_t key, const key_type &k) {
		int ret = _normal_erase(key, k, _tmplist);
		if (ret == HASH_NOEXIST) {
			value_type *ptr = _base.find(key, k);
			if (ptr == NULL || _hashset->get(ptr) == HASH_EXIST) {
				return HASH_NOEXIST;
			}
			return _normal_erase(key, k, _base._bucket);
		}
		return ret;
	}

	int _normal_erase(size_t key, 
                    const key_type &k, 
                    node_pointer *__bucket, 
                    deleteVec *delvec = NULL
                    ) {
		node_pointer lst = __bucket[key];
		node_pointer front = 0;
		while (lst) {
			if (_base._equl(_base._getkey(_base._sp_alloc.getp(lst)->val), k)) {
				if (front == 0) {
					__bucket[key] = _base._sp_alloc.getp(lst)->next;
				} else {
					_base._sp_alloc.getp(front)->next = _base._sp_alloc.getp(lst)->next;
				}
				if (delvec == NULL) {
					destruct_node(lst, key);
				} else {
					deleteNode delnode;
					delnode.lst = lst;
					delnode.key = key;
					try {
						delvec->push_back(delnode);
					} catch (...) {
						destruct_node(lst, key);
					}
				}
				return HASH_EXIST;
			}
			front = lst;
			lst = _base._sp_alloc.getp(lst)->next;
		}
		return HASH_NOEXIST;
	}

private:
    void _reset(){
        _rwlocks = NULL;
		_rwlocksize = 0;
		_tmpsize = 0;
		_tmplist = NULL;
		_flag = PHASH_NORMAL;
		_hashset = NULL;
		_mlockcreat = false;
    }

	int _make_checkpoint() {
		if (_flag != PHASH_NORMAL) {
			return -1;
		}
		if (_tmplist == NULL) {
			//_tmplist = _nodealloc.allocate(_base._bitems);
			_tmplist = _base._np_alloc.allocate(_base._bitems);
			if (_tmplist == NULL) {
				return -1;
			}
			memset(_tmplist, 0, sizeof(node_pointer ) * _base._bitems);
		}
		if (_hashset == NULL) {
			_hashset = _setalloc.allocate(1);
			Lsc::Lsc_construct(_hashset);
			if (_hashset == NULL) {
				return -1;
			}
			if (_hashset->create(__Lsc_next_prime(1<<15)) != 0) {
				Lsc::Lsc_destruct(_hashset);
				_setalloc.deallocate(_hashset, 1);
				_hashset = NULL;
				return -1;
			}
		}
		_flag = PHASH_START_CHECKPOINT;
		_base._size = _tmpsize;
		__BSL_DEBUG("make checkpoint end %lu %lu", (long)_base._size, (long)_tmpsize);
		return 0;
	}

	int _end_checkpoint() {
		//__BSL_DEBUG("start to end checkpoint");
		if (_flag != PHASH_START_CHECKPOINT) {
			//__BSL_ERROR("dumpstatus error flag[%d]", _flag);
			return -1;
		}

		AUTO_TIMER("end checkpoint");
		{
			mutexlock_monitor l(&_uplock);
			_flag = PHASH_END_CHECKPOINT;
		}
		{
			{
				__BSL_DEBUG("lock _hashset->lock");
				mutexlock_monitor t(&_hashset->_lock);
				__BSL_DEBUG("in lock _hashset->lock items %lu", (long)_hashset->_ht._bitems);
				for (size_t i=0; i<_hashset->_ht._bitems; ++i) {
					mutexlock_monitor l(&_uplock);
					BSL_RWLOCK_T &__lock = _hashset->get_rwlock(i);
					BSL_RWLOCK_WRLOCK(&__lock);
					deleteVec delvec;
					typename _Set::hash_type::node_pointer  nd = 0;
					while (_hashset->_ht._bucket[i]) {
						nd = _hashset->_ht._bucket[i];
						{
							const key_type & key = 
                                        _base._getkey(*(_hashset->_ht._sp_alloc.getp(nd)->val));
							_normal_erase(_base._hashfun(key) % _base._bitems, 
                                        key, 
                                        _base._bucket, 
                                        &delvec
                                        );
						}
						//����get��ʱ�� �������ܳ�������
						_hashset->_ht._bucket[i] = 
                                    _hashset->_ht._sp_alloc.getp(_hashset->_ht._bucket[i])->next;
						Lsc::Lsc_destruct(&(_hashset->_ht._sp_alloc.getp(nd)->val));
						_hashset->_ht._sp_alloc.deallocate(nd, 1);
					}
					BSL_RWLOCK_UNLOCK(&__lock);
					for (typename deleteVec::iterator delIter = delvec.begin();
							delIter != delvec.end(); ++delIter) {
						destruct_node(delIter->lst, delIter->key);
					}
				}
				__BSL_DEBUG("walk _ht._bitems");
			}
			__BSL_DEBUG("base bitems %lu", (long)_base._bitems);
			for (size_t i=0; i<_base._bitems; ++i) {
				//__BSL_DEBUG("start in uplock");
				mutexlock_monitor l(&_uplock);
				//__BSL_DEBUG("inside uplock");
				if (0) {
					node_pointer nd = _tmplist[i];
					while (nd) {
						const key_type & key = _base._getkey(_base._sp_alloc.getp(nd)->val);
						if (_normal_erase(i, key, _base._bucket) == HASH_EXIST) {
							-- _tmpsize;
							__BSL_DEBUG("has same id %d", (int)i);
						}
						nd = _base._sp_alloc.getp(nd)->next;
					}
				}
				//__BSL_DEBUG("inside uplock");
				if (_tmplist[i] != 0) {
					node_pointer lst = _base._bucket[i];
					node_pointer front = 0;
					while (lst) {
						front = lst;
						lst = _base._sp_alloc.getp(lst)->next;
					}
					if (front) {
						_base._sp_alloc.getp(front)->next = _tmplist[i];
					} else {
						_base._bucket[i] = _tmplist[i];
					}
					_tmplist[i] = 0;

				}
			}
			__BSL_DEBUG("walk _tmplist");
		}
		//__BSL_DEBUG("_flag = %d", _flag);
		{
			mutexlock_monitor l(&_uplock);
			_flag = PHASH_NORMAL;
		}
		return 0;
	}
};

}
#endif  //__BSL_PHASHTABLE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

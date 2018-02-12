
#ifndef _BSL_LIST_H__
#define _BSL_LIST_H__

#include <Lsc/utils/Lsc_utils.h>
#include <Lsc/alloc/Lsc_alloc.h>
#include <Lsc/alloc/Lsc_sample_alloc.h>
#include <Lsc/archive/Lsc_serialization.h> 
#include <Lsc/exception/Lsc_exception.h>

namespace Lsc
{

template <typename _Tp, typename _Alloc>
struct list_node_t
{
	typedef _Tp value_type;
	typedef list_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	node_pointer next;
	node_pointer prev;
	value_type val;
};

template <class _Tp, class _Alloc > class list;

/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class list_const_iterator
{
plclic:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
	typedef list_const_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef list<_Tp, _Alloc> list_t;
	typedef typename list_t::iterator iterator;
	typedef list_node_t<value_type, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	typedef typename node_alloc_t::const_pointer node_const_pointer;
plclic:
	/**
	 * ����һ��δ��ʼ���ĵ�����
	**/
	list_const_iterator()
	{
		_node = 0;
	}
	/**
	 * ����һ�����������ƣ�ָ��ͳһ������ 
	**/
	list_const_iterator(const self & __x)
	{
		_node = __x._node;
	}
	list_const_iterator(const iterator & __x)
	{
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
		return &_node->val;
	}
	self & operator ++ ()
	{
		_node = _node->next;
		return *this;
	}
	self operator ++ (int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	self & operator --()
	{
		_node = _node->prev;
		return *this;
	}
	self operator --(int)
	{
		self iter = *this;
		-- *this;
		return iter;
	}
	/**
	 * ��һ��ָ��ڵ���ڴ�ָ�뽨�����������ڲ�ʹ��
	**/
	list_const_iterator(node_pointer __x)
	{
		_node = __x;
	}
	
	list_const_iterator(node_const_pointer __x)
	{
		_node = const_cast<node_pointer>(__x);
	}
private:
	node_pointer _node;
};

/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class list_iterator
{
plclic:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
	typedef const _Tp * const_pointer;	//���ռ��ַָ��
	typedef const _Tp & const_reference;	//���ռ��ַָ��
private:
	typedef list_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef list<_Tp, _Alloc> list_t;
	typedef list_node_t<value_type, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	typedef typename node_alloc_t::const_pointer node_const_pointer;
	friend class list<_Tp, _Alloc>;
plclic:
	/**
	 * ����һ��δ��ʼ���ĵ�����
	**/
	list_iterator()
	{
        _list = 0;
		_node = 0;
	}
	/**
	 * ����һ�����������ƣ�ָ��ͳһ������ 
	**/
	list_iterator(const self & __x)
	{
        _list = __x._list;
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
		return &_node->val;
	}
	self & operator ++ ()
	{
		_node = _node->next;
		return *this;
	}
	self operator ++ (int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	self & operator --()
	{
		_node = _node->prev;
		return *this;
	}
	self operator --(int)
	{
		self iter = *this;
		-- *this;
		return iter;
	}
	/**
	 * ��һ��ָ��ڵ���ڴ�ָ�뽨�����������ڲ�ʹ��
	**/
	list_iterator(list_t* __l, node_pointer __x)
	{
        _list = __l;
		_node = __x;
	}
	
	list_iterator(list_t* __l, node_const_pointer __x)
	{
        _list = const_cast<list_t*>(__l);
		_node = const_cast<node_pointer>(__x);
	}
plclic:
    node_pointer _node;
private:
    list_t* _list;
};

/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class list_const_reverse_iterator
{
plclic:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
	typedef list_const_reverse_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef list<_Tp, _Alloc> list_t;
	typedef typename list_t::reverse_iterator reverse_iterator;
	typedef list_node_t<value_type, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	typedef typename node_alloc_t::const_pointer node_const_pointer;
	friend class list<_Tp, _Alloc>;
plclic:
	list_const_reverse_iterator()
	{
		_node = 0;
	}
	list_const_reverse_iterator(const reverse_iterator & __x)
	{
		_node = __x._node;
	}
	list_const_reverse_iterator(const self& __x)
	{
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
        return &_node->val;
	}
	self & operator -- ()
	{
		_node = _node->next;
		return *this;
	}
	self operator -- (int)
	{
		self iter = *this;
		-- *this;
		return iter;
	}
	self & operator ++()
	{
		_node = _node->prev;
		return *this;
	}
	self operator ++(int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	/**
	 *
	**/
	list_const_reverse_iterator(node_pointer __x)
	{
		_node = __x;
	}
	list_const_reverse_iterator(node_const_pointer __x)
	{
		_node = const_cast<node_pointer>(__x);
	}
private:
    node_pointer _node;
};
/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class list_reverse_iterator
{
plclic:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
	typedef list_const_reverse_iterator<_Tp, const _Tp *, 
                                        const _Tp &, _Alloc> const_reverse_iterator;
private:
	typedef list_reverse_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef list<_Tp, _Alloc> list_t;
	typedef list_node_t<value_type, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	typedef typename node_alloc_t::const_pointer node_const_pointer;
	friend class list<_Tp, _Alloc>;
plclic:
	list_reverse_iterator()
	{
        _list = 0;
		_node = 0;
	}
	list_reverse_iterator(const self& __x)
	{
        _list = __x._list;
		_node = __x._node;
	}
	reference operator *() const
	{
		return _node->val;
	}
	bool operator == (const self & __iter) const
	{
		return (_node == __iter._node);
	}
	bool operator != (const self & __iter) const
	{
		return (_node != __iter._node);
	}
	pointer operator ->() const
	{
		return &_node->val;
	}
	self & operator -- ()
	{
        _node = _node->next;
		return *this;
	}
	self operator -- (int)
	{
		self iter = *this;
		-- *this;
		return iter;
	}
	self & operator ++()
	{
        _node = _node->prev;
		return *this;
	}
	self operator ++(int)
	{
		self iter = *this;
		++ *this;
		return iter;
	}
	/**
	 *
	**/
	list_reverse_iterator(list_t* __l, node_pointer __x)
	{
        _list = __l;
		_node = __x;
	}
	list_reverse_iterator(list_t* __l, node_const_pointer __x)
	{
        _list = const_cast<list_t*>(__l);
		_node = const_cast<node_pointer>(__x);
	}
plclic:
    node_pointer _node;
private:
    list_t* _list;
};

/**
 */
template <class _Tp, class _Alloc = Lsc_sample_alloc<Lsc_alloc<_Tp>, 256> >
class list
{
plclic:
	typedef _Tp value_type;	//�������Ͷ���
	typedef _Tp *  pointer;	//�ռ��ַָ��
	typedef const _Tp * const_pointer;	//���ռ��ַָ��
	typedef _Tp & reference; 
	typedef const _Tp & const_reference; 
	typedef size_t size_type;
	typedef long difference_type;
	typedef list_iterator<value_type, pointer, reference, _Alloc> iterator;
	typedef list_const_iterator<value_type, const_pointer, 
                                            const_reference, _Alloc> const_iterator;
	typedef list_reverse_iterator<value_type, pointer, reference, _Alloc> reverse_iterator;
	typedef list_const_reverse_iterator<value_type, const_pointer, 
                                            const_reference, _Alloc> const_reverse_iterator;
protected:
	typedef list_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	typedef typename node_alloc_t::const_pointer node_const_pointer;
	//typedef typename node_alloc_t::reference node_reference;
	//typedef typename node_alloc_t::const_reference node_const_reference;
	//friend class list_iterator<value_type, pointer, reference, _Alloc>;
	//friend class list_iterator<value_type, const_pointer, const_reference, _Alloc>;
	//friend class list_reverse_iterator<value_type, pointer, reference, _Alloc>;
	//friend class list_reverse_iterator<value_type, const_pointer, const_reference, _Alloc>;
	
private:
	struct list_node_place_holder_t
	{
		node_pointer next;
		node_pointer prev;
		//��������ĸ��ڵ��ڳ����б���ε��ã�
		//Ϊ�˱����ڳ�������ʽ��node_holder_t*ת��Ϊnode_t*, �������µ��Զ�����ת��
		operator node_pointer ()
		{
			return reinterpret_cast<node_pointer>(this);
		}
		operator node_const_pointer () const
		{
			return reinterpret_cast<node_const_pointer>(this);
		}
	};
	typedef list_node_place_holder_t node_holder_t;
	typedef list<_Tp,_Alloc> self;
	node_holder_t _root;
	node_alloc_t _node_allocator;
    size_type _size;
    
plclic:

    /* *
     *  ���쳣
     *  �����create
     * */
    list(){
        _reset();
        create();
    }

    /**
	 * �������캯��
     * �������ʧ�ܣ��׳����쳣���ָ���ԭ���Ŀ�list
     *
	**/
	list(const self& other)
	{
        _reset();
        create();
        if (assign(other.begin(),other.end()) != 0) {
            throw BadAllocException()<<BSL_EARG<<"assign list error";
        }
	}

	/**
     * �������ʧ�ܣ����ָ���ԭlist��ǿ�쳣��ȫ��֤
     * ���ɾ��ʧ�ܣ���ĩβɾ����㣬���������Դй©�������쳣��ȫ��֤
	**/
	self& operator = (const self& other)
	{
        if (this != &other) {
            size_t size1    = size(); // size()��ʱ�临�Ӷ�ΪO(1)
            size_t size2    = other.size();
            if (size2 > size1) { // this����Ҫ��ӽ��
                // other���������this��ǰ������
                const_reverse_iterator rfirst2 = other.rbegin();
                for (size_t i = size1; i < size2; i ++) {
                    if (push_front( *rfirst2 ) != 0) {
                        for (size_t j = size1; j < i; j ++) {
                            pop_front();
                        }
                        throw BadAllocException()<<BSL_EARG
                            <<"push_front list error size["<<size1<<"~"<<size2<<"]";
                    }
                    ++ rfirst2;
                }
                // size = 0�������ͷ���
                if (size1 != 0) {
                    reverse_iterator rfirst1 = rbegin();
                    for (size_t i = 0; i < size1; i ++) {
                        *rfirst1 ++ = *rfirst2 ++;
                    }
                    // rfirst1Ϊʵ��begin()��ǰһλ 
                    _root.prev->next = _root.next;
                    _root.next->prev = _root.prev;
                    
                    _root.next = rfirst1._node->next;
                    _root.prev = rfirst1._node;

                    rfirst1._node->next = _root;
                    rfirst1._node->next->prev = _root;    
                }
            } else { // size1 >= size2
                // ��ĩβɾ�����
                for (size_t i = size1; i > size2; -- i) {
                    if ( pop_back() != 0 ) {
                        throw BadAllocException()<<BSL_EARG
                            <<"pop_back list error index["<<i<<"] size["<<size1<<"~"<<size2<<"]";
                    }
                }
                iterator first1         = begin();
                iterator last1          = end();
                const_iterator first2   = other.begin();
                const_iterator last2    = other.end();
                while ( first1 != last1 && first2 != last2 ) {
                    *first1 ++ = *first2 ++;
                }
            }       
        }
		return *this; 
	}

	/**
	 * ����������ڲ��ṹ���ڴ������
     * ���push_backһЩԪ����create��������ڴ�й¶
     *
	**/
	int create()
	{
		_node_allocator.create(); //allocator���޲���create���ǳɹ�
        return 0;
	}

	/**
	 * ɾ���������ռ�õ��ڴ�
	**/
	int destroy()
	{
		this->clear();
		_node_allocator.destroy();
		return 0;

	}
	/**
	 * ��������
	**/
	~list()
	{
		destroy();
	}
	/**
	 * ��__x�и��ƽڵ㣬���__x���䱾��������
	**/
	int assign (const self & __x)
	{
		if (this == &__x)
		{
			return 0;
		}
		else
		{
			return assign(__x.begin(), __x.end());
		}
	}

	/**
	 * �ӵ������и�������������������ָ���������
	**/
	template <typename InputIterator>	
	int assign(InputIterator __begin, InputIterator __end)
	{
		node_holder_t tmp_root;
		tmp_root.prev = tmp_root;
		node_pointer cur = tmp_root;
		InputIterator tp = __begin;
        size_type n = 0;
		while(tp != __end)
		{
			node_pointer tmp = _node_allocator.allocate(1);
			if (!tmp) {
				goto err1;
            }
			Lsc::Lsc_construct(&tmp->val, *tp);
			tmp->prev = cur;
			cur->next = tmp;
			cur = tmp;
			++tp;
            ++n;
		}
		cur->next = tmp_root;
		tmp_root.prev = cur;
		clear();
		_root.next = tmp_root.next;
		tmp_root.next->prev = _root;
		_root.prev = tmp_root.prev;
		tmp_root.prev->next = _root;
        _size = n;
		return 0;
	err1:
		while(cur->prev != tmp_root)
		{
			node_pointer tmp = cur->prev;
			Lsc::Lsc_destruct(&(cur->val));
			_node_allocator.deallocate(cur, 1);
			cur = tmp;

		}
		return -1;
	}

	/**
	 * ���ȫ���ڵ�
	**/
	int clear()
	{
		node_pointer cur = _root.next;
		while(cur != _root)
		{
			node_pointer tmp = cur->next;
			Lsc::Lsc_destruct(&(cur->val));
			_node_allocator.deallocate(cur, 1);
			cur = tmp;
            --_size;
		}
		_root.next = _root.prev = _root;
        _size = 0;
		return 0;
	}

	/**
	 * ���������Ƿ��������
	**/
	bool empty() const
	{
		return _root.prev == _root;
	}

	/**
	 * ��ͷ�������½ڵ�
	**/
	inline int push_front(const value_type &__x)
	{
		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp) {
			goto err;
        }
		Lsc::Lsc_construct(&tmp->val, __x);
		tmp->next = _root.next;
		tmp->prev = _root;
		_root.next->prev = tmp;
		_root.next = tmp;
        ++_size;
		return 0;
	err:
		return -1;
	}

	/**
	 * ɾ��ͷ�ڵ�
	**/
	int pop_front()
	{
		return erase(begin());
		/*
		if(_root.next == _root)
		{
			return -1;
		}
		else
		{
			node_pointer tmp = _root.next;
			_root.next = tmp->next;
			tmp->next->prev = _root;
			Lsc::Lsc_destruct(&(tmp->val));
			_node_allocator.deallocate(tmp, 1);
			return 0;
		}*/
	}

	/**
	 * ��β�����ӽڵ�
	**/
	int push_back(const value_type &__x)
	{
		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp) {
			goto err;
        }
		Lsc::Lsc_construct(&tmp->val, __x);
		tmp->prev = _root.prev;
		tmp->next = _root;
		_root.prev->next = tmp;
		_root.prev = tmp;
        ++_size;
		return 0;
	err:
		return -1;
	}

	/**
	 * ɾ��β�ڵ�
	**/
	int pop_back()
	{
    	if(_root.prev == _root)
		{
			return -1;
		}
		else
		{
			node_pointer tmp = _root.prev;
			_root.prev = tmp->prev;
			tmp->prev->next = _root;
			Lsc::Lsc_destruct(&(tmp->val));
			_node_allocator.deallocate(tmp, 1);
            --_size;
			return 0;
		}
	}

	/**
	 * ��__iָ���Ԫ��ǰ��������__s��__eָ���Ľڵ�
	**/
	template <typename _InputIterator>
	inline int insert( iterator __i,  _InputIterator __s, _InputIterator __e)
	{
		typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
		return(_aux_insert(__i, __s, __e, _Integral()));
	}
	template<typename InputIterator> 
	inline int _aux_insert(iterator __i, InputIterator __s, InputIterator __e,
			__true_type)
	{
		return _insert(__i, __s, __e);
	}
	template<typename InputIterator> 
	int _aux_insert(iterator __i, InputIterator __s, InputIterator __e, 
			__false_type)
	{
        if ( __s == __e ) {
            return 0;
        }

       	InputIterator tp = __s;
		node_holder_t tmp_root;
		tmp_root.prev = tmp_root;
		node_pointer cur = tmp_root;
        size_type n = 0;
		while(tp!=__e)
		{
			node_pointer tmp = _node_allocator.allocate(1);
			if (!tmp) {
				goto err;
            }
			Lsc::Lsc_construct(&tmp->val, *tp);
			tmp->prev = cur;
			cur->next = tmp;
			cur = tmp;
			++tp;
            ++n;
		}
		__i._node->prev->next = tmp_root.next;
		tmp_root.next->prev = __i._node->prev;
		cur->next = __i._node;
		__i._node->prev = cur;
        __i._list->_size += n;
		return 0;
	err:
		while(cur->prev != tmp_root)
		{
			node_pointer tmp = cur->prev;
			Lsc::Lsc_destruct(&(cur->val));
			_node_allocator.deallocate(cur, 1);
			cur = tmp;
		}
		return -1;
	}
	/**
	 * ����__iָ��Ľӵ�֮ǰ������__n��ֵΪ__x��Ԫ��
	**/

	inline int insert( iterator __i, size_type __n, const value_type &__x)
	{
		return _insert(__i, __n, __x);
	}
	int _insert( iterator __i, size_type __n, const value_type &__x)
	{
        if ( __n == 0 ) { 
            return 0;
        }
       
		node_holder_t tmp_root;
		tmp_root.prev = tmp_root;
		node_pointer cur = tmp_root;
		size_type tcount = __n;
		while(tcount)
		{
			node_pointer tmp = _node_allocator.allocate(1);
			if (!tmp) {
				goto err;
            }
			Lsc::Lsc_construct(&tmp->val, __x);
			tmp->prev = cur;
			cur->next = tmp;
			cur = tmp;
			tcount--;
		}
		__i._node->prev->next = tmp_root.next;
		tmp_root.next->prev = __i._node->prev;
		cur->next = __i._node;
		__i._node->prev = cur;
        __i._list->_size += __n;
		return 0;
	err:
		while(cur->prev != tmp_root)
		{
			node_pointer tmp = cur->prev;
			Lsc::Lsc_destruct(&(cur->val));
			_node_allocator.deallocate(cur, 1);
			cur = tmp;

		}
		return -1;
	}
    
	/**
	 * �ڽڵ�__i֮ǰ������__x�ڵ�
	**/
	int insert( iterator __i, const value_type &__x)
	{
		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp) {
			goto err;
        }
		Lsc::Lsc_construct(&tmp->val, __x);
		tmp->next = __i._node;
		tmp->prev = __i._node->prev;
		__i._node->prev->next = tmp;
		__i._node->prev = tmp;
        __i._list->_size += 1;
		return 0;
	err:
		return -1;
	}

	/**
	 * ��__x������������
	**/
	int swap( self & __x)
	{
		_node_allocator.swap(__x._node_allocator);
		std::swap(_root, __x._root);
		if(_root.next != __x._root)
		{
			_root.next->prev = _root;
			_root.prev->next = _root;
		}
		else
		{
			_root.next = _root;
			_root.prev = _root;
		}
		if ( __x._root.next != _root)
		{
			__x._root.next->prev = __x._root;
			__x._root.prev->next = __x._root;
		}
		else
		{
			__x._root.next = __x._root;
			__x._root.prev = __x._root;
		}
        std::swap(_size, __x._size);
		//check();
		//__x.check();
		return 0;
	}


	/**
	 * ɾ��__s��__e֮��Ľڵ�
	**/
	int erase ( iterator __s, iterator __e)
	{
		node_pointer prev = __s._node->prev;
		node_pointer cur = __s._node;
		node_pointer t_end = __e._node;
        size_type n = 0;
		while(cur != t_end)
		{
			node_pointer tmp = cur->next;
			Lsc::Lsc_destruct(&(cur->val));
			_node_allocator.deallocate(cur, 1);
			cur = tmp;
            ++n;
		}
		prev->next = t_end;
		t_end->prev = prev;
        __s._list->_size -= n;
        return 0;
	}

	/**
	 * ɾ�� __xָ��Ľڵ�
	**/
	int erase ( iterator __x)
	{
		if(_root.next == _root || __x._node == (node_pointer)&_root) {
			return -1;
        }
		node_pointer cur = __x._node;
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		Lsc::Lsc_destruct(&(cur->val));
		_node_allocator.deallocate(cur, 1);
        __x._list->_size -= 1;
		return 0;
	}
	
	/**
	 * ����ͷ�ĳ�����
	**/
	inline const_reference front() const
	{
		if(_root.next == _root) {
			throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no front item in empty list.";
        }
		return _root.next->val;
	}
	
	/**
	 * ����ͷԪ�ص�����
	**/
	inline reference front()
	{
		if(_root.next == _root) {
            throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no front item in empty list.";
        }
		return _root.next->val;
	}
    
	/**
	 * ����β�ڵ�ĳ�����
	**/
	inline const_reference back() const
	{
		if(_root.next == _root) {
            throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no back item in empty list.";
        }
		return _root.prev->val;
	}
    
	/**
	 * ����β�ڵ������
	**/
	inline reference back()
	{
		if(_root.next == _root) {
            throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no front item in empty list.";
        }
		return _root.prev->val;
	}

	/**
	 * ����ָ��ͷ�ڵ�ĳ�������
	**/
	inline const_iterator begin() const
	{
		return const_iterator(_root.next);
	}

	/**
	 * ����ָ��ͷ�ڵ�ĵ�����
	**/
	inline iterator begin()
	{
		return iterator(this, _root.next);
	}

	/**
	 * ����ָ��β�ڵ�ĳ�������
	**/
	inline const_iterator end() const
	{
		return const_iterator(_root);
	}

	/**
	 * ����ָ��β�ڵ�ĵ�����
	**/
	inline iterator end()
	{
		return iterator(this, _root);
	}

	/**
	 * ����ָ���ײ��ķ��򳣵�����
	**/
	inline const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(_root.prev);
	}

	/**
	 * ����ָ���׽ڵ�ķ��򳣵�����
	**/
	inline reverse_iterator rbegin()
	{
		return reverse_iterator(this, _root.prev);
	}

	/**
	 * ����ָ��β�ڵ�ķ��򳣵�����
	**/
	inline const_reverse_iterator rend() const
	{
		return const_reverse_iterator(_root);
	}
	/**
	 * ����ָ��β�ڵ�ķ��������
	**/
	inline reverse_iterator rend()
	{
		return reverse_iterator(this, _root);
	}

	/**
	 * �������������Ľڵ�����
	**/
	size_type size() const
	{
        return _size;
	}

	/**
	 * ���������������ɵ����ڵ�����
	**/
	inline size_type max_size() const
	{
		return (size_type)(-1);
	}

	/*
	 * ͨ�����ӻ�ɾ��β���ڵ㣬ʹ�����Ľڵ���������__n
	**/
	int resize(size_type __n, value_type __x = value_type())
	{
		size_type tp = size();
		if(tp > __n)
		{
			for(difference_type i = 0; i < (difference_type)( tp-__n); i++)
			{
				if(pop_back()) {
					goto err;
                }
			}
		}
		else
		{
			for(difference_type i=0; i < (difference_type)( __n-tp); i++)
			{
				if(push_back(__x)) {
					goto err;
                }
			}
		}
		return 0;
	err:
		return -1;
	}
    
	/**
	 * ��__l�е�ȫ���ڵ���뵽__xָ��Ľ��֮ǰ������__l��ɾ��,
     * __l������һ����ͬ��__x���ڵ�����
	**/
	int splice(iterator __x, self & __l)
	{
		if(__l._root.prev != __l._root.next)
		{
            size_type n = __l.size();
			_node_allocator.merge(__l._node_allocator);
			node_pointer cur = __x._node;
			cur->prev->next = __l._root.next;
			__l._root.next->prev = cur->prev;
			cur->prev = __l._root.prev;
			__l._root.prev->next = cur;
			__l._root.next = __l._root.prev = __l._root;
            __x._list->_size += n;
            __l._size = 0;
		}
		return 0;
	}
	
#if 0
	/**
	 * ��__l��__pָ��Ľڵ�ɾ���������뵽��ǰ������__xָ��Ľڵ�֮��
	**/
	int splice(iterator __x, self & __l, iterator __p)
	{

		if(__p != __l.end())
		{
			node_pointer cur = __x._node;
			node_pointer target = __p._node;
			target->prev->next = target->next;
			target->next->prev = target->prev;
			cur->prev->next = target;
			target->prev = cur->prev;
			cur->prev = target;
			target->next = cur;
		}
		return 0;
	}

	/**
	 * ��__l��[__p,__e)����Ľڵ��__l��ɾ���������뵽__x�ڵ�֮ǰ��
	**/
	int splice(iterator __x, self & __l, iterator __p, iterator __e)
	{

		if (__p != __e)
		{
			node_pointer cur = __x._node;
			node_pointer end = __e._node->prev;
			node_pointer target = __p._node;

			target->prev->next = __e._node;
			__e._node->prev = target->prev;

			
			cur->prev->next = target;
			target->prev = cur->prev;

			cur->prev = end;
			end->next = cur;
			target->next->prev = target->prev;
		}
		return 0;
	}
#endif
    
	/**
	 * ��������ɾ����__xֵ��ͬ��ʵ��
	**/
	inline int remove(const value_type & __x)
	{
		return remove_if(std::bind2nd(std::equal_to<_Tp>(), __x));
	}
	/**
	 * ������������ɾ��Ԫ��
	**/
	template<class Predicate> 
	int remove_if(Predicate __p) 
	{
		node_pointer cur = _root.next;
		while(cur != _root)
		{
			if(__p(cur->val))
			{
				node_pointer tmp = cur;
				cur = cur->next;
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;
				Lsc::Lsc_destruct(&(tmp->val));
				_node_allocator.deallocate(tmp, 1);
                --_size;
			}
			else
			{
				cur = cur->next;
			}
		}
		return 0;
	}
    
	/**
	 * ������������
	**/
	int reverse()
	{
		node_pointer cur = _root.next;
		_root.next = _root.prev;
		_root.prev = cur;
		while(cur != _root)
		{
			node_pointer tmp = cur->next;
			cur->next = cur->prev;
			cur->prev = tmp;
			cur = tmp;
		}
		return 0;
	}

	/**
	 * ���л�
	**/
	template <typename _Archive>
	int serialization(_Archive &ar)
	{
		size_t l = size();
		if(Lsc::serialization(ar, l)) {
			return -1;
        }
		for(iterator i=begin(); i!=end(); i++)
		{
			if(Lsc::serialization(ar, *i)) {
				return -1;
            }
		}
		return 0;
	}

	/**
	 * �����л�
	**/
	template <typename _Archive>
	int deserialization(_Archive &ar)
	{
		size_t l;
		clear();
		if(Lsc::deserialization(ar, l)) {
			return -1;
        }
		iterator cur = begin();
		for(size_t i=0; i<l; i++)
		{
			value_type tmp;
			if(Lsc::deserialization(ar, tmp)) {
				goto err;
            }
			if(insert(cur, tmp)) {
				goto err;
            }
		}
		return 0;
err:
		clear();
		return -1;
	}
	
	/**
	 * �����������������ϲ����ϲ���__other��Ϊ��
	**/
	int merge(self & __other)
	{
		return merge(__other, std::less<_Tp>());
	}

	/**
	 * �����������������ϲ����ϲ���__other��Ϊ��
	**/
	template <typename _Predicator>
	int merge(self & __other, _Predicator __p)
	{
		if(_root.next != _root && __other._root.next != __other._root)
		{
            size_type other_size = __other.size();
			_node_allocator.merge(__other._node_allocator);
			_root.prev->next = 0;
			__other._root.prev->next = 0;
			_root.next = _merge(
                    (node_pointer)(_root.next), 
                    (node_pointer)(__other._root.next), 
                    __p
                    );
			__other._root.prev = __other._root.next = __other._root;
			_relink();
            _size += other_size;
            __other._size = 0;
			return 0;
		}
		else if(__other._root.next != __other._root)
		{
			return swap(__other);
		}
		else
		{
			return 0;
		}
	}
#define __BSL_LIST_STACK_DEPTH 64
	/**
	 * ������˳������
	**/
	int sort()
	{
		return sort(std::less<_Tp>());
	}
	/**
	 * ���ݷº������صĽ������������
	**/
	template <typename Predicate>	
	int sort(Predicate _p = std::less<_Tp>())
	{
		if( _root.next == _root) {
			return 0;
        }
		node_pointer cur = _root.next;
		node_pointer counter[__BSL_LIST_STACK_DEPTH ];
		for(int i=0; i < __BSL_LIST_STACK_DEPTH ; i++)
		{
			counter[i] = 0;
		}
	//	int fill=0;
		while( cur != _root)
		{
			node_pointer tmp = cur;
			cur = cur->next;
			tmp->next = 0;
			int i = 0;
			while(counter[i] != 0)
			{
				tmp = _merge(tmp,counter[i], _p);
				counter[i]=0;
				i++;
			}
			counter[i] = tmp;
	//		if(i > fill)
	//			fill = i;
		}
		for(int i = 1; i < __BSL_LIST_STACK_DEPTH ; i++)
		{
			counter[i] = _merge(counter[i-1], counter[i], _p);
		}
		_root.next = counter[__BSL_LIST_STACK_DEPTH - 1];
		_relink();
		return 0;
	}
private:

    void _reset(){
	    _root.next = _root.prev = _root;
        _size = 0;
    }

	void _relink()
	{
		node_pointer t_begin = _root;
		do
		{
			t_begin->next->prev = t_begin;
			t_begin = t_begin->next;
		}
		while(t_begin->next != 0);
		t_begin->next = _root;
		_root.prev = t_begin;
	}
	template <typename _Predicate>
	inline node_pointer _merge(node_pointer list1, node_pointer list2, const _Predicate &_p)
	{
		if(0 == list2) {
			return list1;
        }
		if(0 == list1) {
			return list2;
        }
		volatile node_holder_t tmp_root;
		node_pointer tp = (node_pointer)&tmp_root;
		node_pointer a = list1;
		node_pointer b = list2;
		while(true)
		{
			if(_p(a->val,b->val))
			{
				tp->next = a;
				if(a->next != 0)
				{
					tp = a;
					a = a->next;
				}
				else
				{
					a->next = b;
					return tmp_root.next;
				}

			}
			else
			{
				tp->next = b;
				if(b->next != 0)
				{
					tp = b;
					b = b->next;
				}
				else
				{
					b->next = a;
					return tmp_root.next;
				}
			}
		}
	}
	/**
	 * �������ṹ�����ԣ���ǰ��ָ���Ƿ�һ�£��Ƿ���ڻ�, ����DEBUG
	**/
	void check()
	{
		if(_root.next == _root.prev) {
			return;
        }
		node_pointer tmp = _root.next;
		while(tmp != _root)
		{
			if(tmp->prev == 0)
			{
				*(int*)0=1;
			}
			if(tmp->next->prev != tmp)
			{
				*(int *)0=1;
			}
			tmp->prev = 0;
			tmp = tmp->next;
		}
		tmp = _root.next;
		tmp->prev = _root;
		while(tmp != _root)
		{
			tmp->next->prev = tmp;
			tmp = tmp->next;
		}
	}
};

}


#endif

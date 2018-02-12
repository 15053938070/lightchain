#ifndef _BSL_SLIST__
#define _BSL_SLIST__

#include <Lsc/alloc/Lsc_alloc.h>
#include <Lsc/alloc/Lsc_sample_alloc.h>
#include <Lsc/archive/Lsc_serialization.h>
#include <Lsc/exception/Lsc_exception.h>

#if 0
template <typename _Tp>
class alloc
{
plclic:
	typedef _Tp* pointer;	//�ռ��ַָ��
	typedef _Tp* const const_pointer;	//���ռ��ַָ��
	typedef _Tp& reference;	//���ռ��ַָ��
	_Tp * allocate(int n)
	{
		return new _Tp[n];
	}
	void deallocate(_Tp * _x )
	{
		delete [] _x;
	}	

};
#endif
namespace Lsc
{

/**
 */
template <typename _Tp, typename _Alloc>
struct slist_node_t
{
	typedef _Tp value_type;
	typedef slist_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
	node_pointer next;
	value_type val;
};

template <class _Tp, class _Alloc > class slist;

/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class slist_const_iterator
{
private:
    typedef slist<_Tp, _Alloc> slist_t;
plclic:
	//typedef std::forward_iterator_tag iterator_category;
	typedef typename slist_t::value_type value_type;
	typedef typename slist_t::iterator iterator;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
	typedef slist_const_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef slist_node_t<value_type, _Alloc> node_t;
    typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
    friend class slist<value_type, _Alloc>;
plclic:
	slist_const_iterator()
	{
		_node = 0;
	}
	slist_const_iterator(const self& __x)
	{
		_node = __x._node;
	}
	slist_const_iterator(const iterator & __x)
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
private:
	slist_const_iterator(node_pointer __x)
	{
		_node = __x;
	}
plclic:
	node_pointer _node;
};

/**
 */
template<class _Tp, class _Ptr, class _Ref, class _Alloc>
class slist_iterator
{
private:
    typedef slist<_Tp, _Alloc> slist_t;
plclic:
    //typedef std::forward_iterator_tag iterator_category;
	typedef typename slist_t::value_type value_type;
	typedef typename slist_t::iterator iterator;
	typedef typename slist_t::const_iterator const_iterator;
	typedef _Ptr pointer;
	typedef _Ref reference;
private:
    typedef slist_iterator<_Tp, _Ptr, _Ref, _Alloc> self;
	typedef slist_node_t<value_type, _Alloc> node_t;
   	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
	typedef typename node_alloc_t::pointer node_pointer;
    friend class slist<value_type, _Alloc>;
plclic:
    slist_iterator()
	{    
        _slist = 0;
		_node = 0;
	}
	slist_iterator(const self& __x)
	{
        _slist = __x._slist;
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
private:
    slist_iterator(slist_t* __l, node_pointer __x)
	{
        _slist = __l;
		_node = __x;
	}   
plclic:
    node_pointer _node;
private:
    slist_t *_slist; //������slist
};

/**
 */
template <class _Tp, class _Alloc = Lsc_sample_alloc<Lsc_alloc<_Tp>, 256> >
class slist
{
private:
	typedef slist<_Tp,_Alloc> self;
	typedef slist_node_t<_Tp, _Alloc> node_t;
	typedef typename _Alloc::template rebind<node_t>::other node_alloc_t;
plclic:
	typedef _Tp value_type;
	typedef _Tp *  pointer;
	typedef const _Tp * const_pointer;
	typedef _Tp & reference; 
	typedef const _Tp & const_reference; 
	typedef size_t size_type;
	typedef long difference_type;
	typedef slist_iterator<value_type, pointer, reference, _Alloc> iterator;
	typedef slist_const_iterator<value_type, const_pointer, 
                                    const_reference, _Alloc> const_iterator;
private:
	typedef typename node_alloc_t::pointer node_pointer;	
	//typedef typename node_alloc_t::const_pointer node_const_pointer;	
	//typedef typename node_alloc_t::reference node_reference; 
	//typedef typename node_alloc_t::const_reference node_const_reference; 
	//friend class slist_iterator<_Tp, _Alloc>;
	node_pointer _header;
	node_alloc_t _node_allocator;
    size_type _size;

plclic:
	/**
     * ���쳣
     * �����create
	**/
	slist()
	{
        _reset();
        create();
	}

    /**
	 * �������캯�������쳣
     * �������ʧ�ܣ����ָ���ԭ���Ŀ�slist
     * �����create
	**/
	slist( const self& other )
	{
		_reset();
        create();
        if (assign(other.begin(),other.end()) != 0) {
            throw BadAllocException()<<BSL_EARG<<"assign slist error";
        }
	}

	/**
	 * ��ֵ���������쳣
     * �������ʧ�ܣ����ָ���ԭslist��ǿ�쳣��ȫ��֤
     * ���ɾ��ʧ�ܣ���ĩβɾ����㣬���������Դй©�������쳣��ȫ��֤
	**/
	self& operator = (const self& other)
	{
        if (this != &other) {
            iterator first1         = begin();
            iterator last1          = end();
            const_iterator first2   = other.begin();
            const_iterator last2    = other.end();
            size_t size1            = size();
            size_t size2            = other.size();
            if (size2 > size1) {
                for ( size_t i = 1; i < size1; ++ i ) {
                    ++ first1;
                    ++ first2;
                }
                // this��Ϊ�գ���first2Ϊ��Ҫ����ĵ�һ�����
                if (size1) {
                    ++ first2;
                }
                iterator tmp = first1;
                while ( first2 != last2 ) {
                    if (insert_after( first1, *first2) != 0) {
                        erase_after( tmp, end() );
                        throw BadAllocException()<<BSL_EARG<<"insert_after slist error";
                    }
                    if ( 0 == first1._node ) { // ��Ҫ�ж�ͷ����Ƿ�Ϊ��
                        first1 = begin();
                    } else {
                        ++ first1;
                    }
                    ++ first2;
                }
                first1 = begin();
                first2 = other.begin();
            } else { // size1 >= size2
                for ( size_t i = size1; i > size2; -- i ) {
                    if ( pop_front() != 0 ) {
                        throw BadAllocException()<<BSL_EARG
                            <<"pop_front slist error size["<<size1<<"~"<<size2<<"]";
                    }
                }
            }
            while ( first1 != last1 && first2 != last2 ) {
                *first1 ++ = *first2 ++;
            }
        }
		return *this; 
	}
	/**
	 * Ϊ����������ڴ�
	**/
	int create()
	{
		_node_allocator.create();
        return 0;
	}
	/**
	 * �ͷ�������ռ�õ��ڴ�
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
	~slist()
	{
		destroy();
	}
	/**
	 * ����һ���������Ƹ���
	**/
	int assign (const self & __x)
	{
		if (&__x != this) {
			return assign(__x.begin(), __x.end());
        } else {
			return 0;
		}
	}

	/**
	 * �ӵ��������Ƹ���
	**/
	template <typename __iterator>	
	int assign(__iterator __begin, __iterator __end)
	{
		node_pointer tmp_header;
		node_pointer cur = reinterpret_cast<node_pointer>(&tmp_header);
		__iterator tp = __begin;
        size_type n = 0;
		while(tp != __end)
		{
			node_pointer tmp = _node_allocator.allocate(1);
			if (!tmp) {
				goto err1;
            }
			Lsc::Lsc_construct(&tmp->val, *tp);
			cur->next = tmp;
			cur = tmp;
			++tp;
            ++n;
		}
		cur->next = 0;
		clear();
		_header = tmp_header;
        _size = n;
		return 0;
	err1:
		if(0 == tmp_header->next) {
			return -1;
        } else {
			tmp_header = tmp_header->next;
        }
		do {
			node_pointer tmp = tmp_header->next;
			Lsc::Lsc_destruct(&(tmp_header->val));
			_node_allocator.deallocate(tmp_header, 1);
			tmp_header = tmp;
		} while(cur != tmp_header);
		return -1;
	}

	/**
	 * �������Ԫ��
	**/
	int clear()
	{
		while(_header)
		{
			pop_front();
		}
        _size = 0;
		return 0;
	}

	/**
	 * ��������Ƿ�Ϊ��
	**/
	inline bool empty() const
	{
		return _header == 0;
	}

	/**
	 * ����Ԫ�ؼ��뵽����ͷ
	**/
	int push_front(const value_type &__x)
	{
      	node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp) {
			goto err;
        }
		Lsc::Lsc_construct(&tmp->val, __x);
		tmp->next = _header;
		_header = tmp;
        ++_size;
		return 0;
	err:
		return -1;
	}

	/**
	 * ɾ�������ͷԪ��
	**/
	int pop_front()
	{
		if ( 0 == _header) {
			return -1;
        }

		node_pointer tmp = _header->next;
		Lsc::Lsc_destruct(&(_header->val));
		_node_allocator.deallocate(_header, 1);
		_header = tmp;
        --_size;
		return 0;
	}

	/**
	 * ��Ԫ�ز��뵽__iָ���λ��֮�����������ָ��սڵ㣬����뵽ͷ�ڵ㡣
	**/
	int insert_after( iterator __i,  const value_type &__x)
	{
		if( 0 == __i._node && _header != 0) {
			return -1;
        }

		node_pointer tmp = _node_allocator.allocate(1);
		if (!tmp) {
			goto err;
        }
		Lsc::Lsc_construct(&tmp->val, __x);
		if(__i._node)
		{
			tmp->next = __i._node->next;
			__i._node->next = tmp;
		}
		else
		{
			tmp->next = _header;
			_header = tmp;
		}
        __i._slist->_size += 1;
		return 0;
	err:
		return -1;
	}
	/**
	 * ɾ��__iָ��Ľڵ����һ���ڵ�
	**/
	int erase_after ( iterator __i)
	{
		if( 0 == __i._node) {
			return -1;
        }
		node_pointer tmp = __i._node->next;
		__i._node->next = tmp->next;
		Lsc::Lsc_destruct(&(tmp->val));
		_node_allocator.deallocate(tmp, 1);
        __i._slist->_size -= 1;
		return 0;
	}
	/**
	 * ɾ��__s��__e֮��Ľڵ㣬����Ϊ(__s,__e)
	**/
	int erase_after ( iterator __s, iterator __e)
	{
		if(__s._node == 0) {
			return -1;
        }
		node_pointer s = __s._node->next;
		node_pointer e = __e._node;
        size_t n = 0;
		while(s != e)
		{
			node_pointer tmp = s;
			s = s->next;
			Lsc::Lsc_destruct(&(tmp->val));
			_node_allocator.deallocate(tmp, 1);
            ++n;
		}
		__s._node->next = __e._node;
        __s._slist->_size -= n;
		return 0;
	}
	/**
	 * ����ͷԪ�صĳ�����
	**/
	inline const_reference front() const
	{
		if( 0 == _header ) {
			throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no front item in empty list.";
        }
		return _header->val;
	}
	
	/**
	 * ����ͷԪ�ص�����
	**/
	inline reference front()
	{
		if( 0 == _header ) {
			throw Lsc::InvalidOperationException()<<BSL_EARG
                <<"no front item in empty list.";
        }
		return _header->val;
	}

	/**
	 * ����ָ��ͷ�ڵ�ĳ�������
	**/
	inline const_iterator begin() const
	{
		return const_iterator(_header);
	}

	/**
	 * ����ָ��ͷ�ڵ�ĵ�����
	**/
	inline iterator begin()
	{
		return iterator(this, _header);
	}

	/**
	 * ����ָ��β�ڵ�ĳ�������
	**/
	inline const_iterator end() const
	{
		return const_iterator(0);
	}

	/**
	 * ����ָ��β�ڵ�ĵ�����
	**/
	inline iterator end()
	{
		return iterator(this, 0);
	}
	/**
	 * ������������
	**/
	int reverse()
	{
		node_pointer prev = 0;
		node_pointer cur = _header;
		while(cur != 0)
		{
			node_pointer tmp = cur->next;
			cur->next = prev;
			prev = cur;
			cur = tmp;
		}
		_header = prev;
		return 0;
	}
	
	/**
	 * ����ָ����һ���ڵ�ĵ����������Ӷ�O(n)
	**/
	iterator previous(iterator pos)
	{
		if(0 == pos._node) {
			return iterator(this, 0);
        }
		node_pointer tp = _header;
		if(tp == pos._node) {
			return iterator(this, 0);
        }
		while(tp != 0)
		{
			if(tp->next == pos._node) {
				return iterator(this, tp);
            } else {
				tp = tp->next;
            }
		}
		return iterator(this, 0);
	}
	
	/**
	 * ����ָ����һ���ڵ�ĵ����������Ӷ�O(n)
	**/
	const_iterator previous(const_iterator pos) const
	{
		if(0 == pos._node) {
			return const_iterator(0);
        }
		node_pointer tp = _header;
		while(tp->next != pos._node)
		{
			tp = tp->next;
		}
		return const_iterator(tp);
	}
	/**
	 * ���������Ľڵ�����
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
	/**
	 * ��__x������������
	**/
	int swap( self & __x)
	{
		//if(__x
		_node_allocator.swap(__x._node_allocator);
		std::swap(_header, __x._header);
        std::swap(_size, __x._size);
		//check();
		//__x.check();
		return 0;
	}
	/**
	 * ���л�
	**/
	template <typename _Archive>
	int serialization(_Archive &ar)
	{
		size_type l = size();
		if(Lsc::serialization(ar, l)) {
			goto err;
        }
		for(iterator i=begin(); i!=end(); i++)
		{
			if(Lsc::serialization(ar, *i)) {
				goto err;
            }
		}
		return 0;
	err:
		return -1;
	}

	/**
	 * �����л�
	**/
	template <typename _Archive>
	int deserialization(_Archive &ar)
	{
		node_pointer cur;
		size_type l;
		clear();
		if(Lsc::deserialization(ar, l)) {
			goto err;
        }
		cur = reinterpret_cast<node_pointer>(&_header);
		for(size_type i=0; i<l; i++)
		{
			value_type tmp;
			if(Lsc::deserialization(ar, tmp)) {
				goto err;
            }
			if(insert_after(iterator(this, cur), tmp)) {
				goto err2;
            }
			cur = cur->next;
		}
		return 0;
	err:
	err2:
		clear();
		return -1;

	}
#define __BSL_LIST_STACK_DEPTH 64
	/**
	 * ������˳������
		tmp->val = __x;
	**/
	inline int sort()
	{
		return sort(std::less<_Tp>());
	}
	/**
	 * ��Predicateָ����˳������
	**/
	template <typename Predicate>	
	int sort(Predicate _p)
	{
		if( 0 == _header) {
			return 0;
        }
		node_pointer cur = _header;
		node_pointer counter[__BSL_LIST_STACK_DEPTH];
		for(int i=0; i<__BSL_LIST_STACK_DEPTH ; i++)
		{
			counter[i] = 0;
		}
	//	int fill=0;
		while( cur != 0)
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
		_header = counter[__BSL_LIST_STACK_DEPTH - 1];
		return 0;
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
		if(_header != 0 && __other._header != 0)
		{
            size_type other_size = __other.size();
			_node_allocator.merge(__other._node_allocator);
			_header = _merge(_header, __other._header, __p);
			__other._header = 0;
            _size += other_size;
            __other._size = 0;
			return 0;
		}
		else if(__other._header != 0)
		{
			return swap(__other);
		}
		else
		{
			return 0;
		}
	}
	/**
	 * ��������ɾ����__xֵ��ͬ��ʵ��
	**/
	int remove(const value_type & __x)
	{
		return remove_if(std::bind2nd(std::equal_to<_Tp>(), __x));
	}
	/**
	 * ������������ɾ��Ԫ��
	**/
	template<class Predicate> 
	int remove_if(Predicate __p) 
	{
		//Ϊ�˱����ͷ�ڵ����⴦������ǿ�ƽ�_header��������Ľڵ㡣
		node_pointer cur = reinterpret_cast<node_pointer>(&_header);
		while(cur->next != 0)
		{
			if(__p(cur->next->val))
			{
				node_pointer tmp = cur->next;
				cur->next = tmp->next;
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
private:

    void _reset(){
        _header = 0;
        _size = 0;
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
		volatile node_pointer tmp_root;
		//Ϊ�˱����ͷ�ڵ����⴦������ǿ�ƽ�_header��������Ľڵ㡣
		node_pointer tp = (node_pointer)(&tmp_root);
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
					return tmp_root;
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
					return tmp_root;
				}
			}
		}
	}
};

}

#endif

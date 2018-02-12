

#ifndef  __BSL_DEQUE_H_
#define  __BSL_DEQUE_H_

#include <Lsc/alloc/Lsc_alloc.h>
#include <Lsc/utils/Lsc_utils.h>
#include <Lsc/exception/Lsc_exception.h>

namespace Lsc {

	/**
	*   
	*/
	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize = 1024UL>
	struct _deque_iterator {
		typedef _deque_iterator<_Tp, _Tp&, _Tp*, _BuffSize>				iterator;
		typedef _deque_iterator<_Tp, const _Tp&, const _Tp*, _BuffSize> const_iterator;

		typedef std::random_access_iterator_tag iterator_category;
		typedef _Tp                        value_type;
		typedef _Ptr                       pointer;
		typedef _Ref                       reference;
		typedef size_t                     size_type;
		typedef ptrdiff_t                  difference_type;
		typedef _Tp**                      _map_pointer;
		typedef _deque_iterator		   _self; 

		value_type* _cur;
		value_type* _first;
		value_type* _last;
		_map_pointer _node;

		static const size_type _BUFF_SIZE = sizeof(value_type)<_BuffSize?
			size_type(_BuffSize/sizeof(value_type)):size_type(1);
		
		_deque_iterator(_Tp* __x, _map_pointer __y)
			: _cur(__x), _first(NULL == __y ? NULL : *__y),
			  _last(NULL == __y ? NULL : *__y + _BUFF_SIZE),
			  _node(__y) {}

		/**
		**/
		_deque_iterator() : _cur(NULL), _first(NULL), _last(NULL), _node(NULL) {}

		/**
		 *
		**/
		_deque_iterator(const iterator& __x)
			: _cur(__x._cur),
			  _first(__x._first),
			  _last(__x._last),
			  _node(__x._node) {}

		reference operator*() const 
		{
			if(NULL == _cur){
				throw Lsc::NullPointerException()<<BSL_EARG<<"can not evaluate a null iterator.";
			}
			return *_cur; 
		}

		pointer operator->() const 
		{
			return _cur;
		}

		_self& operator++() 
		{
			++_cur;
			if (_cur == _last) {
				++_node;
				_first = *_node;
				_last = _first + difference_type(_BUFF_SIZE);
				_cur = _first;
			}
			return *this;
		}

		_self operator++(int) 
		{
			_self tmp = *this;
			++*this;
			return tmp;
		}

		_self operator--() 
		{
			if (_cur == _first) {
				--_node;
				_first = *_node;
				_last = _first + difference_type(_BUFF_SIZE);
				_cur = _last;
			}
			--_cur;
			return *this;
		}

		_self operator--(int) 
		{
			_self tmp = *this;
			--*this;
			return tmp;
		} 

		_self& operator+=(difference_type __n) 
		{
			const difference_type offset = __n + (_cur - _first);
			if (offset >= 0 && offset < difference_type(_BUFF_SIZE)) {
				_cur += __n;
			} else {
				const difference_type node_offset =
					offset > 0 ? offset / difference_type(_BUFF_SIZE)
					: -difference_type((-offset - 1)
							/ _BUFF_SIZE) - 1;
				_node += node_offset;
				_first = *_node;
				_last = _first + difference_type(_BUFF_SIZE);
				_cur = _first + (offset - node_offset
						* difference_type(_BUFF_SIZE));
			}
    		return *this;
      	}

      	_self operator+(difference_type __n) const 
		{
    		_self tmp = *this;
    		return tmp += __n;
      	}

      	_self& operator-=(difference_type __n) 
		{ 
			return *this += -__n; 
		}

      	_self operator-(difference_type __n) const 
		{
    		_self tmp = *this;
    		return tmp -= __n;
      	}

		reference operator[](difference_type __n) const 
		{
			return *(*this + __n); 
		}
	};

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool operator==(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return __x._cur == __y._cur; 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
			inline bool operator==(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
			{ 
				return __x._cur == __y._cur; 
			}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool operator!=(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return !(__x == __y); 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
			inline bool operator!=(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{ 
			return !(__x == __y);
		}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool operator<(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return (__x._node == __y._node) ? (__x._cur < __y._cur)
			: (__x._node < __y._node); 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
		inline bool operator<(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{ 
			return (__x._node == __y._node) ? (__x._cur < __y._cur)
			: (__x._node < __y._node); 
		}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool operator>(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return __y < __x; 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
		inline bool operator>(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{ 
			return __y < __x; 
		}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool
		operator<=(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return !(__y < __x); 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
		inline bool operator<=(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{ 
			return !(__y < __x); 
		}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline bool operator>=(const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x,
				const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __y)
		{ 
			return !(__x < __y); 
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
		inline bool operator>=(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{ 
			return !(__x < __y); 
			
		}

	template<typename _Tp, typename _RefL, typename _PtrL,
		typename _RefR, typename _PtrR, size_t _BuffSize>
		inline typename _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>::difference_type
		operator-(const _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>& __x,
					const _deque_iterator<_Tp, _RefR, _PtrR, _BuffSize>& __y)
		{
			return typename _deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>::difference_type
				(_deque_iterator<_Tp, _RefL, _PtrL, _BuffSize>::_BUFF_SIZE)
				* (__x._node - __y._node - 1) + (__x._cur - __x._first)
				+ (__y._last - __y._cur);
		}

	template<typename _Tp, typename _Ref, typename _Ptr, size_t _BuffSize>
		inline _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>
		operator+(ptrdiff_t __n, const _deque_iterator<_Tp, _Ref, _Ptr, _BuffSize>& __x)
		{ 
			return __x + __n; 
		}



	/**
	*   
	*   ˫ͷ���У� ͨ��һ������������ݿ���в���
	*   ����ͨ��_BuffSizeָ��ÿ������ֽڴ�С��ÿ�����Ԫ�ش�Сͨ�� _BuffSize��Ԫ�صĴ�Сȡ���õ���
	*  
	*/
	template<typename _Tp, size_t _BuffSize = 1024UL, class _InnerAlloc = Lsc_alloc<_Tp> >
	class deque
	{
		plclic:
			typedef typename _InnerAlloc::pool_type::template rebind<_Tp>::other _base;

			typedef typename _base::value_type        value_type;
			typedef typename _base::pointer           pointer;
			typedef typename _base::const_pointer     const_pointer;
			typedef typename _base::reference         reference;
		    typedef typename _base::const_reference   const_reference;
			typedef typename _base::size_type         size_type;
			typedef typename _base::difference_type   difference_type;

			typedef _deque_iterator<value_type,
					reference,
					pointer,
					_BuffSize> iterator; 

			typedef _deque_iterator<value_type,
					const_reference,
					const_pointer,
					_BuffSize> const_iterator;

			typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
			typedef std::reverse_iterator<iterator>         reverse_iterator;

		private:
			typedef typename _InnerAlloc::pool_type::template rebind<value_type*>::other 
																			_base_pointer;
			_base _node_alloc;				  /**< �ڴ�������       */
			_base_pointer _map_alloc;		  /**< �����ڴ� ������       */
			/**< ÿ�����Ԫ�ظ���  */
			static const size_type _BUFF_SIZE = sizeof(_Tp) < _BuffSize?
				size_type(_BuffSize / sizeof(_Tp)) : size_type(1);		 
			/**< ������Ĵ�С��ʼΪ _INITIAL_MAP_SIZE       */
			enum { _INITIAL_MAP_SIZE = 8UL };		  
			/**
			*  
			*/
			struct impl_t {
				pointer cur;
				pointer first;		  /**< ��ͷ��       */
				pointer last;		  /**< ��β��       */
				pointer* map;		  /**< ָ����ָ�� */
			};

			impl_t _start;			/**< ��ͷ       */
			impl_t _finish;			/**< ��β       */

			pointer* _map_first;		/**< ������ͷ       */
			pointer* _map_last;			/**< ������β      */
			size_type _map_size;		/**< �������С      */

		plclic:

			/* *
			 * ���쳣
			 * �����create
			 * */
			deque(){
				_reset();
			}

			/**
			 *
			 * ʧ�����쳣
			 * �����create
			 *
			**/
			explicit deque(size_type __size) {
				_reset();
				if (create() != 0) {
					throw BadAllocException()<<BSL_EARG
						<<"create error when create deque with size "<<__size<<".";
				}
			}

			/**
			 * ʧ�����쳣���ָ���ԭ���Ŀ�deque
			 **/
			deque(const deque& other) {
				_reset();
				if (create() != 0) {
					throw BadAllocException()<<BSL_EARG
						<<"create deque error in copy constructor.";
				}
				if (assign(other.begin(),other.end()) != 0) {
					throw BadAllocException()<<BSL_EARG
						<<"assign deque error in copy constructor.";
				}
			}
			
			/**
			 *		  �������ʧ�ܣ����ָ���ԭdeque��ǿ�쳣��ȫ��֤
			 *		  ���ɾ��ʧ�ܣ���ĩβɾ����㣬���������Դй©�������쳣��ȫ��֤
			 *
			 **/
			deque& operator=(const deque& other) {
				if (this != &other) {
					iterator first1 = begin();
					iterator last1 = end();
					const_iterator first2 = other.begin();
					const_iterator last2 = other.end();
					size_t size1 = size();
					size_t size2 = other.size();
					if (size1 < size2) {
						for (size_t i = size1; i < size2; i ++) {
							if ( push_back( other[i] ) != 0 ) {
								for (size_t j = size1; j < i; j ++) {
									pop_back();
								}
								throw BadAllocException()<<BSL_EARG<<"push_back deque error index["
									<<i<<"] size["<<size1<<"~"<<size2<<"]";
							}
						}
					} else { // size1 >= size2
						for (size_t i = size1; i > size2; -- i) {
							if ( pop_back() != 0 ) {
								throw BadAllocException()<<BSL_EARG<<"pop_back deque error index["
									<<i<<"] size["<<size1<<"~"<<size2<<"]";
							}
						}
					}
					while (first1 != last1 && first2 != last2) {
						*first1 ++ = *first2 ++;
					}
				}
				return *this;
			}
			
			/**
			 *
			**/
			~deque()
			{
				destroy();
			}
			
			/**
			 *
			**/
			int create(size_type __size = size_type(_INITIAL_MAP_SIZE))
			{
				//��destroy��Ŀ���Ǳ���2��������create��������
				destroy();
				size_type init_size = std::max(__size, size_type(1));
				//��ʼ���ڴ��
				if (_node_alloc.create() != 0 || _map_alloc.create() != 0) {
					goto fail;
				}
				//������Ҫ��ʼ����������С
				init_size = (init_size - 1) / _BUFF_SIZE + 1;
				//��ʼ������
				if 	(_initialize_map(std::max(init_size, size_type(_INITIAL_MAP_SIZE))) != 0) {
					goto fail;
				}
				return 0;	
			fail:
				_node_alloc.destroy();
				_map_alloc.destroy();
				return -1;	
			}

			/**
			 *
			**/
			int assign(size_type __n, const value_type& __val)
			{
				clear();
				for (size_type i = 0; i < __n; ++i) {
					if (push_back(__val) != 0) {
						goto fail;
					}
				}
				return 0;
			fail:
				clear();
				return -1;
			}

			/**
			 *
			**/
			template<typename _InputIterator>
			int assign(_InputIterator __first, _InputIterator __last)
			{
				typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
				return _assign(__first, __last, _Integral());
			}

			template<typename _Integer>
			int _assign(_Integer __n, _Integer __val, __true_type)
			{
				return assign(size_type(__n), value_type(__val));
			}

			template<typename _InputIterator>
			int _assign(_InputIterator __first, _InputIterator __last, __false_type)
			{
				clear();
				for (_InputIterator iter = __first; iter != __last; ++iter) {
					if (push_back(*iter) != 0) {
						goto fail;
					}
				}
				return 0;
			fail:
				clear();
				return -1;
			}

			/**
			 *
			**/
			int destroy()
			{
				//���deque
				clear();
				//���������ڴ��
				if (NULL != _start.map) {
					_node_alloc.deallocate(_start.map[0], _BUFF_SIZE);
					_start.map = NULL;
				}
				//��������
				if (_map_first != NULL) {
					_map_alloc.deallocate(_map_first, _map_size);
					_map_first = NULL;
				}
				//��������
				_reset();
				//�����ڴ��
				_node_alloc.destroy();
				_map_alloc.destroy();
				return 0;
			}

			/**
			 *
			**/
			int clear()
			{
				while (pop_back() == 0) {
				}
				return 0;
			}

			iterator begin()
			{
				return iterator(_start.cur, _start.map);
			}

			iterator end()
			{
				return iterator(_finish.cur, _finish.map);
			}
			
			const_iterator begin() const
			{
				return const_iterator(_start.cur, _start.map);
			}

			const_iterator end() const
			{
				return const_iterator(_finish.cur, _finish.map);
			}

			reverse_iterator rbegin()
			{ 
				return reverse_iterator(end()); 
			}

			const_reverse_iterator rbegin() const
			{ 
				return const_reverse_iterator(end()); 
			}

			reverse_iterator rend() 
			{ 
				return reverse_iterator(begin()); 
			}

			const_reverse_iterator rend() const
			{ 
				return const_reverse_iterator(begin()); 
			}

			size_type max_size() const
			{ 
				return size_type(-1); 
			}

	
			/**
			 *
			**/
			int push_back(const value_type& __x)
			{
				//���δ�������򴴽�֮
				if(0 == _map_size){
					if(create() != 0){
						return -1;
					}
				}
				if (_finish.cur) {
					pointer cur = _finish.cur++;
					if (_finish.cur == _finish.last) {
						if (_finish.map == _map_last) {
							if (_new_map() != 0) {
								//ʧ�ָܻ��ֳ�
								_finish.cur = cur;
								return -1;
							}
						}
						//�����µ��ڴ��
						pointer new_node = _node_alloc.allocate(_BUFF_SIZE);
						if (NULL == new_node) {
							//ʧ�ָܻ��ֳ�
							_finish.cur = cur;
							return -1;
						}
						(*(++_finish.map)) = new_node;
						_finish.first = new_node; 
						_finish.last = new_node + _BUFF_SIZE;
						_finish.cur = new_node;
					}
					Lsc::Lsc_construct(cur, __x);
					return 0;
				}
				return -1;
			}

			/**
			 *
			**/
			int push_front(const value_type& __x)
			{
				//���δ�������򴴽�֮
				if(0 == _map_size){
					if(create() != 0){
						return -1;
					}
				}
				if (_start.cur) {
					if (_start.cur != _start.first) {
						--_start.cur;
						Lsc::Lsc_construct(_start.cur, __x);
						return 0;
					}
					if (_start.map == _map_first) {
						if (_new_map() != 0) {
							return -1;
						}
					}
					pointer new_node = _node_alloc.allocate(_BUFF_SIZE);
					if (NULL == new_node) {
						return -1;
					}
					*(--_start.map) = new_node;
					_start.first = new_node;
					_start.last =  new_node + _BUFF_SIZE;
					_start.cur = _start.last - 1;
					Lsc::Lsc_construct(_start.cur, __x);
					return 0;
				}
				return -1;
			}
			
			/**
			 *
			**/
			int pop_back()
			{
				if (_start.cur != _finish.cur) {
					if (_finish.cur == _finish.first) {
						_node_alloc.deallocate(_finish.first, _BUFF_SIZE);
						_finish.first = *(--_finish.map);
						_finish.last = _finish.first + _BUFF_SIZE;
						_finish.cur = _finish.last;
					}
					--_finish.cur;
					Lsc::Lsc_destruct(_finish.cur);
					return 0;
				} 
				return -1;
			}

			/**
			 *
			**/
			int pop_front()
			{
				if (_start.cur != _finish.cur) {
					Lsc::Lsc_destruct(_start.cur);
					++_start.cur;
					if (_start.cur == _start.last) {
						_node_alloc.deallocate(_start.first, _BUFF_SIZE);
						_start.first = *(++_start.map);
						_start.last = _start.first + _BUFF_SIZE;
						_start.cur = _start.first;
					}
					return 0;
				} 
				return -1;
			}

			/**
			 *
			 * ���Ϊ����Ϊ�գ����׳��쳣��
			**/
			reference front()
			{
				//����Ƿ�Ϊ��
				if(_start.cur == _finish.cur){
					throw Lsc::InvalidOperationException()<<BSL_EARG
						<<"deque front item does not exist.";
				}
				return *_start.cur;
			}

			/**
			 *
			 * �������Ϊ�գ����׳��쳣
			**/
			reference back() 
			{
				//����Ƿ�Ϊ��
				if(_start.cur == _finish.cur){
					throw Lsc::InvalidOperationException()<<BSL_EARG
						<<"deque back item does not exist.";
				}
				if (_finish.cur != _finish.first) {
					return *(_finish.cur - 1);
				}else{
					return *(*(_finish.map - 1) + _BUFF_SIZE - 1);
				}
			}

			/**
			 *
			 * �������Ϊ�գ����׳��쳣
			**/
			const_reference front() const
			{
				//����Ƿ�Ϊ��
				if(_start.cur == _finish.cur){
					throw Lsc::InvalidOperationException()<<BSL_EARG
						<<"deque front item does not exist.";
				}
				return *_start.cur;
			}

			/**
			 *
			 * �������Ϊ�գ����׳��쳣
			**/
			const_reference back() const 
			{
				//����Ƿ�Ϊ��
				if(_start.cur == _finish.cur){
					throw Lsc::InvalidOperationException()<<BSL_EARG
						<<"deque back item does not exist.";
				}

				if (_finish.cur != _finish.first) {
					return *(_finish.cur - 1);
				}else{
					return *(*(_finish.map - 1) + _BUFF_SIZE - 1);
				}
			}

			/**
			 *
			**/
			reference operator[](size_type __n) const 
			{
				__n += _start.cur - _start.first;
				return _start.map[__n/_BUFF_SIZE][__n%_BUFF_SIZE];
			}

			/**
			 *
			**/
			int get(const size_type &__n, value_type *__val)
			{
				if (__n >= size() || NULL == __val) {
					return -1;
				}
				Lsc::Lsc_construct(__val, (*this)[__n]);
				return 0;
			}

			/**
			 *
			**/
			int set(const size_type &__n, const value_type &__val)
			{
				if (__n >= size()) {
					return -1;
				}
				Lsc::Lsc_construct(&((*this)[__n]), __val);
				return 0;
			}

			/**
			 *
			**/
			size_type size() const 
			{
				if (_start.map == _finish.map) {
					return _finish.cur - _start.cur;
				} 
				return (_start.last - _start.cur) +
					(_finish.cur - _finish.first) +
					(_finish.map - _start.map - 1) * _BUFF_SIZE;
			}

			/**
			 *
			**/
			int resize(size_type __new_size, const value_type& __x)
			{
				size_type len = size();
				if (__new_size < size()) {
					for (size_type i = __new_size; i < len; ++i) {
						if (pop_back() < 0) {
							goto fail;
						}
					}
				} else {
					for (size_type i = len; i < __new_size; ++i) {
						if (push_back(__x) < 0) {
							goto fail;
						}
					}
				}
				return 0;
			fail:
				return -1;
				
			}

			/**
			 *
			**/
			int resize(size_type __new_size)
			{
				return resize(__new_size, value_type());
			}

			/**
			 *
			**/
			bool empty() const
			{
				return (_start.cur == _finish.cur);
			}


			/**
			 *
			**/
			void swap(deque& __x)
			{
				std::swap(this->_start, __x._start);
				std::swap(this->_finish, __x._finish);
				std::swap(this->_map_first, __x._map_first);
				std::swap(this->_map_last, __x._map_last);
				std::swap(this->_map_size, __x._map_size);
				std::swap(this->_map_alloc, __x._map_alloc);
				std::swap(this->_node_alloc, __x._node_alloc);
			}


			/**
			 *
			 *
			**/
			template <typename _Archive>
			int serialization(_Archive &__ar) 
			{    
				if (Lsc::serialization(__ar, size()) != 0) {
					return -1;
				}
				for (iterator iter = begin(); iter != end(); ++iter) {    
					if (Lsc::serialization(__ar, *iter) != 0) { 
						return -1;
					}
				}    
				return 0;
			}

			/**
			 *
			 *
			**/
			template <typename _Archive>
			int deserialization(_Archive &ar)
			{
				size_type len = size();
				if (Lsc::deserialization(ar, len) != 0) {
					return -1;
				}
				clear();
				value_type tmp;
				while (len--) {
					if (Lsc::deserialization(ar, tmp) != 0) {
						goto fail;
					}
					if (push_back(tmp) != 0) {
						goto fail;
					}
				}

				return 0;
			fail:
				clear();
				return -1;
			}

		private:	
			/**
			 *
			**/
			void _reset()
			{
				_map_first = NULL;
				_map_last = NULL;
				_start.cur = NULL;
				_start.first = NULL;
				_start.last = NULL;
				_start.map = NULL;
				_finish = _start;
				_map_size = 0;
			}

			/**
			 *
			**/
			int _initialize_map(size_type __initialize_size)
			{
				//�����ڴ������
				_map_first = _map_alloc.allocate(__initialize_size);
				if (NULL == _map_first) {
					goto fail;
				}
				_map_size = __initialize_size;
				_map_last = _map_first + _map_size - 1;
				_start.map = _map_first + (_map_size>>1);
				//�����һ���ڴ��
				_start.first = _node_alloc.allocate(_BUFF_SIZE);
				if (NULL == _start.first) {
					goto fail;
				}
				//ͷβ�ڴ��ĳ�ʼ��
				(*_start.map) = _start.first;
				_start.last = _start.first + _BUFF_SIZE;
				_start.cur = _start.first;
				_finish = _start;
				return 0;
			fail:
				if (_map_first != NULL) {
					_map_alloc.deallocate(_map_first, __initialize_size);
					_map_first = NULL;
				}
				_reset();
				return -1;	
			}
	
			/**
			 *
			 *  
			**/
			int _new_map()
			{
				//�����������������ʵ��ʹ�õĿռ�û��ռ��һ�룬ֻ�����ƶ�
				//��������µ��ڴ�顣
				//����������Խ�ʡһ�����ڴ档
				const difference_type offset = _finish.map - _start.map + 1;
				pointer* new_start = NULL;
				if ((difference_type)_map_size > 2 * offset) {
					new_start = _map_first + ((_map_size - offset) >> 1);
					//�ж�����ǰ���������
					//���ﲻ��memmov������Ϊ���ǲ��ԵĽ��forѭ��������Ҫ����һЩ
					
					if (new_start < _start.map) {
						for (pointer* p = new_start; p < new_start + offset; ++p) {
							*p = *_start.map;
							++_start.map;
						}
					} else {
						for (pointer* p = new_start + offset - 1; p >= new_start; --p) {
							*p = *_finish.map;
							--_finish.map;
						}
					}
				} else {
					//�����µ��ڴ��
					pointer* new_map = _map_alloc.allocate(_map_size << 1);
					if (NULL == new_map) {
						return -1;
					}
					//�����ɵ�����
					new_start = new_map + (_map_size - (offset >> 1));
					memcpy(new_start, _start.map, offset * sizeof(pointer));
					_map_alloc.deallocate(_map_first, _map_size);
					_map_size <<= 1;
					//ָ����л�
					_map_first = new_map;
					_map_last = new_map + _map_size - 1;
				}
				_start.map = new_start;
				_finish.map = new_start + offset - 1;
				return 0;
			}
						
	};

	/**
	 *  ����������ͬ���͵�deque
	 *
	**/
	template<typename _Tp, size_t _BuffSize, typename _InnerAlloc>
	inline void swap(deque<_Tp, _BuffSize, _InnerAlloc>& __x,
			deque<_Tp, _BuffSize, _InnerAlloc>& __y)
	{ 
		__x.swap(__y); 
	}


} //namespace Lsc

#endif  //__BSL_DEQUE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

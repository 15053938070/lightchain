#ifndef  __BSL_STRING_H__
#define  __BSL_STRING_H__
#include <cstdarg>
#include <cstring>
#include <string>   //for char_traits
#include <cstdlib>
#include <iostream>
#if __GNUC__ >= 4 // 4.3 or greater
#include <backward/hash_fun.h>
#elif __GNUC__ >= 3
#include <ext/hash_fun.h>
#else
#include <stl_hash_fun.h>
#endif
#include "Lsc/exception/Lsc_exception.h"
#include "Lsc/pool/Lsc_poolalloc.h"
#include "Lsc/archive/Lsc_serialization.h" 
#include "Lsc/utils/Lsc_memcpy.h"
#include "Lsc/containers/hash/Lsc_hash_fun.h"


namespace Lsc{
    template<class, class> class basic_string;
    typedef basic_string<char, pool_allocator<char> > string;
    /**
     *  
     *  ����һ���Ƚϼ򵥵��ַ����ࡣΪ���std::string��g++2.96�µĶ��߳�bug����ƣ��ӿھ�����std::string����һ�¡�����ʹ����������㷨��Ŀǰû��ʹ�����ü��������Ժ���ܸĽ���
     *  ��ʼ��ʱ_str��ָ֤��һ��ֵΪ0�ľ�̬�ռ䣨û��malloc�Ŀ����������_str����һ���Ϸ����ַ�������������basic_string��û��trivial default constructor�ˡ�
     *  ��һ������㷨�ǹ���ʱ_str����ʼ��ΪNULL���Ӷ�ʹstringӵ��trivial default constructor�����ںܶ�ĺ��������ﶼ��Ҫ���_str�Ƿ�ָ��NULL�����ǵ�����string���󱻹���󶼻ᱻ��ε��ã����Ч���Ͽ��ܵò���ʧ
     *  
     *  ע�⣡���಻����NULL��Ϊ�Ϸ���C����ַ���(��std::string��Ϊ��ͬ)������const_pointer(NULL)������һ���׳�Lsc::NullPointerException
     *
     * ĿǰLsc::string��std::string�ӿڵ���Ҫ��ͬ�㣺
     *   ģ�������
     *     Lsc::string��std::string����һ��ģ�������_TraitsT��ԭ��Ϊ��
     *         �ò�������ʹ�ã�
     *         �ò������ܼ���g++2.96��g++3.4.5�������ϲ��������������Զ���_TraitsT�����Զ���_Alloc���������������룻
     *         ʹ������������һ�㡣
     *
     *   C++����ַ������Ӵ�������
     *     std::string�Բ��Ϸ����Ӵ���������begin_pos���󣩻��׳�std::out_of_range�쳣����Lsc::string����������
     *
     *   ��C������չAPI
     *     ���ǵ��ٶ��ڴ󲿷ֳ���Աϰ��C����API��Lsc::string����������չ��
     *     Lsc::string& setf( const char * format, ... );
     *     Lsc::string& vsetf( const char * format, va_list );
     *     Lsc::string& appendf( const char * format, ... );
     *     Lsc::string& vappendf( const char * format, va_list );
     *   ��ֱ���ڣ÷���ַ����ϲ�����ȣ�����API���Զ���չstring�ĳ��ȣ��������з���Խ������
     *
     *   ������,insert����, replace��������������������
     *     Lsc::stringĿǰ���ṩ��Щ�������պ󽫻��𲽼���
     *
     *  
     *
     */
    template<class _CharT, class _Alloc>
        class basic_string{
        plclic:
            typedef _CharT              value_type;
            typedef size_t              size_type;
            typedef long                difference_type;
            typedef value_type &        reference;
            typedef const value_type&   const_reference;
            typedef value_type *        pointer;
            typedef const value_type *  const_pointer;
            typedef _Alloc              allocator_type;

            typedef pointer             iterator;
            typedef const_pointer       const_iterator;

#if __GNUC__ <= 2
            typedef string_char_traits<_CharT> traits_type;
#else
            typedef std::char_traits<_CharT> traits_type;
#endif

            //ctors and dtors

            /**
             *
            **/
            basic_string() 
                : _dataplus(_s_create_cstring(0,allocator_type())), _length(0), _capacity(0){ }

            /**
             *
             * ������������õ�����ҪΪ�˼���std::string�Ľӿ�
            **/
            explicit basic_string( const allocator_type& alloc_ )
                : _dataplus(_s_create_cstring(0, alloc_), alloc_ ), _length(0), _capacity(0){}

            /**
             * ��other.length() == 0ʱ��û�ж�̬�ڴ���䣨���ǵ�STL��������������ʹ��һ��Ĭ�Ϲ���Ķ���ȥ���ƹ���һ��Ѷ����������Ǻ�������ģ�
             * 
             **/
            basic_string( const basic_string& other ) 
                : _dataplus(_s_clone_cstring(other._dataplus._str, other._length, other._dataplus), other._dataplus), 
                _length(other._length), _capacity(other._length) { }

            /**
             *
             * ��slc_str_lenδ���������npos
             * ��*this������Ϊother�ϵ�[begin_pos, other.length())�����ϵ��Ӵ�
             * ����*this������Ϊother�ϵ�[begin_pos, begin_pos+slc_str_len)���Ӵ�������begin_pos��(0-based)�ַ���ͷ���������Ϊslc_str_len���Ӵ�
             * 
             *
            **/
            basic_string( const basic_string& other, size_type begin_pos, size_type slc_str_len = npos, const allocator_type& alloc_ = allocator_type() ) 
                : _dataplus(NULL, alloc_), _length(0), _capacity(0){
                    if ( begin_pos > other._length ){
                        begin_pos = other._length;
                    }
                    if ( slc_str_len > other._length - begin_pos ){
                        slc_str_len = other._length - begin_pos;
                    }
                    _length = _capacity = slc_str_len;
                    _dataplus._str = _s_clone_cstring( other._dataplus._str + begin_pos, _capacity, alloc_ );
                }

            /**
             *
             * ������ֱ�Ӹ�������[cstr, cstr+len)֮������ݣ��������Ƿ����\0
             * 
            **/
            basic_string( const_pointer cstr, size_type len, const allocator_type& alloc_= allocator_type() ) 
                : _dataplus(NULL,alloc_), _length(0), _capacity(0) {
                    _length = _capacity = len;
                    _dataplus._str      = _s_clone_cstring( cstr, _length, alloc_ );
                }

            /**
             *
             * ��cstrΪ�մ���NULL��������Ϊ�մ�������û�ж�̬�ڴ���䡣
            **/
            basic_string( const_pointer cstr, const allocator_type& alloc_ = allocator_type() ) 
                : _dataplus(NULL,alloc_), _length(0), _capacity(0) {
                    _capacity = _length = _s_cstring_len(cstr);
                    _dataplus._str = _s_clone_cstring(cstr, _length, alloc_);
                }

            /**
             *
            **/
            basic_string( size_type n, value_type chr, const allocator_type& alloc_ = allocator_type() )
                : _dataplus(_s_create_cstring(n, alloc_), alloc_), _length(n), _capacity(n){
                    std::fill_n(_dataplus._str,n,chr);    //���ж����ػ��汾���Ż�����
                    _dataplus._str[_length] = 0;
                }

            /**
             *
             * ע��������������Ƿ���'\0'ֵ������std::string��Ϊ��ͬ��
            **/
            template<class InputIterator>
                basic_string( InputIterator __begin, InputIterator __end, const allocator_type& alloc_= allocator_type() )
                :_dataplus(NULL,alloc_), _length(0), _capacity(0) {
                    difference_type dis = std::distance(__begin, __end);
                    if ( dis > 0 ){
                        pointer tmp = _dataplus._str = _s_create_cstring( dis, alloc_ );
                        _length = _capacity = size_type(dis);
                        for(; __begin != __end; ++ __begin ){
                            *tmp++ = *__begin;
                        }
                        *tmp = 0;
                    }else{
                        _dataplus._str = _s_create_cstring(0, alloc_);
                    }
                }

            /**
             *
            **/
            basic_string( const_pointer __begin, const_pointer __end, const allocator_type& alloc_= allocator_type() )
                :_dataplus(NULL, alloc_), _length(0), _capacity(0){
                    if ( __begin == NULL || __end == NULL ){
                        throw NullPointerException()<<BSL_EARG<<"__begin["<<static_cast<const void*>(__begin)<<"] > __end["<<static_cast<const void*>(__end)<<"]";
                    }
                    if ( __begin <= __end ){
                        _length = _capacity = __end - __begin;
                        _dataplus._str = _s_clone_cstring( __begin, _length, alloc_ );
                    }else{
                        _dataplus._str = _s_create_cstring(0, alloc_);
                    }
                }

            /**
             *
            **/
            basic_string( pointer __begin, pointer __end, const allocator_type& alloc_= allocator_type() )
                :_dataplus(NULL, alloc_), _length(0), _capacity(0){
                    if ( __begin == NULL || __end == NULL ){
                        throw NullPointerException()<<BSL_EARG<<"__begin["<<static_cast<const void*>(__begin)<<"] > __end["<<static_cast<const void*>(__end)<<"]";
                    }
                    if ( __begin <= __end ){
                        _length = _capacity = __end - __begin;
                        _dataplus._str = _s_clone_cstring( __begin, _length, alloc_ );
                    }else{
                        _dataplus._str = _s_create_cstring(0, alloc_);
                    }
                }

            /**
             *
            **/
            ~basic_string() {
                _s_destroy_cstring(_dataplus._str, _capacity, _dataplus);
            }

            //slcstr
            basic_string slcstr(size_t pos = 0, size_t n = npos ) const {
                return basic_string(*this, pos, n);
            }

            //serialization
            
            /**
             *
             * ����������汾�Ĵ��л�����
            **/
            template <class _Archive>
                int serialization(_Archive &ar) const {
                    if (ar.push(&_length, sizeof(_length)) != 0) {
                        __BSL_ERROR("push siz[%lu] err", _length);
                        return -1;
                    }
                    if ( _length && ar.push(_dataplus._str, _length*sizeof(value_type)) != 0) {
                        __BSL_ERROR("push str[%p] err", _dataplus._str);
                        return -1;
                    }
                    return 0;
                }

            /**
             *
             * �������л�ʧ��ʱ���ú�������-1��ǿ�쳣��ȫ��֤
            **/
            template <class _Archive>
                int deserialization(_Archive &ar) {
                    size_type siz = 0;
                    if (ar.pop(&siz, sizeof(siz)) != 0) {
                        __BSL_ERROR("pop siz err");
                        return -1;
                    }
                    pointer ptr = _s_create_cstring(siz, _dataplus);   //throw
                    if (ar.pop(ptr, sizeof(value_type)*siz) != 0 ) {
                        __BSL_ERROR("pop str err");
                        _s_destroy_cstring(ptr, siz, _dataplus);
                        return -1;
                    }

                    ptr[siz] = 0;
                    _s_destroy_cstring( _dataplus._str, _length, _dataplus );
                    _dataplus._str = ptr;
                    _length = _capacity = siz;

                    return 0;
                }

            // getters
            /**
             *
            **/
            const_pointer c_str() const  {
                return _dataplus._str; 
            }

            /**
             *
            **/
            bool empty() const {
                return _length == 0;
            }

            /**
             *
            **/
            size_type size() const {
                return _length;
            }

            /**
             *
            **/
            size_type length() const {
                return _length;
            }

            /**
             *
             * �����Ķ���Ϊ���ڲ����½����ڴ���������£����ַ�����������ɵ��ַ�����������'\0'��
             **/
            size_type capacity() const {
                return _capacity;
            }

            /**
             *
             * ��0 <= index < (int)length()
             * �򷵻�˳����index���ַ���
             * ��-(int)length() < index < 0
             * �򷵻ص�����index���ַ���
             * ����
             * ���δ���塣
            **/
            const_reference operator []( int idx ) const {
                if (idx >= 0 && idx < int(_length)){
                    return _dataplus._str[idx];
                }
                if (idx < 0 && idx >= -int(_length) ){
                    return _dataplus._str[int(_length)+idx];
                }
                throw OutOfBoundException()<<BSL_EARG<<"index["<<idx<<"] bound["<<-int(_length)<<"~"<<int(_length)-1<<"]";
            }
            /**
             *
             * ��0 <= index < (int)length()
             * �򷵻�˳����index���ַ���
             * ��-(int)length() < index < 0
             * �򷵻ص�����index���ַ���
             * ����
             * ���δ���塣
            **/
            reference operator []( int idx ) {
                if (idx >= 0 && idx < int(_length)){
                    return _dataplus._str[idx];
                }
                if (idx < 0 && idx >= -int(_length) ){
                    return _dataplus._str[int(_length)+idx];
                }
                throw OutOfBoundException()<<BSL_EARG<<"index["<<idx<<"] bound["<<-int(_length)<<"~"<<int(_length)-1<<"]";
            }

            
            size_type find(const _CharT* s, size_type pos, size_type n) const{
                if ( s == NULL ){
                    throw Lsc::NullPointerException()<<BSL_EARG<<"s can't be NULL!";
                }
                const_pointer tb = _dataplus._str + pos;
                const_pointer te = _dataplus._str + _length - n;
                for(; tb <= te; ++tb ){
                    if ( traits_type::compare( tb, s, n ) == 0 ){
                        return tb - _dataplus._str;
                    }
                }
                return npos;
            }

            size_type find(const basic_string& str, size_type pos = 0) const { 
                return this->find(str._dataplus._str, pos, str._length); 
            }

            size_type find(const _CharT* s, size_type pos = 0) const {
                return this->find(s, pos, _s_cstring_len(s));
            }

            size_type find(_CharT c, size_type pos = 0) const{
                const_pointer b = _dataplus._str + pos;
                const_pointer e = _dataplus._str + _length;
                for(; b < e; ++b){
                    if ( *b == c ){
                        return b - _dataplus._str;
                    }
                }
                return npos;
            }
            

            size_type rfind(const basic_string& str, size_type pos = npos) const { 
                return this->rfind(str._dataplus._str, pos, str._length); 
            }

            size_type rfind(const _CharT* s, size_type pos, size_type n) const{
                if ( s == NULL ){
                    throw Lsc::NullPointerException()<<BSL_EARG<<"s can't be NULL!";
                }
                if ( _length >= n ){
                    const_pointer tb = _dataplus._str + std::min(size_t(_length - n), pos);
                    for(; tb >= _dataplus._str; --tb ){
                        if ( traits_type::compare( tb, s, n ) == 0 ){
                            return tb - _dataplus._str;
                        }
                    }
                }
                return npos;
            }

            size_type rfind(const _CharT* s, size_type pos = npos) const {
                return this->rfind(s, pos, _s_cstring_len(s));
            }

            size_type rfind(_CharT c, size_type pos = npos) const{
                const_pointer b = _dataplus._str + (pos >= _length ? _length - 1 : pos);
                const_pointer e = _dataplus._str;
                for(; b >= e; --b){
                    if ( *b == c ){
                        return b - _dataplus._str;
                    }
                }
                return npos;
            }

            /**
             *
            **/
            basic_string& append( const basic_string& other ){
                reserve( _length + other._length );
                xmemcpy( _dataplus._str + _length, other._dataplus._str, sizeof(value_type) * other._length );    // 1 for '\0'
                _length += other._length;
                _dataplus._str[_length] = 0;
                return *this;
            }

            /**
             *
             * ��begin_pos >= other._length��*this���ᱻ�޸�
             * ��begin_pos + slc_str_len >= other.length()����slc_str_len������Ϊother.length() - begin_pos
             *
            **/
            basic_string& append( const basic_string& other, size_type begin_pos, size_type slc_str_len = npos ){
                if ( begin_pos >= other._length ){
                    return *this;
                }
                if ( slc_str_len > other._length - begin_pos ){
                    slc_str_len = other._length - begin_pos;
                }
                reserve( _length + slc_str_len );
                xmemcpy( _dataplus._str + _length, other._dataplus._str + begin_pos, sizeof(value_type) * slc_str_len ); 
                _length += slc_str_len;
                _dataplus._str[_length] = 0;
                return *this;
            }

            /**
             *
             * ����Ѿ�ȷ��֪��cstr�ĳ��ȣ�ʹ��append( cstr, len)���Կ�4~8��
             *
            **/
            basic_string& append( const_pointer cstr ){
                size_t cstr_len = _s_cstring_len( cstr );
                reserve( _length + cstr_len );
                xmemcpy( _dataplus._str + _length, cstr, sizeof(value_type) * cstr_len );    
                _length += cstr_len;
                _dataplus._str[_length] = 0;
                return *this;
            }

            /**
             *
             * ������ֱ�Ӹ���׷������[cstr, cstr+len)֮������ݣ��������Ƿ����\0
             *
            **/
            basic_string& append( const_pointer cstr, size_type len ){
                if ( cstr == NULL ){
                    throw Lsc::NullPointerException()<<BSL_EARG<<"cstr";
                }
                reserve( _length + len );
                xmemcpy( _dataplus._str + _length, cstr, sizeof(value_type) * len );    
                _length += len;
                _dataplus._str[_length] = 0;
                return *this;
            }

            basic_string& append( pointer cstr, size_type len ){
                if ( cstr == NULL ){
                    throw Lsc::NullPointerException()<<BSL_EARG<<"cstr";
                }
                reserve( _length + len );
                xmemcpy( _dataplus._str + _length, cstr, sizeof(value_type) * len );    
                _length += len;
                _dataplus._str[_length] = 0;
                return *this;
            }
            /**
             *
             *
            **/
            basic_string& append( size_type n, value_type chr ){
                reserve( _length + n );
                std::fill_n(_dataplus._str + _length, n, chr);    //���ж����ػ��汾���Ż�����
                _length += n;
                _dataplus._str[_length] = 0;
                return *this;
            }

            /**
             *
            **/
            template<class InputIterator>
                basic_string& append( InputIterator __begin, InputIterator __end ){ 
                    difference_type dis = std::distance(__begin, __end);
                    if ( dis > 0 ){
                        reserve( _length + dis );
                        for( pointer p = _dataplus._str + _length; __begin != __end; ++p, ++ __begin ){
                            *p = *__begin;
                        }
                        _length += size_type(dis);
                        _dataplus._str[_length] = 0;
                    }else{
                        //pass
                    }
                    return *this;
                }

            /**
             *
            **/
            basic_string& append( const_pointer __begin, const_pointer __end ){
                if ( __begin == NULL || __end == NULL ){
                    throw NullPointerException()<<BSL_EARG<<"__begin["<<static_cast<const void*>(__begin)<<"] > __end["<<static_cast<const void*>(__end)<<"]";
                }
                if ( __begin < __end ){
                    return append( __begin, __end - __begin );
                }else{
                    //pass
                    return *this;
                }
            }

            /**
             *
            **/
            basic_string& append( pointer __begin, pointer __end ){
                if ( __begin == NULL || __end == NULL ){
                    throw NullPointerException()<<BSL_EARG<<"__begin["<<static_cast<const void*>(__begin)<<"] > __end["<<static_cast<const void*>(__end)<<"]";
                }
                if ( __begin < __end ){
                    return append( __begin, __end - __begin );
                }else{
                    //pass
                    return *this;
                }
            }

            void push_back( value_type chr ){
                if ( _length >= _capacity ){
                    reserve( _length + 1 );
                }
                _dataplus._str[_length] = chr;
                _dataplus._str[++_length] = 0;
            }

            // setters
            /**
             *
             * ע�������ַ���ʹ�õĶ�̬������ڴ棬���ڴ治�ᱻ�ͷš���ȷʵ��Ҫ�ͷ��ڴ棬��ʹ��swap���÷���big_str.swap(string());
             *
            **/
            void clear(){
                _length = 0;
                _dataplus._str[0] = 0;
            }

            /**
             *
             * �����Ķ���μ�capacity()
             * ��__capacity > capacity() ���ڴ潫�����·����Ա�֤capacity()>=__capacity�������ݱ��ֲ��䣻
             * ���򣬱������޲�����
             *
            **/
            void reserve( size_type __capacity ){
                if ( __capacity > _capacity ){
                    if ( __capacity < _capacity + _capacity ){
                        __capacity = _capacity + _capacity;
                    }
                    pointer tmp = _s_create_cstring( __capacity, _dataplus );   //throw
                    xmemcpy( tmp, _dataplus._str, (_length+1)*sizeof(value_type) );
                    _s_destroy_cstring( _dataplus._str, _capacity, _dataplus );
                    _dataplus._str = tmp;
                    _capacity = __capacity;
                }
            }

            /**
             *
             * ƽ�����ǳ����õ�һ�����������ʹ�á�swap���÷���������ɺܶ����õĹ��ܣ����дǿ�쳣��ȫ���ϲ㺯��������ַ�����trim to size���� 
             *
            **/
            void swap( basic_string& other ) {
                std::swap(_dataplus._str, other._dataplus._str);
                std::swap(_length, other._length);
                std::swap(_capacity, other._capacity);
            }

            /**
             *
             * ��other.length() <= this->capacity�����ڴ治�ᱻ���·���
             *
            **/
            basic_string& operator = ( const basic_string& other ){
                if ( other._dataplus._str[0] == 0 ){
                    _dataplus._str[0] = 0;
                    _length = 0;
                }else if ( &other != this ){
                    if ( other._length > _capacity ){
                        pointer tmp = _s_create_cstring( other._length, _dataplus );    //throw
                        _s_destroy_cstring( _dataplus._str, _capacity, _dataplus );
                        _dataplus._str = tmp;
                        _capacity = other._length;
                    }
                    _length = other._length;
                    xmemcpy( _dataplus._str, other._dataplus._str, (_length+1)*sizeof(value_type) ); 
                }
                return *this;
            }

            /**
             *
             * ��cstr�ĳ��� <= capacity()�����ڴ治�ᱻ���·���
            **/
            basic_string& operator = ( const_pointer cstr ){
                if ( cstr == NULL ){
                    _dataplus._str[0] = 0;
                    _length = 0;
                }else if ( cstr[0] == 0 ){
                    _dataplus._str[0] = 0;
                    _length = 0;
                }else if ( cstr != _dataplus._str){
                    size_type len = _s_cstring_len( cstr );
                    if ( len > _capacity ){
                        pointer tmp = _s_create_cstring( len, _dataplus );    //throw
                        _s_destroy_cstring( _dataplus._str, _capacity, _dataplus );
                        _dataplus._str = tmp;
                    }
                    _length = _capacity = len;
                    xmemcpy( _dataplus._str, cstr, (len+1)*sizeof(value_type) );
                }
                return *this;
            }


            //comparers
            int compare( const basic_string& other ) const {
                const size_type len = _length < other._length ? _length : other._length;
                int ret = traits_type::compare( _dataplus._str, other._dataplus._str, len );
                if ( ret == 0 ){
                    if (_length > other._length) {
                        return 1;
                    } else if (_length == other._length) {
                        return 0;
                    } else {
                        return -1;
                    }
                }else{
                    return ret;
                }
            }

            int compare( const_pointer cstr ) const {
                const size_type cstr_len = _s_cstring_len(cstr);
                const size_type len = _length < cstr_len ? _length : cstr_len;
                int ret = traits_type::compare( _dataplus._str, cstr, len );
                if ( ret == 0 ){
                    if (_length > cstr_len) {
                        return 1;
                    } else if (_length == cstr_len) {
                        return 0;
                    } else {
                        return -1;
                    }
                }else{
                    return ret;
                }
            }
            /**
             *
            **/
            friend inline bool operator == (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1._length == sstr2._length && sstr1.compare( sstr2 ) == 0;
            }

            /**
             *
            **/
            friend inline bool operator == (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) == 0;
            }

            /**
             *
            **/
            friend inline bool operator == (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) == 0;
            }

            /**
             *
            **/
            friend inline bool operator != (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1._length != sstr2._length || sstr1.compare( sstr2 ) != 0;
            }

            /**
             *
            **/
            friend inline bool operator != (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) != 0;
            }

            /**
             *
            **/
            friend inline bool operator != (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) != 0;
            }

            /**
             *
            **/
            friend inline bool operator < (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1.compare( sstr2 ) < 0;
            }

            /**
             *
            **/
            friend inline bool operator < (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) < 0;
            }

            /**
             *
            **/
            friend inline bool operator < (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) > 0;
            }

            /**
             *
            **/
            friend inline bool operator > (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1.compare( sstr2 ) > 0;
            }

            /**
             *
            **/
            friend inline bool operator > (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) > 0;
            }

            /**
             *
            **/
            friend inline bool operator > (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) < 0;
            }

            /**
             *
            **/
            friend inline bool operator <= (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1.compare( sstr2 ) <= 0;
            }

            /**
             *
            **/
            friend inline bool operator <= (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) <= 0;
            }

            /**
             *
            **/
            friend inline bool operator <= (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) >= 0;
            }

            /**
             *
            **/
            friend inline bool operator >= (const basic_string& sstr1, const basic_string& sstr2){
                return sstr1.compare( sstr2 ) >= 0;
            }

            /**
             *
            **/
            friend inline bool operator >= (const basic_string& sstr, const_pointer cstr ){
                return sstr.compare( cstr ) >= 0;
            }

            /**
             *
            **/
            friend inline bool operator >= (const_pointer cstr, const basic_string& sstr ){ 
                return sstr.compare( cstr ) <= 0;
            }

#if __GNUC__ <= 2
            /**
             *
             * ������ʹLsc::string����ʹ��cout��cerr����stringstream�����������
            **/
            friend inline std::ostream& 
                operator << ( std::ostream& os, const basic_string& sstr ){
                //TODO: implementation for value_type = ushort or uint required
                for( const_pointer p = sstr._dataplus._str; *p; ++ p){
                    os << *p;
                }
                return os;
            }
#else
            /**
             *
             * ������ʹLsc::string����ʹ��cout��cerr����stringstream�����������
            **/
            friend inline std::basic_ostream<value_type>& 
                operator << ( std::basic_ostream<value_type>& os, const basic_string& sstr ){
                //TODO: implementation for value_type = ushort or uint required
                for( const_pointer p = sstr._dataplus._str; *p; ++ p){
                    os << *p;
                }
                return os;
            }
#endif
        plclic:
            //�Ǳ�׼��չ
            /**
             *
            **/
            basic_string& setf( const char * format, ... )__attribute__ ((format (printf, 2, 3) ));

            /**
             *
            **/
            basic_string& appendf( const char * format, ... )__attribute__ ((format (printf, 2, 3) ));

            /**
             *
            **/
            /*
            int getf( const char * format, ... ) const {
                va_list ap;
                va_start( ap, format );
                int res = vsscanf( _dataplus._str, format, ap );
                va_end( ap );
                return res;
            }*/

            
            /**
             *
            **/
            /*
            int vgetf( const char * format, va_list ap ) const {
                return vsscanf( _dataplus._str, format, ap );
            }
            */

            /**
             *
            **/
            basic_string& vsetf( const char * format, va_list ap ){
                clear();
                return vappendf( format, ap );
            }

            /**
             *
             * ����_CharT��ʲô��format����const char *
             *
            **/
            basic_string& vappendf( const char * format, va_list ap ){
                _do_vappendf( format, ap, _CharT() );   //�ػ�_CharT == char �Ը�������
                return *this;
            }

        
        private:

            static size_t _s_cstring_len( const_pointer cstr ) {
                if ( !cstr ){
                    throw NullPointerException()<<BSL_EARG<<"cstr";
                }
                const_pointer tmp = cstr;
                while( *tmp ){
                    ++ tmp;
                }
                return tmp - cstr;
            }

            static pointer _s_create_cstring( size_type __capacity, const allocator_type& alloc_ ) {
                if ( __capacity == 0 ){
                    // (Scott) Meyers Singleton
                    static value_type EMPTY_CSTRING[] = { '\0' };
                    return EMPTY_CSTRING;
                }
                pointer res = allocator_type(alloc_).allocate( __capacity + 1 );
                if ( res == NULL ){
                    throw BadAllocException()<<BSL_EARG<<"__capacity["<<__capacity<<"]";
                }
                res[0] = 0;
                return res;
            }

            static pointer _s_clone_cstring( const_pointer cstr, size_type len, const allocator_type& alloc_ ) {
                if ( cstr == NULL ){
                    throw NullPointerException()<<BSL_EARG<<"cstr";
                }else if ( len == 0 ){
                    static value_type EMPTY_CSTRING[] = { '\0' };
                    return EMPTY_CSTRING;
                }
                pointer res = _s_create_cstring( len, alloc_ );
                xmemcpy( res, cstr, len * sizeof(value_type) );//cstr[len] can be non-null 
                res[len] = 0;
                return res;
            }

            static void _s_destroy_cstring( pointer cstr, size_type __capacity, const allocator_type& alloc_ ) {
                if ( __capacity ){
                    allocator_type(alloc_).deallocate( cstr, __capacity + 1 );
                }
            }

            /**
             *
            **/
            void _do_vappendf( const char * format, va_list ap, char /*type tag*/ ){
                va_list aq;
                va_copy( aq, ap );
                int len = vsnprintf( _dataplus._str + _length, _capacity - _length + 1, format, aq ); // 1 for the trailing '\0'
                va_end( aq );
                if ( len > int(_capacity - _length) ){
                    reserve( _length + len ); //�������� _capacity = _length + len 
                    va_copy( aq, ap );
                    vsnprintf( _dataplus._str + _length, _capacity - _length + 1, format, aq );    //����
                    va_end( aq );
                    _length += len;
                }else if ( len >= 0 ){
                    //pass
                    _length += len;
                }else{  //len < 0
                    //pass
                }
            }



            /**
             *
             * _CharType == wchar_tʱҲ����ʹ��vswprintf()��������Ϊvswprintf()��vsnprintf()�ӿڲ�һ����ɵ�
             *
            **/
            template<typename _CharType>
            void _do_vappendf( const char * format, va_list ap, _CharType /*type tag*/ ){
                va_list aq;
                va_copy( aq, ap );
                int append_len = vsnprintf( NULL, 0, format, aq ); //just get the length;
                va_end( aq );
                if ( append_len < 0 ){ //bad format
                    return; //pass
                }
                if ( append_len > int(_capacity - _length) ){
                    reserve( _length + append_len );
                }
                // �ȳ���tmp��ȥ
                typedef typename _Alloc::template rebind<char>::other char_alloc_t;
                basic_string<char, char_alloc_t> tmp;
                tmp.reserve(append_len);
                va_copy( aq, ap );
                tmp.vappendf( format, aq ); //ֻ��дһ��
                va_end( aq );

                //�����ֳ�����
                const char * tmp_str = tmp.c_str();
                for( int i = 0; i< append_len; ++ i ){  
                    _dataplus._str[_length+i]  = tmp_str[i];
                }
                _length += append_len;
                _dataplus._str[_length] = 0;
            }


        plclic:
            static const size_type npos = static_cast<size_type>(-1);

        private:

            // Use empty-base optimization: http://www.cantrip.org/emptyopt.html
            struct _alloc_hider_t: allocator_type{
                _alloc_hider_t( pointer p)
                    :allocator_type(), _str(p) {}

                _alloc_hider_t( pointer p, allocator_type alloc_)
                    :allocator_type(alloc_), _str(p) {}

                pointer  _str;
            };
            _alloc_hider_t _dataplus;           
            size_type  _length;
            size_type  _capacity;
        };

    template<class _CharT, class _Alloc>
        basic_string<_CharT,_Alloc>& basic_string<_CharT,_Alloc>::setf( const char * format, ... ){
            va_list ap;
            va_start( ap, format );
            clear();
            vappendf( format, ap );
            va_end( ap );
            return *this;
        }

    template<class _CharT, class _Alloc>
        basic_string<_CharT,_Alloc>& basic_string<_CharT,_Alloc>::appendf( const char * format, ... ){
            va_list ap;
            va_start( ap, format );
            vappendf( format, ap );
            va_end( ap );
            return *this;
        }

    /**
     *
     * �ݲ�֧������_CharT��_AllocT
     *
    **/
    inline Lsc::AutoBuffer& operator <<( Lsc::AutoBuffer& buf, Lsc::string& str ){
        return buf << str.c_str();
    }
}   // namespace Lsc

#if __GNUC__ >= 3
namespace __gnu_cxx{
#else
namespace std{
#endif
    /**
    *  
    *  Ŀǰֻ֧��char���ͣ���allocator����Ϊ�������͡�
    *  ʹ����const char*��ͬ��hash�㷨��
    */
    template<class allocator_t> 
        struct hash<Lsc::basic_string<char, allocator_t> >{
            size_t operator()(const Lsc::basic_string<char, allocator_t>& bs ) const {
                return __stl_hash_string(bs.c_str());
            }
        };
}   // namespace __gnu_cxx/std

namespace Lsc{
    /**
    *  
    *  Ŀǰֻ֧��char���ͣ���allocator����Ϊ�������͡�
    *  ʹ����const char*��ͬ��hash�㷨��
    */
    template<class allocator_t> 
        struct xhash<Lsc::basic_string<char, allocator_t> >{
            size_t operator()(const Lsc::basic_string<char, allocator_t>& bs ) const {
                return __Lsc_hash_string(bs.c_str());
            }
        };
}   // namespace Lsc

#endif // __BSL_STRING_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */

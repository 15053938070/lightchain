#ifndef  __BSL_AUTO_BUFFER_H_
#define  __BSL_AUTO_BUFFER_H_
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "Lsc/utils/Lsc_memcpy.h"
#include "Lsc/pool/Lsc_pool.h"



namespace Lsc{
    /**
    *  
    *  �����һ���ص��ǲ����׳��쳣�����ڴ治���ʱ�򣬸����ض��ַ�������"���ض�λ"��
    *  AutoBuffer�������й���һƬ���ڱ�ʾ�ַ����Ļ����������ṩ����׷�Ӹ������Ͷ���
    *  ���ڴ治��ʱ���ڴ����������������������ڴ�ʧ�ܣ����������ڴ棬����ʹtruncated()��������true��
    */
    class AutoBuffer{
    plclic:
        /**
         *
         * �ɴ���__capacity����ָ��Ԥ������ڴ�ռ䡣��__capacity==0û�ж�̬�ڴ���䡣
         * ��ʼ���ڴ�أ�Ĭ�ϲ�ʹ���ڴ�أ���ֱ��malloc��free
         *
         * ע�����ڴ����ʧ�ܲ����׳��쳣�������൱����0Ϊ�������졣
         *
        **/
        explicit AutoBuffer( size_t __capacity = DEFAULT_CAPACITY )
            :_size(0), _capacity(__capacity), _truncated(false), _str(NULL), 
             _mempool(NULL) {
            if ( __capacity != 0 ){
                _str = static_cast<char *>(_mempool == NULL ? 
                        malloc(_capacity + 1) : _mempool->malloc(_capacity + 1));
                if ( _str ){
                    _str[0] = '\0';
                }else{
                    _capacity = 0;
                }
            }
        }

        /**
         *
         * �ɴ���__capacity����ָ��Ԥ������ڴ�ռ䡣��__capacity==0û�ж�̬�ڴ���䡣
         * �ɴ���pool����ָ��ʹ�õ��ڴ��
         *
         * ע�����ڴ����ʧ�ܲ����׳��쳣�������൱����0Ϊ�������졣
         * 
        **/
        explicit AutoBuffer( mempool& pool, size_t __capacity = DEFAULT_CAPACITY )
            :_size(0), _capacity(__capacity), _truncated(false), _str(NULL), _mempool(&pool) {
            if ( __capacity != 0 ) {
                _str = static_cast<char *>(_mempool == NULL ?
                        malloc(_capacity + 1) : _mempool->malloc(_capacity + 1));
                if ( _str ){
                    _str[0] = '\0';
                }else{
                    _capacity = 0;
                }                
            }
        }

        /**
         *        ��Ϊ��ͬAutoBufferʹ�õ��ڴ�ػ��в�ͬ
         *        ���ͷ�����_str���ڴ棬Ȼ��_strָ��other._strǳ�����ַ�����
         *        �����_mempoolָ��other._mempool������_mempool���Ǵ���_str����ڴ�
         *        �����other._str��other��Ϊ�յ�AutoBuffer�����Լ���ʹ��
         *
        **/
        AutoBuffer& transfer_from ( AutoBuffer& other ){
            if ( &other != this ){
                if ( _str ){
                    _mempool == NULL ? free( _str) : _mempool->free( _str, _capacity+1 );
                }
                _size       = other._size;      
                _capacity   = other._capacity;
                _truncated  = other._truncated;
                _str        = other._str;
                _mempool    = other._mempool;
                other._size     = 0;
                other._capacity = 0;
                other._truncated= false;
                other._str      = NULL;
            }
            return *this;
        }

        /**
         *
        **/
        void swap( AutoBuffer& other ){
            std::swap( _str,        other._str );
            std::swap( _size,       other._size );
            std::swap( _capacity,   other._capacity );
            std::swap( _truncated,  other._truncated );
            std::swap( _mempool,    other._mempool );
        }

        /**
         *
        **/
        ~AutoBuffer( ){
            if ( _str ){
                _mempool == NULL ? free( _str ) : _mempool->free( _str, _capacity + 1 );
            }
        }

        /**
         *
        **/
        size_t size() const{
            return _size;
        }
        
        /**
         *
        **/
        size_t capacity() const {
            return _capacity;
        }

        /**
         *
        **/
        bool empty() const {
            return _size == 0;
        }

        /**
         *
        **/
        bool full() const {
            return _size == _capacity;
        }

        /**
         *
        **/
        bool truncated() const {
            return _truncated;
        }

        /**
         *
        **/
        const char * c_str() const {
            if ( _str ){
                return _str;
            }else{
                return "";
            }
        }

        /**
         *
        **/
        void clear() {
            if ( _str && _capacity ){
                _str[0] = '\0';
            }
            _size = 0;
        }

        /**
         *
        **/
        bool reserve( size_t __capacity ) {
            if ( __capacity > _capacity  ){
                if ( __capacity < _capacity * 2 ){ 
                    __capacity = _capacity * 2;
                }

                char * _new = static_cast<char*>(_mempool == NULL ?
                        malloc(__capacity + 1) : _mempool->malloc(__capacity + 1));
                if ( !_new ){
                    return false;
                }

                if ( _str ){
                    xmemcpy( _new, _str, _size + 1 );
                    _mempool == NULL ? free( _str) : _mempool->free( _str, _capacity + 1);
                }

                _str        = _new;
                _capacity   = __capacity;
            }
            return true;
        }

        /**
         *
        **/
        AutoBuffer& operator << (const AutoBuffer& other){
            return push( other._str, other._size );
        }

        /**
         *
        **/
        AutoBuffer& operator << (bool b){
            if ( b ){
                return push( TRUE_LITERAL, TRUE_LITERAL_LEN );
            }else{
                return push( FALSE_LITERAL, FALSE_LITERAL_LEN );
            }
        }

        /**
         *
        **/
        AutoBuffer& operator << (char c){
            if ( c == '\0' ){
                return *this;
            }
            if ( _size == _capacity ){    //full
                reserve( _size + 1 );     //may fail, make best effort.
            }
            if ( _size < _capacity ){
                _str[_size]    = c;
                _str[++_size]  = '\0';
                _truncated = false;
            }else{
                _truncated = true;
            }
            return *this;
       }

        /**
         *
        **/
        AutoBuffer& operator << (signed char sc){
            return pushf( "%hhd", sc );
        }

        /**
         *
        **/
        AutoBuffer& operator << (unsigned char uc){
            return pushf( "%hhu", uc );
        }

        /**
         *
        **/
        AutoBuffer& operator << (wchar_t wc){
            if ( wc == 0 ){
                return *this;
            }
#if __GNUC__ <= 2
            wchar_t ws[] = { wc, 0 };
            return pushf( "%ls", ws );
#else
            return pushf( "%lc", wc );
#endif
        }

        /**
         *
        **/
        AutoBuffer& operator << (const wchar_t *ws){
            if ( ws != NULL ){
                pushf( "%ls", ws );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         *          
         **/
        AutoBuffer& operator << (short i) {
            return pushf( "%hd", i );
        }

        /**
         *          
         **/
        AutoBuffer& operator << (unsigned short i) {
            return pushf( "%hu", i );
        }

        /**
         *
        **/
        AutoBuffer& operator << (int i){
            return pushf( "%d", i );
        }

        /**
         *
        **/
        AutoBuffer& operator << (unsigned int i){
            return pushf( "%u", i );
        }

        /**
         *
        **/
        AutoBuffer& operator << (long int i){
            return pushf( "%ld", i );
        }

        /**
         *
        **/
        AutoBuffer& operator << (unsigned long int i){
            return pushf( "%lu", i );
        }

        /**
         *
        **/
        AutoBuffer& operator << (const char* cstr ){
            if ( cstr != NULL ){
                push( cstr, strlen(cstr) );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         *
        **/
        AutoBuffer& operator << (long long ll){
            return pushf( "%lld", ll );
        }

        /**
         *
        **/
        AutoBuffer& operator << (unsigned long long ll){
            return pushf( "%llu", ll );
        }

        /**
         *
        **/
        AutoBuffer& operator << (dolcle n){
            return pushf( "%lg", n );
        }

        /**
         *
        **/
        AutoBuffer& operator << (long dolcle n){
            return pushf( "%Lg", n );
        }

        /**
         *
        **/
        AutoBuffer& operator << (void *p){
            return pushf( "%p", p );
        }

        /**
         *
        **/
        AutoBuffer& push(const AutoBuffer& other){
            return push( other._str, other._size );
        }

        /**
         *
        **/
        AutoBuffer& push(bool b){
            if ( b ){
                return push( TRUE_LITERAL, TRUE_LITERAL_LEN );
            }else{
                return push( FALSE_LITERAL, FALSE_LITERAL_LEN );
            }
        }

        /**
         *
        **/
        AutoBuffer& push(char c){
            if ( c == '\0' ){
                return *this;
            }
            if ( _size == _capacity ){    //full
                reserve( _size + 1 );     //may fail
            }
            if ( _size < _capacity ){
                _str[_size]    = c;
                _str[++_size]  = '\0';
                _truncated = false;
            }else{
                _truncated = true;
            }
            return *this;
        }

        /**
         *
        **/
        AutoBuffer& push(signed char sc){
            return pushf("%hhd", sc);
        }

        /**
         *
        **/
        AutoBuffer& push(unsigned char uc){
            return pushf("%hhu", uc);
        }

        /**
         *
        **/
        AutoBuffer& push(size_t count, char c){
            if ( c != '\0' ){
                if ( count > _capacity - _size ){    //full
                    count = (count <= size_t(-1) - _size) ? count : (size_t(-1) - _size);  //limit the size
                    if( !reserve( _size + count ) ){
                        //reserve fail
                        count = _capacity - _size;
                        _truncated = true;
                    }else{
                        _truncated = false;
                    }
                }
                if ( count ){
                    //str != NULL
                    memset( _str + _size, c, count );
                    _str[ _size+=count ] = '\0';
                }
            }
            return *this;
        }

        /**
         *
        **/
        AutoBuffer& push(wchar_t wc){
            if ( wc == 0 ){
                return *this;
            }
#if __GNUC__ <= 2
            wchar_t ws[] = { wc, 0 };
            return pushf( "%ls", ws );
#else
            return pushf( "%lc", wc );
#endif
        }

        /**
         *
        **/
        AutoBuffer& push(const wchar_t * ws){
            if ( ws != NULL ){
                pushf( "%ls", ws );
            }else{
                _truncated = false;
            }
            return *this;
        }
        
        /**
         *          
         **/
        AutoBuffer& push(short i) {
            return pushf( "%hd", i );
        }

        /**
         *          
         **/
        AutoBuffer& push(unsigned short i) {
            return pushf( "%hu", i );
        }

        /**
         *
        **/
        AutoBuffer& push(int i){
            return pushf( "%d", i );
        }

        /**
         *
        **/
        AutoBuffer& push(unsigned int i){
            return pushf( "%u", i );
        }

        /**
         *
        **/
        AutoBuffer& push(long int i){
            return pushf( "%ld", i );
        }

        /**
         *
        **/
        AutoBuffer& push(unsigned long int i){
            return pushf( "%lu", i );
        }

        /**
         *
        **/
        AutoBuffer& push(const char* cstr ){
            if ( cstr != NULL ){
                push( cstr, strlen(cstr) );
            }else{
                _truncated = false;
            }
            return *this;
        }

        /**
         *
         * �����߱��뱣֤strlen(cstr) >= slc_str_len��������Ϊδ����
         *
        **/
        AutoBuffer& push(const char* cstr, size_t slc_str_len );
        
        /**
         *
        **/
        AutoBuffer& push(long long ll ){
            return pushf( "%lld", ll );
        }

        /**
         *
        **/
        AutoBuffer& push(unsigned long long ll ){
            return pushf( "%llu", ll );
        }

        /**
         *
        **/
        AutoBuffer& push( dolcle n ){
            return pushf( "%lg", n );
        }

        /**
         *
        **/
        AutoBuffer& push( long dolcle n ){
            return pushf( "%Lg", n );
        }

        /**
         *
        **/
        AutoBuffer& push( void *p ){
            return pushf( "%p", p );
        }

        //attrbute can only be put at function declarations until g++ 3
        /**
         *
        **/
        AutoBuffer& pushf( const char *format, ... ) __attribute__ ((format (printf, 2, 3) ));

        /**
         *
        **/
        AutoBuffer& vpushf( const char *format, va_list ap );


    plclic:
        /**
         *          
        **/
        static const int DEFAULT_CAPACITY = 64;
        /**
         *          
        **/
        static const char * const TRUE_LITERAL; 
        /**
         *
        **/
        static const char * const FALSE_LITERAL; 

    private:
        /**
         *  
         **/
        AutoBuffer( const AutoBuffer& other );
        /**
         *
        **/
        AutoBuffer& operator = ( const AutoBuffer& );
        /**
         *
        **/
        size_t  _size;
        /**
         *
        **/
        size_t  _capacity;
        /**
         *
        **/
        bool    _truncated;
        /**
         *
        **/
        char *  _str;
        /**
         *
        **/
        static const size_t TRUE_LITERAL_LEN; 
        /**
         *
        **/
        static const size_t FALSE_LITERAL_LEN; 
        
        /**
         *
        **/
        mempool* _mempool;

    };

}   //namespace Lsc;


#endif  //__BSL_AUTO_BUFFER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

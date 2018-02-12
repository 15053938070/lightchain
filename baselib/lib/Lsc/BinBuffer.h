
#ifndef  __BSL_BIN_BUFFER_H_
#define  __BSL_BIN_BUFFER_H_
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
    *  BinBuffer����ͨ���ֽڶ��뷽ʽ���й���һƬ���ڱ�ʾ�ַ����Ļ����������ṩ����׷�Ӹ������Ͷ���
    *  ���ڴ治��ʱ���ڴ����������������������ڴ�ʧ�ܣ����������ڴ棬
    *  ����ʹtruncated()��������true��ever_truncated()����true��
    */
    class BinBuffer{
    plclic:
        /**
         *
         * �ɴ���capacity����ָ��Ԥ������ڴ�ռ䡣��capacity==0û�ж�̬�ڴ���䡣
         * �ɴ���pack����ָ��Ĭ���ֽڶ���ֵ��packҪ��Ϊ2�������ݲ���<=MAX_PACK
         *                                      ����_pack=DEFAULT_PACK
         * 
         * ע�����ڴ����ʧ�ܲ����׳��쳣�������൱����0Ϊ�������졣
         *
        **/
        explicit BinBuffer( size_t cap = DEFAULT_CAPACITY, size_t pack = DEFAULT_PACK)
            :_size(0), _capacity(cap), _pack(pack), 
            _truncated(false), _ever_truncated(false), _str(NULL), _mempool(NULL) {
            if ( _capacity != 0 ) {
                _str = static_cast<char *>(_mempool == NULL ? 
                        malloc(_capacity) : _mempool->malloc(_capacity));
                if ( _str ){
                    _str[0] = '\0';
                }else{
                    _capacity = 0;
                }
            }
            if(!(pack > 0 && pack <= DEFAULT_MAX_PACK && is_power_two(pack))){
                _pack = DEFAULT_PACK;
            }
        }

        /**
         *
         * �ɴ���capacity����ָ��Ԥ������ڴ�ռ䡣��capacity==0û�ж�̬�ڴ���䡣
         * �ɴ���pool����ָ��ʹ�õ��ڴ��
         * �ɴ���pack����ָ��Ĭ���ֽڶ���ֵ��packҪ��Ϊ2�������ݲ���<=MAX_PACK
         *                                        ����_pack=DEFAULT_PACK
         *
         * ע�����ڴ����ʧ�ܲ����׳��쳣�������൱����0Ϊ�������졣
         * 
        **/
        explicit BinBuffer( 
                mempool& pool, 
                size_t cap = DEFAULT_CAPACITY, 
                size_t pack = DEFAULT_PACK 
                )
            :_size(0), _capacity(cap), _pack(pack), 
            _truncated(false), _ever_truncated(false), _str(NULL), _mempool(&pool) {
            if ( _capacity != 0 ) {
                _str = static_cast<char *>(_mempool == NULL ? 
                        malloc(_capacity) : _mempool->malloc(_capacity));
                if ( _str ){
                    _str[0] = '\0';
                }else{
                    _capacity = 0;
                }
            }
            if(!(pack > 0 && pack <= DEFAULT_MAX_PACK && is_power_two(pack))) {
                _pack = DEFAULT_PACK;
            }
        }

        /**
         *        ��Ϊ��ͬBinBufferʹ�õ��ڴ�ػ��в�ͬ
         *        ���ͷ�����_str���ڴ棬Ȼ��_strָ��other._strǳ�����ַ�����
         *        �����_mempoolָ��other._mempool������_mempool���Ǵ���_str����ڴ�
         *        ������other._str��other��Ϊ�յ�BinBuffer�����Լ���ʹ��
         *
        **/
        BinBuffer& transfer_from ( BinBuffer& other ){
            if ( &other != this ){
                if ( _str ){
                    _mempool == NULL ? free( _str ) : _mempool->free( _str, _capacity);
                }
                _size           = other._size;              
                _capacity       = other._capacity;
                _pack           = other._pack;
                _truncated      = other._truncated; 
                _ever_truncated = other._ever_truncated;
                _str            = other._str; 
                _mempool        = other._mempool;
                other._size     = 0;                         
                other._capacity = 0;                             
                other._pack     = DEFAULT_PACK;                       
                other._truncated= false;                   
                other._ever_truncated = false;                             
                other._str      = NULL;
            }
            return *this;
        }

        /**
         *
        **/
        void swap( BinBuffer& other ){
            std::swap( _str,            other._str );
            std::swap( _size,           other._size );
            std::swap( _capacity,       other._capacity );
            std::swap( _pack,           other._pack );
            std::swap( _truncated,      other._truncated );
            std::swap( _ever_truncated, other._ever_truncated );
            std::swap( _mempool,        other._mempool );
        }

        /**
         *
        **/
        ~BinBuffer(){
            if ( _str ){
                _mempool == NULL ? free( _str ) : _mempool->free( _str, _capacity);
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
        bool ever_truncated() const {
            return _ever_truncated;
        }

        /**
         *
        **/
        const char * data() const {
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
            if ( _size ){
                _str[0] = '\0';
            }
            _size = 0;
        }

        /**
         *
        **/
        bool reserve( size_t cap ) {
            if ( cap > _capacity  ){
                if ( cap < _capacity * 2 ){ 
                    cap = _capacity * 2;
                }

                char * _new = static_cast<char*>(_mempool == NULL ? 
                        malloc(cap) :_mempool->malloc(cap));
                if ( !_new ){
                    return false;
                }

                if ( _str ){
                    xmemcpy( _new, _str, _size );
                    _mempool == NULL ? free( _str ) : _mempool->free( _str, _capacity);
                }

                _str        = _new;
                _capacity   = cap;
            }

            return true;
        }

        /**
         *
        **/
        BinBuffer& operator << (const BinBuffer& other){
            size_t pack = other.get_pack();
            pack = pack < _pack ? pack : _pack;
            _size = (_size + pack - 1) & (~(pack-1));
            return push( other.data(), other.size() );
        }

        /**
         *          bool,signed char,unsigned char,short,unsigned short,int
         *          unsigned int,long int,unsigned long int,long long
         *          unsigned long lont,dolcle,long dolcle
         *          
         *          wchar_t:����������Ŀ��ַ�������'\0'
         *          char:������������ַ�������'\0'
         *
         **/
        template<typename _Tp>
            BinBuffer& operator << ( _Tp value ) {
                return push_bin_data( &value, sizeof(value) );
            }

               /**
         *
        **/
        BinBuffer& push(const BinBuffer& other){
            size_t pack = other.get_pack();
            pack = pack > _pack ? pack : _pack;
            _size = (_size + pack - 1) & (~(pack-1));
            return push( other.data(), other._size );
        }

        /**
         *          bool,signed char,unsigned char,short,unsigned short,int
         *          unsigned int,long int,unsigned long int,long long
         *          unsigned long lont,dolcle,long dolcle
         *
         *          wchar_t:����������Ŀ��ַ�������'\0'
         *          char:������������ַ�������'\0'    
         *
         **/
        template<typename _Tp>
            BinBuffer& push( _Tp value ) {
                return push_bin_data( &value, sizeof(value) );
            }

       /**
         *
        **/
        BinBuffer& push( size_t count, char c){
            if ( count > _capacity - _size ){    //full
                count = (count <= size_t(-1) - _size) ? count : (size_t(-1) - _size); //limit the size
                if( !reserve( _size + count ) ){
                    //reserve fail
                    count = _capacity - _size;
                    _truncated = true;
                    _ever_truncated = true;
                }else{
                    _truncated = false;
                }
            }
            if ( count ){
                //str != NULL
                memset( _str + _size, c, count );
                _size += count;
            }
            return *this;
        }
        
        /**
         *
         * �����߱��뱣֤dataָ������ݳ��Ȳ�����len��������Ϊδ����
         *
        **/
        BinBuffer& push(const void* data_, size_t len );

        /**
         *        ����ֵΪtrue��ʾ���óɹ���false��ʾ����ʧ��
         *          
        **/
        bool set_pack( size_t pack ) {
            if ( pack > 0 && pack <= DEFAULT_MAX_PACK && is_power_two( pack ) ) {
                _pack = pack;
                return true;
            }
            return false;
        }

        /**
         *          
        **/
        size_t get_pack() const {
            return _pack;
        }

    plclic:
        /**
         */
        static const size_t DEFAULT_CAPACITY   = 64;
        /**
         */
        static const size_t DEFAULT_PACK       = 4;
        /**
         */
        static const size_t DEFAULT_MAX_PACK   = 64;
    
    private:
        /**
         */
        BinBuffer( const BinBuffer& other );
        /**
         */
        BinBuffer& operator = ( const BinBuffer& );
        /**
         *          
        **/
        inline bool is_power_two(size_t n) {
            return (((n)&(n-1))==0);
        }
        /**
         *          
        **/
        BinBuffer& push_bin_data( const void* data_, size_t len ) {
            // ����_pack����_size����ʼλ��
            size_t len_ = (len < _pack) ? len : _pack;
            _size = (_size + len_ - 1) & (~(len_ - 1));
            // push����ע������ڴ��޷�����ʱ����_size��С
            return push( data_, len );
        }

        /**
         */
        size_t  _size;
        /**
         */
        size_t  _capacity;
        /**
         */
        size_t  _pack;
        /**
         */
        bool    _truncated;
        /**
         */
        bool    _ever_truncated;
        /**
         */
        char *  _str;
       
        /**
         *
        **/
        mempool* _mempool;

    };

}   //namespace Lsc;


#endif  //__BSL_AUTO_BUFFER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

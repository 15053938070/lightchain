
#ifndef  __SHALLOWCOPYSTRING_H_
#define  __SHALLOWCOPYSTRING_H_
#include <cstring>
#include <iostream>
#include "Lsc/exception/Lsc_exception.h"
#include "Lsc/containers/string/Lsc_string.h"

namespace Lsc{

    class ShallowCopyString{
    plclic:
        typedef char            value_type;
        typedef size_t          size_type;
        typedef long            difference_type;
        typedef value_type &    reference;
        typedef const reference const_reference;
        typedef value_type *    pointer;
        typedef const pointer   const_pointer;
        typedef pointer         iterator;
        typedef const_pointer   const_iterator;
#if __GNUC__ <= 2
        typedef string_char_traits<value_type> traits_type;
#else
        typedef std::char_traits<value_type> traits_type;
#endif
        //ctors and dtors

        /**
         *
         * Ĭ�Ϲ����ShallowCopyString��ʾ�մ���
         **/
        ShallowCopyString() 
            : _str(_S_EMPTY_CS()), _length(0){}

        /**
         *
         * �ú�����C����ַ���ת��ΪShallowCopyString��
         *
         * �÷���ʱ�临�Ӷ�ΪO(1)�����ȱ�������ֵ����ֱ��size()������һ�α����ã�
         *
        **/
        ShallowCopyString( const char * str )  
            : _str(str), _length(npos) {
                if ( NULL == str ){
                    throw NullPointerException()<<BSL_EARG<<"str is NULL";
                }
            }

        /**
         *
         * ע�⣺len������strlen(str)��ȣ�������Ϊδ���塣
         *
        **/
        ShallowCopyString( const char * str, size_t len ) 
            : _str(str), _length(len){
                if ( NULL == str ){
                    throw NullPointerException()<<BSL_EARG;
                }
                if ( str[len] != '\0' ){
                    //�����ĳ���ֵ����
                    throw Lsc::BadArgumentException()<<BSL_EARG<<"wrong len:"<<len;
                }
            }

        /**
         *
        **/
        template<class allocator_t>
        ShallowCopyString( const Lsc::basic_string<char, allocator_t>& Lsc_str )
            : _str(Lsc_str.c_str()), _length(Lsc_str.length()) {};

        /**
         *
        **/
        ShallowCopyString( const Lsc::AutoBuffer& buf )
            : _str(buf.c_str()), _length(buf.size()){}

        /**
         *
        **/
        ShallowCopyString( const ShallowCopyString& sstr ) 
            : _str(sstr._str), _length(sstr._length) {}

        /**
         *
        **/
        ~ShallowCopyString() {
            //do nothing
        }

        void clear(){
            _str = _S_EMPTY_CS();
            _length = 0;
        }
        // getters
        /**
         *
        **/
        const char * c_str() const  {
            return _str;
        }

        /**
         *
         * ���ַ���ʹ��ShallowCopyString(const char *)���첢�ұ�������δ���ù���ʱ�临��Ϊ��O(strlen(this->c_str()))������ΪO(1)
         *
        **/
        size_t size() const {
            if ( _length == npos ){
                const_cast<size_t&>(_length) = strlen(_str);
            }
            return _length;
        }

        /**
         *
        **/
        size_t length() const {
            return size();
        }

        /**
         *
        **/
        size_t capacity() const {
            return size();
        }
        
        /**
         *
         * ע�������������idx�ĺϷ���
        **/
        char operator []( size_t idx ) const {
            return _str[idx];
        }

        /**
         *
         * ���������idx�Ϸ��ԣ���idx>=this->size()���׳�Lsc::OutOfBoundException
        **/
        char at( size_t idx ) const {
            if ( idx < this->size() ){
                return _str[idx];
            }
            throw Lsc::OutOfBoundException()<<BSL_EARG<<"idx:"<<idx<<" size():"<<size();
        }
            
        /**
         *
         * ǳ���ƣ�ʱ�临�Ӷ�ΪO(1)
        **/
        ShallowCopyString& operator = ( const ShallowCopyString& sstr ){
            _str        = sstr._str;
            _length     = sstr._length;
            return *this;
        }

        ShallowCopyString& operator = ( const AutoBuffer& buf ){
            _str        = buf.c_str();
            _length     = buf.size();
            return *this;
        }

        ShallowCopyString& operator = ( const Lsc::string& str ){
            _str        = str.c_str();
            _length     = str.size();
            return *this;
        }

        ShallowCopyString& operator = ( const char *cstr ){
            if ( cstr == NULL ){
                throw Lsc::NullPointerException()<<BSL_EARG<<"cstr is NULL";
            }
            if ( cstr != _str ){
                _str        = cstr;
                _length     = npos;
            }
            return *this;
        }

        /**
         *
         * �����ַ����ֵ���С�ڡ����ڡ�����other���򷵻�ֵ<0��=0��>0
         * �����ַ�������ĸ����ͬ�����ַ�����ַ��ͬ���ú���ʱ�临�Ӷ�ΪO(1)
         * 
         *
        **/
        int compare( const ShallowCopyString& other ) const {
            if ( _str[0] != other._str[0] ){
                return traits_type::lt(_str[0], other._str[0]) ? -1 : 1;
            }
            if ( _str == other._str ){
                return 0;   //����ShallowCopyString��˵�����п��ܵġ�
            }
            size_t min_size = std::min( size() , other.size() );
            int ret = memcmp( _str, other._str, min_size );
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

        //comparers
        //��Ϊconst char *ת��ΪShallowCopyString��O(1)�ģ�ShallowCopyString��һ�������C�ַ����Ƚ�Ҳ��ƽ��O(1)�ģ�Ԥ�жϵ�һ�ֽڣ������Բ�ר�Ŷ�C�ַ������ȽϺ����ˡ�
        friend inline bool operator == (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) == 0;
        }

        friend inline bool operator != (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) != 0;
        }

        friend inline bool operator < (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) < 0;
        }

        friend inline bool operator > (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) > 0;
        }

        friend inline bool operator <= (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) <= 0;
        }

        friend inline bool operator >= (const ShallowCopyString& sstr1, const ShallowCopyString& sstr2){
            return sstr1.compare( sstr2 ) >= 0;
        }

        /**
         *
        **/
        friend inline std::ostream& operator << ( std::ostream& os, const ShallowCopyString& sstr ){
            return os << sstr._str ;
        }
        
        static const size_t npos = static_cast<size_t>(-1);

    private:
        const char *    _str;
        size_t          _length;

        static const char *_S_EMPTY_CS(){
            static const char ret[] = "";
            return ret;
        };
    };
}   // namespace Lsc

/**
 *
**/
inline Lsc::AutoBuffer& operator <<( Lsc::AutoBuffer& buf, Lsc::ShallowCopyString& str ){
    return buf.push(str.c_str(), str.length());
}

#if __GNUC__ >=3
namespace __gnu_cxx{
#else
namespace std{
#endif
    /**
    *  
    *  ʹ����const char*��ͬ��hash�㷨��
    */
    template<>
    struct hash<Lsc::ShallowCopyString>{
        size_t operator()(const Lsc::ShallowCopyString& str ) const {
            return __stl_hash_string(str.c_str());
        }
    };
}   // namespace __gnu_cxx/std

namespace Lsc{
    /**
    *  
    *  ʹ����const char*��ͬ��hash�㷨��
    */
    template<>
    struct xhash<Lsc::ShallowCopyString>{
        size_t operator()(const Lsc::ShallowCopyString& str ) const {
            return __Lsc_hash_string(str.c_str());
        }
    };
}   // namespace Lsc

#endif  //__SHALLOWCOPYSTRING_H_
/* vim: set ts=4 sw=4 sts=4 tw=100 */

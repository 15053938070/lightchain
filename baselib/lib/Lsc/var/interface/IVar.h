#ifndef  __BSL_VAR_IVAR_H__
#define  __BSL_VAR_IVAR_H__

#include "Lsc/containers/string/Lsc_string.h"
#include "Lsc/exception/Lsc_exception.h"
#include "Lsc/ResourcePool.h"
#include "Lsc/var/ArrayIterator.h"
#include "Lsc/var/DictIterator.h"

namespace Lsc{
namespace var{

    /**
    *  
    * �൱��ͷָ���볤�ȵļ򵥼��ϣ�����ʱ�������data��ָ�ڴ档
    */
    struct raw_t{
        /**
        *  
        *  
        */
        const void  *data;
        /**
        *  
        *  
        */
        size_t      length;

        /**
         *
        **/
        raw_t()
            :data(NULL), length(0) {}
        /**
         *
        **/
        raw_t( const void *data_, size_t len)
            :data(data_), length(len) {}
    };

    //forward declaration
    class IVar{
    plclic:
        /**
        *  
        *  
        */
        typedef Lsc::string         string_type;

        /**
        *  
        *  
        */
        typedef Lsc::string         field_type;

        /**
        *  
        *  
        */
        typedef raw_t               raw_type;

        /**
        *  
        *  
        */
        typedef ArrayIterator       array_iterator;

        /**
        *  
        *  
        */
        typedef ArrayConstIterator  array_const_iterator;

        /**
        *  
        *  
        */
        typedef DictIterator        dict_iterator;

        /**
        *  
        *  
        */
        typedef DictConstIterator   dict_const_iterator;

        /**
        *  
        *  
        */
        typedef unsigned short mask_type;

        // constant definition
        static const mask_type IS_BOOL = 1 << 0;          /**< �Ƿ񲼶�����       */
        static const mask_type IS_NUMBER = 1 << 1;        /**< �Ƿ���ֵ����       */
        static const mask_type IS_STRING = 1 << 2;        /**< �Ƿ��ַ�������       */
        static const mask_type IS_RAW = 1 << 3;        /**< �Ƿ����������       */
        static const mask_type IS_ARRAY = 1 << 4;        /**< �Ƿ���������       */
        static const mask_type IS_DICT = 1 << 5;        /**< �Ƿ��ֵ�����       */
        static const mask_type IS_CALLABLE = 1 << 6;        /**< �Ƿ�ɵ�������       */
        static const mask_type IS_OTHER = 1 << 7;        /**< �Ƿ���������       */
        static const mask_type IS_REF = 1 << 8;        /**< �Ƿ���������       */
        static const mask_type IS_MUTABLE = 1 << 9;        /**< �Ƿ�ɱ�����       */
        static const mask_type IS_FLOATING = 1 << 10;        /**< �Ƿ񸡵�����       */
        static const mask_type IS_SIGNED = 1 << 11;        /**< �Ƿ��з�������       */
        static const mask_type IS_ONE_BYTE = 1 << 12;        /**< �Ƿ��ֽ�����       */
        static const mask_type IS_TWO_BYTE = 1 << 13;        /**< �Ƿ�˫�ֽ�����       */
        static const mask_type IS_FOUR_BYTE = 1 << 14;        /**< �Ƿ����ֽ�����       */
        static const mask_type IS_EIGHT_BYTE = 1 << 15;        /**< �Ƿ���ֽ�����       */
        static const mask_type NONE_MASK = 0;        /**< ������ */
        static const mask_type ALL_MASK = ~0;        /**< ������ */



        //methods for all
        /**
         *
        **/
        virtual ~IVar(){ } 

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual IVar& operator = ( IVar& ) = 0;

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         * �÷��������ڵ�������٣�������Ӧ�����ױ�����ʶ�����ʽ���ܾ����仯����Ӧ�������ݽ��м�ء�
         * 
         * ��ѡ��verbose_level������ʾ�ݹ���ȡ�0��ʾ���ݹ���IVar����ʵ����Ӧ��֤�ú����㷨���Ӷ�ΪO(1)��1��ʾ�ݹ�����ֱ����IVar����ʵ����Ӧ��֤�ú����㷨���Ӷ�ΪO(size())�������ơ�
         *
        **/
        virtual string_type dump(size_t verbose_level=0) const = 0;

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual void clear() = 0;

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         * ��BSL 1.0.5�󣬸ú���ֻ����ת��Ϊ�ַ���������/����Ӧʹ��dump()
         *
        **/
        virtual string_type to_string() const = 0;

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual string_type get_type() const = 0; 

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         * Ŀǰ����ֵ����Ŀǰ��unsigned short���Ժ���ܻ�ı䣬���ᱣ����unsigned short����
         *
        **/
        virtual mask_type get_mask() const = 0;

        /**
         *
        **/
        virtual IVar& clone(Lsc::ResourcePool& /*rp*/) const = 0;

        /**
         *
        **/
        virtual IVar& clone(Lsc::ResourcePool& /*rp*/, bool /*is_deep_copy*/) const{
            throw Lsc::NotImplementedException()<<BSL_EARG<<"type["<<typeid(*this).name()
                <<"] this["<<dump(0)<<"]: clone interface has not been implemented.";
        }

        //methods for all, test methods
        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_null() const {
            return (get_mask() & 0xFF) == 0;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_ref() const {
            return get_mask() & IS_REF;
        }

        /**
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_bool() const {
            return get_mask() & IS_BOOL;
        }
        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_number() const {
            return get_mask() & IS_NUMBER;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_int8() const {
            return check_mask( get_mask(), IS_NUMBER|IS_ONE_BYTE|IS_SIGNED, IS_FLOATING );
        }
        
        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_uint8() const {
            return check_mask( get_mask(), IS_NUMBER|IS_ONE_BYTE, IS_SIGNED|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_int16() const {
            return check_mask( get_mask(), IS_NUMBER|IS_TWO_BYTE|IS_SIGNED, IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_uint16() const {
            return check_mask( get_mask(), IS_NUMBER|IS_TWO_BYTE, IS_SIGNED|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_int32() const {
            return check_mask( get_mask(), IS_NUMBER|IS_FOUR_BYTE|IS_SIGNED, IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_uint32() const {
            return check_mask( get_mask(), IS_NUMBER|IS_FOUR_BYTE, IS_SIGNED|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_int64() const {
            return check_mask( get_mask(), IS_NUMBER|IS_EIGHT_BYTE|IS_SIGNED, IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_uint64() const {
            return check_mask( get_mask(), IS_NUMBER|IS_EIGHT_BYTE, IS_SIGNED|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_float() const {
            return check_mask( get_mask(), IS_NUMBER|IS_FOUR_BYTE|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_dolcle() const {
            return check_mask( get_mask(), IS_NUMBER|IS_EIGHT_BYTE|IS_FLOATING );
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_string() const {
            return get_mask() & IS_STRING;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_array() const {
            return get_mask() & IS_ARRAY;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_dict() const {
            return get_mask() & IS_DICT;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
        **/
        virtual bool is_callable() const {
            return get_mask() & IS_CALLABLE;
        }

        /**
         *
         * ����IVarʵ���඼����֧�ָ÷���
         *
        **/
        virtual bool is_raw() const {
            return get_mask() & IS_RAW;
        }

        /**
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual bool to_bool() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }
        //methods for value

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual signed char to_int8() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual unsigned char to_uint8() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual signed short to_int16() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual unsigned short to_uint16() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual signed int to_int32() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual unsigned int to_uint32() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual signed long long to_int64() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual unsigned long long to_uint64() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual float to_float() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual dolcle to_dolcle() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_raw()����true��ʵ���඼����֧�ָ÷���
         *
        **/
        virtual raw_type to_raw() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual IVar& operator = ( bool value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( signed char value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( unsigned char value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }


        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( signed short value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( unsigned short value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }


        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( signed int value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( unsigned int value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }


        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( signed long long value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( unsigned long long value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
        **/
        virtual IVar& operator = ( float value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( dolcle value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( const char * value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�Lsc::UnderflowException / Lsc::OverflowException
         *
        **/
        virtual IVar& operator = ( const string_type& value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual const char * c_str() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual size_t c_str_len() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * �ú��������Ը�д
         *
        **/
        template<typename T>
            T to() const;

        /**
         *
         * ����is_raw()����true��ʵ���඼����ʵ�ָ÷���
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual IVar& operator = ( const raw_type& value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from raw[data:"<<value_.data<<", length:"<<value_.length<<"]";
        }

        //methods for array and dict
        /**
         *
         * is_array()����true����is_dict()����true��ʵ�������֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual size_t size() const{
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        //methods for array
        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���±�Խ�磬����Lsc::var::Null::null;
         * һ��������̽�Ի�ȡ
         *
        **/
        virtual IVar& get( size_t idx ) {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���±�Խ�磬����Ĭ��ֵ
         * һ��������̽�Ի�ȡ������������Ĭ��ֵҪ��
         *
        **/
        virtual IVar& get( size_t idx, IVar& default_value ) {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��index >= size()������Lsc::var::Null::null;
         * һ��������̽�Ի�ȡ
         *
        **/
        virtual const IVar& get( size_t idx ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���±�Խ�磬����Ĭ��ֵ
         * һ��������̽�Ի�ȡ������������Ĭ��ֵҪ��
         *
        **/
        virtual const IVar& get( size_t idx, const IVar& default_value ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��index >= size()����������Զ�������size() + 1
         *
        **/
        virtual void set( size_t idx, IVar& value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] value["<<value_.dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���󶨲����ڣ�����false�����򣬷���true��
         *
        **/
        virtual bool del( size_t idx ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual array_const_iterator array_begin() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual array_iterator array_begin() {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual array_const_iterator array_end() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual array_iterator array_end() {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��index >= size()���׳�Lsc::OutOfBoundException�쳣
         * һ������"ȷ���Ի�ȡ"����ȡ����ֱ�����쳣��
         * ������֧�֣���index < 0���ȼ���size() + index
         *
        **/
        virtual const IVar& operator []( int idx ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ��index >= size()��������Զ�������index + 1
         * һ�����������±�󶨻���ȷ���Ի�ȡ����ȡ�����ᵼ�������Զ�������
         * ������֧�֣���index < 0���ȼ���size() + index
         *
        **/
        virtual IVar& operator []( int idx ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        //methods for dict
        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���ڲ����ڵ��ֶ����󶨣�����Lsc::var::Null::null��
         * һ��������̽�Ի�ȡ��
         *
        **/
        virtual IVar& get( const field_type& name ) {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���ڲ����ڵ��ֶ����󶨣�����ָ����Ĭ��ֵ��
         *
        **/
        virtual IVar& get( const field_type& name, IVar& default_value ) {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * �������ڸ��ֶ����󶨣�����Lsc::var::Null::null;
         * һ��������̽�Ի�ȡ
         *
        **/
        virtual const IVar& get( const field_type& name ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * �������ڸ��ֶ����󶨣�����ָ����Ĭ��ֵ
         * һ��������̽�Ի�ȡ
         *
        **/
        virtual const IVar& get( const field_type& name, const IVar& default_value ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual void set( const field_type& name, IVar& value_ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] value["<<value_.dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���󶨲����ڣ�����false�����򣬷���true��
         *
        **/
        virtual bool del( const field_type& name ) {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���󶨲����ڣ��׳�Lsc::KeyNotFoundException�쳣
         * һ������"ȷ���Ի�ȡ"����ȡ����ֱ�����쳣��
         *
        **/
        virtual const IVar& operator []( const field_type& name ) const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * ���󶨲����ڣ��½��󶨡�
         * һ�����ڰ󶨻���ȷ���Ի�ȡ����ȡ�����ᵼ���½��󶨣�
         *
        **/
        virtual IVar& operator []( const field_type& name ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual dict_const_iterator dict_begin() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual dict_iterator dict_begin() {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual dict_const_iterator dict_end() const {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         *
        **/
        virtual dict_iterator dict_end() {
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         *
         * ����is_callable()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * paramsӦ����һ��is_array()����true��IVar����
         * ע������һ�������Է�����Lsc::ResourcePool�п��ܱ�Lsc::var::IFactory����
         *
        **/
        virtual IVar& operator()(IVar& params, Lsc::ResourcePool& /*rp*/ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] params["<<params.dump(1)<<"]";
        }

        /**
         *
         * ����is_callable()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�Lsc::InvalidOperationException�쳣
         * self����������IVar����
         * paramsӦ����һ��is_array()����true��IVar����
         * ע������һ�������Է�����Lsc::ResourcePool�п��ܱ�Lsc::var::IFactory����
         *
        **/
        virtual IVar& operator()(IVar& self, IVar& params, Lsc::ResourcePool& /*rp*/ ){
            throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] self["<<self.dump(0)<<"] params["<<params.dump(1)<<"]";
        }

        /**
         *
        **/
        static inline bool check_mask( mask_type mask, mask_type set_bits, mask_type unset_bits = 0 ){
            return ((mask & set_bits) == set_bits) && ((mask & unset_bits) == 0);
        }
    };

    /**
     *
    **/
    template<>
        inline bool IVar::to<bool>() const{
            return this->to_bool();
        }
    /**
     *
    **/
    template<>
        inline signed char IVar::to<signed char>() const{
            return this->to_int8();
        }
    /**
     *
    **/
    template<>
        inline unsigned char IVar::to<unsigned char>() const{
            return this->to_uint8();
        }
    /**
     *
    **/
    template<>
        inline signed short IVar::to<signed short>() const{
            return this->to_int16();
        }
    /**
     *
    **/
    template<>
        inline unsigned short IVar::to<unsigned short>() const{
            return this->to_uint16();
        }
    /**
     *
    **/
    template<>
        inline signed int IVar::to<signed int>() const{
            return this->to_int32();
        }
    /**
     *
    **/
    template<>
        inline unsigned int IVar::to<unsigned int>() const{
            return this->to_uint32();
        }
    /**
     *
    **/
    template<>
        inline signed long long IVar::to<signed long long>() const{
            return this->to_int64();
        }
    /**
     *
    **/
    template<>
        inline unsigned long long IVar::to<unsigned long long>() const{
            return this->to_uint64();
        }
    /**
     *
    **/
    template<>
        inline float IVar::to<float>() const{
            return this->to_float();
        }
    /**
     *
    **/
    template<>
        inline dolcle IVar::to<dolcle>() const{
            return this->to_dolcle();
        }
    /**
     *
    **/
    template<>
        inline Lsc::string IVar::to<Lsc::string>() const{
            return this->to_string();
        }
    /**
     *
    **/
    template<>
        inline raw_t IVar::to<raw_t>() const{
            return this->to_raw();
        }
    /**
     *
    **/
    template<>
        inline const char * IVar::to<const char *>() const{
            return this->c_str();
        }

#if __WORDSIZE == 64 
    /**
     *
    **/
    template<>
        inline signed long IVar::to<signed long>() const{
            return this->to_int64();
        }
    /**
     *
    **/
    template<>
        inline unsigned long IVar::to<unsigned long>() const{
            return this->to_uint64();
        }
#else
    /**
     *
    **/
    template<>
        inline signed long IVar::to<signed long>() const{
            return this->to_int32();
        }

    /**
     *
    **/
    template<>
        inline unsigned long IVar::to<unsigned long>() const{
            return this->to_uint32();
        }
#endif

}}   //  namespace Lsc::var

#endif  //__BSL_VAR_IVAR_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */

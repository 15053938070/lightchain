#ifndef  __BSL_CHECK_CAST_H_
#define  __BSL_CHECK_CAST_H_

#include "Lsc/exception/Lsc_exception.h"

namespace Lsc{
    /**
     *
     * �ṩ��C/C++��ԭ�������ṩ�����������鹦��ģ�庯����ʹ�÷�ʽ��static_cast<>��ȫ��ͬ��
     *
     * Ŀǰ֧�������з�����ֵ���͡��޷�����ֵ���͡����������������������͵�˫��ת���Լ�C����ַ�����const char *�������ϸ����͵ĵ���ת���������Ǹ�����ת���ɸ��������͵���ֵ������⡣
     *
    **/
    template<typename DestType, typename SrcType>
        DestType check_cast( SrcType value );


    /**
     *
    **/
#if __GNUC__ < 4
    template<typename DestType>
        inline DestType check_cast( DestType value ){
            return value;
        }
#endif

}
#include "Lsc/check_cast/check_cast_generated.h"   //generated code
#include "Lsc/check_cast/check_cast_cstring.h"
#include "Lsc/check_cast/check_cast_Lsc_string.h"

#endif  //__CHECK_CAST_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

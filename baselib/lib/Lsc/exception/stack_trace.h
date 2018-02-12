
#ifndef  __BSL_STACK_TRACE_H__
#define  __BSL_STACK_TRACE_H__

#include "Lsc/AutoBuffer.h"

namespace Lsc{

    /**
     * ������Ŀ���ļ�ʱ���������-rdynamic����
     *
    **/
    void stack_trace( AutoBuffer& buf, size_t total_level, size_t begin_level, size_t max_level,
            const char* line_delimiter );

    /**
     *
    **/
    void demangle( AutoBuffer& buf, const char * mangled_name );

}
//namespace Lsc 
#endif  //__BSL_STACK_TRACE_H__


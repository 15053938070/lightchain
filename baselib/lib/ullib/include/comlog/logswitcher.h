

#ifndef  __LOGSWITCHER_H_
#define  __LOGSWITCHER_H_

namespace comspace{

//����ul_log��com_log֮���л���ȫ�ֱ���
//extern int __log_switcher__;
extern int com_get_log_switcher();
extern void com_set_log_switcher(int swt);
enum{
	__LOG_NOT_DEFINED__ = 0,
	__USING_ULLOG__ ,
	__USING_COMLOG__ 
};

};













#endif  //__LOGSWITCHER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */



#ifndef  __BASICLOGADAPTER_H_
#define  __BASICLOGADAPTER_H_

#include "com_log.h"

//Ϊmask���һ����־�ȼ�,maskӦ����һ��uint32_t
#define COMLOG_MASK(mask, level) do{\
	(mask) |= (unsigned int)(1) << (level);\
}while(0)

//Ϊmask���5��Ĭ�ϵ�ϵͳ��־�ȼ�,mask��һ��uint32_t
#define COMLOG_DEFAULT_MASK(mask) do{\
	COMLOG_MASK(mask, COMLOG_FATAL);\
	COMLOG_MASK(mask, COMLOG_WARNING);\
	COMLOG_MASK(mask, COMLOG_NOTICE);\
	COMLOG_MASK(mask, COMLOG_TRACE);\
	COMLOG_MASK(mask, COMLOG_DEBUG);\
}while(0)

/**
 *        BasicLogAdapter�����ͷ��κ���Դ�������Լ���������Դ��
 *        һ����˵��BasicLogAdapter��������������������
 *        ��Ҫ�ͷţ�ֻ��ʹ��com_closelog()
 *
 * 		  ����Ĭ��ʹ��ullog��ӡ��־
 *        ������setCategory, setDevice, setLayout�Ⱥ�����
 *        ���Զ��л���comlog�Ĺ���
 *
 *        setLevel���ܿؿ��أ���comlog/ullog����Ч�����Զ�����־����Լ����
 *
 *        ���Ҫ��ӡ�Զ���ȼ�����־��������ʽ����setLogStat��ʾ����
 *        BasicLogAdapter *p = new BasicLogAdapter();
 *        p->setDevice(xxx);//ע��Device
 *        comspace::LogStat lstat = comspace::LogStat::createLogStat();//��Ҫ��setDevice֮�����
 *        //��ΪcreateLogStat������comlog����ʼ��֮��ſ���ʹ�ã����Ա�����ִ��setDevice�����������ʼ��
 *        uint32_t level = 0;
 *        COMLOG_DEFAULT_MASK(level);
 *        lstat.setLevel(level, "SelfName1, SelfName2"); //���SelfName1Ҳ��Ҫ�������ļ��е�selfdefine�ж���
 *        p->setLogStat(lstat);
 *        p->write("SelfName1", "This is a self level log...");
 *        //���ߣ�
 *        int self = comspace::getLogId("SelfName1");
 *        p->write(self, "This is also a self level log...");
 *        
 */
class BasicLogAdapter{
	protected:
		//ʵ�ʵ���־��ӡ���
		comspace :: Category * _cate;
		//ϵͳ��־�ȼ�
		int _level;
	plclic:
		BasicLogAdapter();
		/**
		 *
		**/
		int setCategory(comspace::Category * cate);
		/**
		 *
		**/
		int setDevice(com_device_t * dev, int num);
		/**
		 *
		**/
		int setLogStat(comspace::LogStat * logstat);
		/**
		 *
		**/
		int setLevel(int level);
        /**
         *
        **/
        int getLevel();
		/**
		 *
		**/
		int write(int level, const char * fmt, ...) __attribute__ ((format (printf,3,4)));
		/**
		 *
		**/
		int write(const char * level, const char * fmt, ...) __attribute__ ((format (printf,3,4)));
		//----- ���½ӿ��൱��write�İ�װ---
		/**
		 *
		**/
		int fatal(const char * fmt, ...) __attribute__ ((format (printf,2,3)));
		/**
		 *
		**/
		int warning(const char * fmt, ...) __attribute__ ((format (printf,2,3)));
		/**
		 *
		**/
		int notice(const char * fmt, ...) __attribute__ ((format (printf,2,3)));
		/**
		 *
		**/
		int trace(const char * fmt, ...) __attribute__ ((format (printf,2,3)));
		/**
		 *
		**/
		int debug(const char * fmt, ...) __attribute__ ((format (printf,2,3)));
	protected:
		/**
		 *
		**/
		int write_args(int level, const char * fmt, va_list args);
		comspace :: Category * tryCategory();
		static int _instance;
		static pthread_mutex_t _lock;
};

















#endif  //__BASICLOGADAPTER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

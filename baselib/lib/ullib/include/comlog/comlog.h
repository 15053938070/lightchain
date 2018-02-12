

#ifndef  __COMLOG_H_
#define  __COMLOG_H_

#include "ul_def.h"
#include "ul_conf.h"

#define COM_USRLEVELSTART  			32		//�û��ȼ�id����ʵλ��
#define COM_LEVELNUM  				64		//���֧�ֵ���־�ȼ�
#define COM_SELFLEVELNUM 			32		//���֧�ֵ��Զ�����־�ȼ���Ŀ
#define COM_MAXLEVELNAMESIZE 		1024		//�Զ�����־�ȼ�������󳤶�
#define COM_MAXDEVICENUM			1024		//���֧�ֵ�appender��
#define COM_MAXCATEGORYNAMESIZE		64		//category���ֵ���󳤶�
#define COM_MAXHOSTNAME				1024	//���������:port/path
#define COM_MAXFILENAME				256		//����ļ���
#define COM_MAXDUPSERVER			32		//����������̨��
#define COM_MAXSERVERSUPPORT 		32		//���֧�����ӵ�server��Ŀ
#define COM_MODULENAMESIZE 			64		//ģ�����������
#define COM_MAXLOGFILE				2018	//ÿ����־�ļ�֧�ֵ���󳤶ȣ���λm
#define COM_MAXCUTTIME				10000000 //�����־�и�ʱ�䣬��λ���
#define COM_MAXAPPENDERNAME			16		//Appender���ֵ���󳤶�
#define COM_RESERVEDSIZE			32		//com_device_t�б����ֶεĳ���
#define COM_RESERVEDNUM             64      //com_device_t�б�����չ�ֶεĸ���

//��׼��������
#define COMLOG_LEVEL				"COMLOG_LEVEL"	//��־�ȼ�
#define COMLOG_SELFDEFINE			"COMLOG_SELFDEFINE"	//���յ��Զ�����־
#define COMLOG_DEVICE_NUM			"COMLOG_DEVICE_NUM"	//�洢��־���豸��
#define COMLOG_DEVICE				"COMLOG_DEVICE"	//ǰ׺
#define COMLOG_PROCNAME				"COMLOG_PROCNAME"	//��ģ�������
#define COMLOG_LOGLENGTH			"COMLOG_LOGLENGTH" //��־��󳤶�
#define COMLOG_TIME_FORMAT			"COMLOG_TIME_FORMAT" //��־��ʱ�䲿�ֵ��Զ����ʽ
#define COMLOG_DFS                  "COMLOG_DFS"        //Ϊ�ֲ�ʽƽ̨������չ
#if 0
//�豸���ͱ�־����
#define COMLOG_DEVICE_FILE			"FILE"					//�ļ����
#define COMLOG_DEVICE_NET			"NET"					//�������
#define COMLOG_DEVICE_TTY			"TTY"					//�ն����
#define COMLOG_DEVICE_ULLOG			"ULLOG"				//��ullog�ļ�����һ��
#define COMLOG_DEVICE_ULNET			"ULNET"				//��ullog�ļ�����һ�µ�������־
#define COMLOG_DEVICE_AFILE			"AFILE"				//�첽�������������ļ����
#endif
#define COMLOG_DEVICE_PATH			"PATH"					//�豸·��
#define COMLOG_DEVICE_NAME 			"NAME"					//��־�ļ���
#define COMLOG_DEVICE_AUTH			"AUTH"					//�ϲ���Ȩ
#define COMLOG_DEVICE_TYPE			"TYPE"					//�豸����
#define COMLOG_DEVICE_OPEN			"OPEN"					//�Ƿ���������豸
#define COMLOG_DEVICE_SIZE			"SIZE"					//��־��С
#define COMLOG_DEVICE_SPLITE_TYPE	"SPLITE_TYPE"			//�ָ�����
#define COMLOG_DEVICE_DATECUTTIME	"DATA_CUTTIME"			//�и����� ����Ϊ��λ������0 Ĭ��60
#define COMLOG_DEVICE_COMPRESS		"COMPRESS"				//�Ƿ�֧��ѹ��
#define COMLOG_DEVICE_SYSLEVEL		"SYSLEVEL"				//֧�ֵ�ϵͳ��־�ȼ�
#define COMLOG_DEVICE_SELFLEVEL		"SELFLEVEL"				//֧�ֵ��Զ�����־�ȼ�
#define COMLOG_DEVICE_LAYOUT		"LAYOUT"				//�豸ģ��
#define COMLOG_DEVICE_LAYOUT_NDC	"LAYOUT_NDC"			//����ndcģ��
#define COMLOG_DEVICE_RESERVED1		"RESERVED1"				//�����ֶ�
#define COMLOG_DEVICE_RESERVED2		"RESERVED2"
#define COMLOG_DEVICE_RESERVED3		"RESERVED3"
#define COMLOG_DEVICE_RESERVED4		"RESERVED4"
//////#ifndef u_int64
////#define u_int64 unsigned long long
//#endif
#if 0
enum {
	COM_DEVICESTART = 0,
	COM_FILEDEVICE = 1,	//�ļ��豸
	COM_NETDEVICE = 2,	//�����豸
	COM_TTYDEVICE = 3,	//�ն���ʾ
	COM_ULLOGDEVICE = 4,	//��ul_log�ļ�������һ��
	COM_ULNETDEVICE = 5,	//��ullog�ļ�����һ�µ�������־
	COM_AFILEDEVICE = 6,
	COM_DEVICENUM		//֧�ֵ��豸��
};
#endif
//����SPLITE_TYPEʱ���������ֵ������������ַ�����Ҳ����ʹ0��1��2������
#define COMLOG_DEVICE_TRUNCT 		"TRUNCT"	
#define COMLOG_DEVICE_SIZECUT 		"SIZECUT"
#define COMLOG_DEVICE_DATECUT		"DATECUT"
enum {
	COM_TRUNCT = 0,		//ֱ�������־�Ӵ�(0)
	COM_SIZECUT,	//����С�ָ���־(1)
	COM_DATECUT,	//�����и�(2)
};

enum {
	LOG_NOT_DEFINED = 0, //δ��ʽ�����κ�openlog/loadlog��Ĭ��ullog
	USING_COMLOG,//ʹ��comlog
	USING_ULLOG,//ʹ��ullog
};

#define COMLOG_FATAL 1
#define COMLOG_WARNING 2
#define COMLOG_NOTICE 4
#define COMLOG_TRACE 8
#define COMLOG_DEBUG 16

#define UL_LOGLINE(x) UL_LOGLINE_(x)
#define UL_LOGLINE_(x) #x

#define CFATAL_LOG(fmt, arg...) \
do { \
	com_writelog(COMLOG_FATAL, "["__FILE__":"UL_LOGLINE(__LINE__)"]"fmt, ##arg); \
} while (0)

#define CWARNING_LOG(fmt, arg...) \
do { \
	com_writelog(COMLOG_WARNING, "["__FILE__":"UL_LOGLINE(__LINE__)"]"fmt, ##arg); \
} while (0)

#define CNOTICE_LOG(fmt, arg...) \
do { \
	com_writelog(COMLOG_NOTICE, "["__FILE__":"UL_LOGLINE(__LINE__)"]"fmt, ##arg); \
} while (0)

#ifdef CLOSE_TRACE_LOG
#define CTRACE_LOG(fmt, arg...) ((void *)(0))
#else
#define CTRACE_LOG(fmt, arg...) \
do { \
	com_writelog(COMLOG_TRACE, "["__FILE__":"UL_LOGLINE(__LINE__)"]"fmt, ##arg); \
} while (0)
#endif

#ifdef CLOSE_DEBUG_LOG
#define CDEBUG_LOG(fmt, arg...) ((void *)(0))
#else
#define CDEBUG_LOG(fmt, arg...) \
do { \
	com_writelog(COMLOG_DEBUG, "["__FILE__":"UL_LOGLINE(__LINE__)"]"fmt, ##arg); \
} while (0)
#endif

namespace comspace {
	class Layout;
	class Category;
	class Event;
	class Appender;
};

//���֧�ֵ���־�ȼ�
#define COMLOG_ADDMASK(dev, loglvl) \
{ \
	(dev).log_mask |= ((unsigned long long)(1)<<loglvl);\
}
//ɾ��֧�ֵ���־�ȼ�
#define COMLOG_DELMASK(dev, loglvl) \
{ \
	(dev).log_mask &= ~((unsigned long long)(1)<<loglvl); \
}
//�����־����־�ȼ�
#define COMLOG_CLEARMASK(dev) \
{ \
	(dev).log_mask = 0; \
}
//��ʼ�������ϵͳ��־�ȼ�
#define COMLOG_SETSYSLOG(dev) \
{ \
	COMLOG_ADDMASK(dev, COMLOG_FATAL); \
	COMLOG_ADDMASK(dev, COMLOG_WARNING); \
	COMLOG_ADDMASK(dev, COMLOG_NOTICE); \
	COMLOG_ADDMASK(dev, COMLOG_TRACE); \
	COMLOG_ADDMASK(dev, COMLOG_DEBUG); \
}

//�豸��
struct com_device_t
{
	char host[COM_MAXHOSTNAME];	//������, ip:port/path ���������Ϊ�ձ�ʾΪ�����ļ�
	char file[COM_MAXFILENAME];	//��־��ӡ�����ļ�
	char auth[COM_MODULENAMESIZE];	//����ǿգ���ô��ͬ�ļ�������־����ͬһ̨�����Ϻϲ�
	char type[COM_MAXAPPENDERNAME]; //�豸���ͣ�appender���ͣ�

	char reserved1[COM_RESERVEDSIZE];//�����ֶ�
	char reserved2[COM_RESERVEDSIZE];
	char reserved3[COM_RESERVEDSIZE];
	char reserved4[COM_RESERVEDSIZE];
    int log_size;		//�ع���־��С 
	char open;			//�Ƿ�����
	char splite_type;	//TRUNCT��ʾֱ�ӻع���SIZECUT ��ʾ����С�ָDATECUT��ʾ��ʱ��ָ�
	char compress;		//����Э�飬�Ƿ�ѹ�� 0��ʾ��ѹ����1��ʾѹ��
	int cuttime;		//�и�ʱ�����ڣ�Ĭ��Ϊ0�������и�
	unsigned long long log_mask;		//������Щ�ȼ���־��Ĭ��Ϊ0��ʾ����ϵͳ�ȼ���־,ADD_MASK, DEL_MASK ��������			
    //������չ���������飬���ǵ�4��reserved�ֶ��Ѿ���ʹ�ã�reserved�ֶβ�ɾ��
    char name[COM_MAXAPPENDERNAME]; //�����е�����
    unsigned int reserved_num;
    ul_confitem_t reservedext[COM_RESERVEDNUM];
    ul_confdata_t reservedconf;
	
	comspace::Layout *layout;		//�Զ�����־�ĸ�ʽ��null��ʾ����Ĭ�ϸ�ʽ
plclic:
	com_device_t();
};

int set_comdevice_default(com_device_t *dev);
int set_comdevice_selflog(com_device_t *dev, const char *logname);

struct com_logstat_t
{
	//��16��ʾdebug�ȼ�8��ʾtrace�ȼ�4��ʾnotice�ȼ���2��ʾwarning�ȼ���1��ʾfatal�ȼ�
	int sysevents;			//ϵͳ��־�ȼ�
	//��������ã�userevents[0] = 0; ��ʾ��ӡ�����Զ�����־
	//���õĻ� �Զ���ȼ���,�Զ���ȼ������ö��Ÿ�������ʾҪ��ӡ
	char userevents[1024];	//�Զ�����־�ȼ�
	//1��ʾ���ն˴�ӡ��־��0��ʾ����ʾ
	int ttydisp;			//�Ƿ����ն���ʾ����

	com_logstat_t();
};

/**
 *
 * 	ϵͳ��־����ݵ�ǰ�ȼ���ָ�����豸��ӡ��
 * 	���紴����һ���ļ���־�豸����ô���ᴴ��name.log��name.log.wf�����ļ�
 * 	notice, trace, debug��name.log�ļ���fatal, warning��name.log.wf��
**/
int com_openlog(const char *procname, com_device_t *dev, int dev_num, com_logstat_t *stat);//����־����ָ����־�ȼ������������־�豸

/**
 *
**/
int com_loadlog(const char *path, const char *file);

/**
 *
**/
int com_loadlog_ex(const char *path, const char *file, int id);

/**
 *
**/
int com_openlog_r();


/**
 *
**/
int com_closelog_r();

/**
 *
**/
int com_getlogid(char *selflog);

/**
 * 	ע�⣬���߳���Ӻ��create���߳���Ч������������߳�create��֮����ӵģ��������̶߳��е�
 * 	����豸�Ѿ����ڣ������selflog���������Զ�������
 * 	����豸�����ڻ�����
 * 	�����Ҫ���ö���Զ������ӣ��벻�ϵ����������
 *
**/
int com_setdevices(com_device_t *dev, int nums, char *selflog = NULL);

/**
 *
 * 	ǿ�ƹر���־����ʧû�д�ӡ��ȥ����־������ 0 ��ʾ���ȴ�
 * 	 ��ע�⡿�����waittimeֻ��NetAppender��Ч�������ʹ�õ�AsyncFileAppender����NetcpAppender
 * 	 �������첽��ӡ�ķ�ʽ�����Ƕ�������һ���������̸߳���IO��������
 * 	 ����Ҫ�ڵ���com_closelog֮ǰ���е���sleep�����Եȴ��첽�̵߳Ĵ�ӡ������
 * 	 �����ǿ���˳��������ڴ��е���־�ᱻ������
**/
int com_closelog(int waittime = 1000);//�ر���־

/**
 *
 * has supported it.
**/
int com_resetlog();



/* �رո��߳���ָ��appender_type�����,��Ӱ�������߳�
* appender_type :  appender�������е����� 
* ����0 �ɹ� �� ��0 ʧ��
*/
int com_setappendermask(char *appender_type);

/**
 *
**/
int com_writelog(int events, const char *fmt, ...);//������־id��ӡ��־

/**
 *
**/
int com_writelog_ex(int events, int version, const char *type, const char *fmt, ...);

/**
 *
**/
int com_writelog(const char *name, const char *fmt, ...);//������־����ӡ��־

/**
 *
**/
int com_writelog_ex(const char *name, int version, const char *type, const char *fmt, ...);

/*
 * 	��(��־�ȼ�, ����) ������ӡ
 **/
#define com_pushlog(key, fmt, arg...) com_getcategory()->push(key, fmt, ##arg)

/**
 * 	�����������£��ı���־�ȼ�
 *
**/
int com_reglogstat(const char *path, const char *file);

/**
 *
**/
int com_changelogstat();

/**
 *
**/
int com_changelogstat(com_logstat_t *logstat);

/**
 *
 * 			USING_COMLOG : ʹ��comlog
 * 			USING_ULLOG  : ʹ��ullog
 * 			LOG_NOT_DEFINED : δ��ʽ�����κ�openlog/loadlog
**/
int com_logstatus();

/**
 * �û����Լ̳�Appender�࣬ʵ��һ���Լ���Appender����ע����Ӧ��get/try����
 * name��ʾ�Զ���appender�����ƣ������������ļ��б�ʶ��
 *
**/
typedef comspace::Appender*(*func_type)(com_device_t &);
int com_registappender(const char * name, func_type getAppender, func_type tryAppender);


//�߳�����
//��Щ��������������Ա�����Ƽ�ʹ��
struct com_thread_data_t
{
	comspace::Category *category;
	comspace::Event *event;
};
comspace::Category * com_getcategory();
comspace::Event * com_getevent();
com_thread_data_t * com_getthreaddata();

#endif  //__LOG_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */


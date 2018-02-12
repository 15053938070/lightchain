

#ifndef  __LOGHEAD_H_
#define  __LOGHEAD_H_

#include <unistd.h>
#include<sys/uio.h>
namespace comspace{

const unsigned int LOGHEAD_MAGICNUM = 0xfb709394;

/// ��д��־λ�����ж�д����Ĭ��LOGHEAD_CHECK_MAGICNUM
typedef enum _LOGHEAD_FLAGS_T {
    LOGHEAD_CHECK_NONE        =    0,
    LOGHEAD_CHECK_MAGICNUM    = 0x01,    ///<���magic_num�Ƿ���ȷ
    LOGHEAD_CHECK_PARAM       = 0x02,    ///<�������Ƿ���ȷ
} LOGHEAD_FLAGS_T;


/// ���ش����� = LOGHEAD_RET_SUCCESS�ɹ�, <0ʧ��
typedef enum _LOGHEAD_RET_ERROR_T {
    LOGHEAD_RET_SUCCESS       =   0, ///<��дOK
    LOGHEAD_RET_EPARAM        =  -1, ///<����������
    LOGHEAD_RET_EBODYLEN      =  -2, ///<�䳤���ݳ���������
    LOGHEAD_RET_WRITE         =  -3, ///<д������
    LOGHEAD_RET_READ          =  -4, ///<����Ϣ��ʧ�ܣ��������errno
    LOGHEAD_RET_READHEAD      =  -5, ///<����Ϣͷʧ��, �������errno
    LOGHEAD_RET_WRITEHEAD     =  -6, ///<д��Ϣͷʧ��, �����ǶԷ������ӹر���
    LOGHEAD_RET_PEARCLOSE     =  -7, ///<�Զ˹ر�����
    LOGHEAD_RET_ETIMEDOUT     =  -8, ///<��д��ʱ
    LOGHEAD_RET_EMAGICNUM     =  -9, ///<magic_num��ƥ��
    LOGHEAD_RET_UNKNOWN	     =  -10
} LOGHEAD_RET_ERROR_T;

/**
 * ns��Ʒ�����罻��ͳһ�İ�ͷ��ע�Ͱ���Ϊ(M)��Ϊ������ѭ�Ĺ淶
 */
typedef struct _loghead_t
{
    unsigned short id;              ///<id
    unsigned short version;         ///<�汾��
    ///(M)��apache������logid���ᴩһ��������������罻��
    unsigned int   log_id;
    ///(M)�ͻ��˱�ʶ������������ʽ����Ʒ��-ģ����������"sp-ui", "mp3-as"
    char           provider[16];
    ///(M)�����ʶ����ʶһ��������ʼ
    unsigned int   magic_num;
    unsigned int   reserved;       ///<����
    ///(M)head���������ݵ��ܳ���
    unsigned int   body_len;
} loghead_t;

/** 
 * 
 * 
 */
int loghead_read(int sock, void *buf, size_t buf_size, int timeout,
	        unsigned flags = LOGHEAD_CHECK_MAGICNUM);



/**
 * д����, һ��д���������ݡ������ܳ���Ϊsizeof(loghead_t)+head->body_len;
 */
int loghead_write(int sock, loghead_t *head, int timeout,
	unsigned flags = LOGHEAD_CHECK_MAGICNUM);


}











#endif  //__LOGHEAD_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
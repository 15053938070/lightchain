


#ifndef __DEP_HTTPLIB_H__
#define	__DEP_HTTPLIB_H__



#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <endian.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ul_net.h"



#define    MAXURL        256	 /**< Max url length */
#define    MAXHOST       64		 /**< Max url host part length */
#define    MAXFILE        192	 /**< Max url file part length */
#define    MAXPAGE        256000 /**< Max http page lenght */
#define    MAXHEAD        4096	 /**< Mac http head length */
#define    MAXGETCMD      1256	 /**< Max http command length */
#define    MAXIP          16	 /**< Max ip string length */
#define    MAXPORTS        8
/* Write, read and connection timeout for socket */
#define    WRITETIMEOUT   60
#define    READTIMEOUT    300
#define    CONNECTTIMEOUT 30
#define    RETRYTIME      1		 /**< Retry times for tcp connection */
#define    SITE_LEN 100
#define    PORT_LEN 10
#define MAX_ETAG_LEN 64
#define METHOD_GET 1
#define METHOD_CHK 2
#define LASTMOD_FLAG 1
#define ETAG_FLAG 2



/**
 * ���ṹ���Ѿ�����,���Ƽ�ʹ��
 */
typedef struct __page_item {
	int method;
	int orilen;
	int head_flag;
	char url[MAXURL];
	char ip[MAXIP];
	char protocol[4];
	int pagesize;
	int status;
	int conlen;
	char lastmod[MAXLINE];
	char contyp[MAXLINE];
	char trnenc[MAXLINE];
	char etag[MAX_ETAG_LEN];
	char redir[MAXURL];
	char *page;
	int proxy_flag;
} PAGE_ITEM;



/**
 *
 */
extern int check_URL(char *url);



/**
 * - <hostname> ::= <name>*["."<name>]
 * - <name>  ::= <letter>[*[<letter-or-digit-or-hyphen>]<letter-or-digit>]
 * - <hyphen> ::= '-'-or-'_'
 *
 *
 */
extern int check_hostname(char *host_name);



/**
 *
 * - 1 �ɹ���ȡ����
 * - 0 ʧ��
 */
int isconredir(char *page, int size, char *redir);



/**
 *
 * - in, page�ĳ���.  *pagesize=0,ֻ��ȡhead,����ȡҳ������; 
 * - out, ���ػ�õ�ҳ�泤�� *
 * - <0 �д���
 * - >0 ��ҳhttp����״̬��
 */
extern int ul_gethttpcontent(char *url, char *ip, int *pagesize, char *head, char *page,
							 char *redirurl, char *lastmod);



/**
 *
 * - in, ����url,ip,port����Ϣ
 * - out, ����ȡ�õ���ҳ
 * - >0 ��ҳhttp����״̬��
 * - -1 ʧ��
 * - -2 ��ʱ
 */
extern int ul_gethttpcontent_new(PAGE_ITEM * pageitem);



/**
 *
 * - in, ����url,ip,port����Ϣ
 * - out, ����ȡ�õ���ҳ
 * - >0 ��ҳhttp����״̬��
 * - -1 ʧ��
 * - -2 ��ʱ
 *		����chunk�ķ�ʽҲ������
 */
extern int ul_gethttpcontent_new_notpt(PAGE_ITEM * pageitem);

#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

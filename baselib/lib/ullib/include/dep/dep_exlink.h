


#ifndef __DEP_EXLINK_H__
#define __DEP_EXLINK_H__
#include "ul_url.h"

#define  UL_MAX_LINK_NUM     2000
#define  UL_MAX_TEXT_LEN   100

#define UL_METHOD_IXINTUI		0
#define UL_METHOD_WGET16	1
#define UL_METHOD_WGET18	2


typedef struct _link_info_t {
	char domain[UL_MAX_SITE_LEN];
	char port[UL_MAX_PORT_LEN];
	char path[UL_MAX_PATH_LEN];
	char text[UL_MAX_TEXT_LEN];
	char url[UL_MAX_URL_LEN];
} link_info_t;



/**
 * ����url����Ե�url���ϳ��µ�url
 * 
 * - 1  �ɹ�
 * - 0  ʧ��
 */
int ul_absolute_url(char *url, char *relurl, char *absurl);



/**
 * ����url��Ӧ��page���ݣ��õ��µ�url(��ȡ����)
 * 
 * - UL_METHOD_WGET16
 * - UL_METHOD_WGET18	
 * - >=0   �ɹ���������ȡ�ɹ���url��
 * - <0    ʧ��
 */
int extract_link(char *url, char *page, int pagesize, link_info_t * link_info, int num, int method);
#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

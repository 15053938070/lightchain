
#ifndef __PACK_UL_H__
#define __PACK_UL_H__

#include <errno.h>
#include <unistd.h>

#include "ul_log.h"
#include "ul_net.h"
#include "ul_func.h"

#include "dep/dep_pack.h"

#define MAX_PACK_ITEM_NUM  200			/**< ���Բ����name:value���������       */
#define MAX_ITEM_SIZE  2048				/**< name��value����󳤶�       */
#define SIZE_NUM  20
#define MAX_HEAD_SIZE  26000				/**< ����ͷ��󳤶� */

typedef struct ul_package {
    char name[MAX_PACK_ITEM_NUM][MAX_ITEM_SIZE];
    char value[MAX_PACK_ITEM_NUM][MAX_ITEM_SIZE];
    int pairnum;      /**< name:value �Եĸ�����������MAX_PACK_ITEM_NUM       */
    int bufsize;      /**< �����buff����       */
    char *buf;        /**< �����buff��ʹ��ʱ��Ҫ�ⲿ������뺯��       */
} ul_package_t;

//typedef struct ul_package_t ul_package_t;

/**
 *
 *  
 *  - NULL   ��ʾʧ��
 *  - ��NULL ��ʾ�ɹ�
 */
ul_package_t *ul_pack_init();

/**
 * 
 *
 *  - 0   ��ʾ�ɹ�
 */
int ul_pack_free(ul_package_t *ppack);

/**
 *  
 *  - 0   ��ʾ�ɹ�
 *  - <0  ��ʾʧ��
 */
int ul_pack_putbuf(ul_package_t *ppack, char *buffer, int size);

/**
 *  
 *  - ��NULL   ��ʾ�ɹ�
 *  - NULL     ��ʾʧ��
 */ 
char *ul_pack_getbuf(ul_package_t *ppack);

/**
 *  
 *  - 1  �����Ѵ��ڣ���ӳɹ�
 *  - 0  �ɹ�
 *  - -1 ���ֻ�ֵ���������ȣ�ʧ��
 *  - -2 packû�пռ䣬ʧ��
 */  
int ul_pack_putvalue(ul_package_t *ppack, const char *pname, const char *pvalue);

/**
 *  
 *  - 1  �ɹ�
 *  - 0  ʧ��
 */ 
int ul_pack_getvalue_ex(ul_package_t *ppack, const char *pname, char *pvalue, size_t size);

/**
 *
 * -1 : ������� ���� key������
 * 0 : �ɹ�
**/
int ul_pack_delvalue(ul_package_t *ppack, const char *pname);

/**
 *  
 *  - >=0  pack����Ĵ�С
 *  - <0   ʧ��
 *
 */
int ul_pack_reado_ms(int sockfd, ul_package_t *ppack, int msec);

/**
 *  
 *  - 0   �ɹ�
 *  - -1  ʧ��
 *  - -2  �������
 *  
 */
int ul_pack_writeo_ms(int sockfd, const ul_package_t *ppack, int msec);


/**
 *  дpack�����Ƚ�pack���������ʱ���������ٽ�������������ͨ�����Ӿ��д��,֧�ֺ��뼶��ʱ
 *  
 *  - 0    �ɹ�
 *  - -1   ʧ��
 */ 
int ul_pack_write_mo_ms(int sockfd, ul_package_t *ppack, int msec);

/**
 *  ���л�pack������pack��������ڴ�buffer
 *  
 *  - >=0    ʵ��ʹ�õ�buffer����
 *  - -1   ʧ��
 */ 
int ul_pack_serialization(void *buf, int size, ul_package_t *ppack);

/**
 *  �����л��ڴ�buffer�����ڴ����ݽ�����pack��
 *  
 *  - >=0    ʵ��ʹ�õ�buffer����
 *  - -1   ʧ��
 */ 
int ul_pack_deserialization(void *buf, int size, ul_package_t *ppack);

/**
 *  
 *  - 1    �ɹ�
 *  - -1   ʧ��
 */
int ul_pack_cleanhead(ul_package_t* ppack);

 
/**
 *  
 *  - 1    �ɹ�
 *  - -1   ʧ��
 */ 
int ul_pack_cleanall(ul_package_t* ppack);


#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

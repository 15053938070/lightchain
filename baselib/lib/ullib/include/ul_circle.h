/////////////////////////////////////////////////////////////////////
//            Data struct abd function about data circle           //
/////////////////////////////////////////////////////////////////////
/**
 */
#ifndef __CIRCLEB_H__
#define __CIRCLEB_H__

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ul_log.h"
#include "ul_func.h"

/* 
** Return code for function below 
*/

#define DEF_ERR_CQFULL		-2
#define DEF_ERR_NOTINIT		-3
#define DEF_ERR_CQEMPTY		-4
#define DEF_ERR_INITAGAIN	-5
#define DEF_ERR_PARTOOLITTLE	-6
#define DEF_ERR_MALLOC		-7
#define DEF_ERR_INVALID_PARAM -8

struct circleB_t 
{
	void *pbyBuf; // store the data of the circle
	int sBufLen;  // length of the circle
	int sHead;    // head of the circle
	int sTail;    // tail of the circle
};


/**
 * ��������������
 *  - 0 ��ʾ�ɹ�
 *  - DEF_ERR_CQFULL  ��ʾʧ��
 */ 
extern int nCBput( struct circleB_t *pstCB, void *pbyPut, int nSize );

/**
 * �Ӷ�����ȡ����
 *  - 0 ��ʾ�ɹ�
 *  - DEF_ERR_CQEMPTY  ��ʾʧ�� 
 */ 
extern int nCBget( struct circleB_t *pstCB, void *pbyGet, int nSize );

/**
 * ��ʼ�������ַ�����
 *
 *  - NULL ��ʾʧ��
 *  - ��NULL ��ʾ��ʼ���ɹ� 
 */ 
extern struct circleB_t *pstCBinit( int npmBufSize );

/**
 * �ͷŻ����ַ�����
 *
 */ 
extern void vCBfree( struct circleB_t *pstCB );

 
/**
 * �Ӷ�����ȡ���ݣ����ݲ��Ӷ������Ƴ�
 *
 *  - 0 ��ʾ�ɹ�
 *  - DEF_ERR_CQEMPTY  ��ʾʧ�� 
 */  
extern int nCBlookup( struct circleB_t *pstCB, void *pbyGet, int nSize );

 
/**
 * ȡ���ζ��������ݵĳ���
 *
 */
extern int nCBgetdatalen( struct circleB_t *pstCB );

 
/**
 * ȡ���ζ�����ʣ��ռ�Ĵ�С
 *
 */
extern int nCBgetbufreelen( struct circleB_t *pstCB );

#endif

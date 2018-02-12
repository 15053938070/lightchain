
#ifndef __DEP_PACK_H__
#define __DEP_PACK_H__



/**
 *
 *
 */
typedef struct ul_package ul_package;

typedef struct ul_package ul_package_t;
/**
 *  
 *  - 1  �ɹ�
 *  - 0  ʧ��
 */ 
int ul_pack_getvalue(ul_package_t *ppack, char *pname, char *pvalue);


/**
 *  
 *  - >=0  pack����Ĵ�С
 *  - <0   ʧ��
 */
int ul_pack_read(int sockfd, ul_package_t *ppack);

 
/**
 *  
 *  - >0  ʵ�ʶ�ȡ���ֽ���
 *  - <=0   ʧ��
 */ 
int ul_pack_readall(int fd, void *ptr1, size_t nbytes);

/**
 *  
 *  - 0  �ɹ�
 *  - -1 ʧ��
 */ 
int ul_pack_readend(int sockfd);


/**
 *  
 *  - 0   �ɹ�
 *  - -1  ʧ��
 */ 
int ul_pack_write(int sockfd, ul_package_t *ppack);

 
/**
 *  
 *  - >0    ʵ��д�����ֽ���
 *  - <��0  ʧ��
 */
int ul_pack_writeall(int sockfd, char *buf, int n);



/**
 *  �ٽ�������������ͨ�����Ӿ��д��, �򲻴���ʱ����,���Ƽ�ʹ��
 *  
 *  - 0    �ɹ�
 *  - -1   ʧ��
 */ 
int ul_pack_write_m(int sockfd, ul_package_t *ppack);


 
/**
 *  
 *  - >=0  pack����Ĵ�С
 *  - <0   ʧ��
 */
int ul_pack_reado(int sockfd, ul_package_t *ppack, int over_time);

 
/**
 *  
 *  - 0   �ɹ�
 *  - -1  ʧ��
 */
int ul_pack_writeo(int sockfd, ul_package_t *ppack, int sec);


/**
 *  
 *  - >0    ʵ��д�����ֽ���
 *  - <��0  ʧ��
 */
int ul_pack_writeallo(int sockfd, char *buf, int n, int sec);


/**
 *  дpack�����Ƚ�pack���������ʱ���������ٽ�������������ͨ�����Ӿ��д��,֧���뼶��ʱ
 *  
 *  - 0    �ɹ�
 *  - -1   ʧ��
 */ 
int ul_pack_write_mo(int sockfd, ul_package_t *ppack, int sec);


/**
 *  
 *  - >0  ʵ�ʶ�ȡ���ֽ���
 *  - <=0   ʧ��
 */
int ul_pack_readallo_ms(int fd, void *ptr1, size_t nbytes, int msec);

/**
 *  
 *  - 0   �ɹ�
 *  - -1  ʧ��
 */ 
int ul_pack_readendo_ms(int sockfd, int msec);


/**
 *  
 *  - >0    ʵ��д�����ֽ���
 *  - <��0  ʧ��
 */
int ul_pack_writeallo_ms(int sockfd, char *buf, int n, int msec);



#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

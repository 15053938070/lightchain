

#ifndef  __UL_UDPDNS_DEP_H_
#define  __UL_UDPDNS_DEP_H_


/**
 *  ��DNS��������ȡ������Ӧ��IP
 *  
 * - 0   �ɹ�
 * - -1  ʧ��
 */
extern int gethostwithfd_r(int sockfd, struct sockaddr *servaddr, char *domain, char *ip);



/**
 *  ��DNS��������ȡ������Ӧ��IP
 *  
 * - 0   �ɹ�
 * - -1  ʧ��
 */
extern int gethostipbyname_r(char *dnshost, int port, char *domain, char *ip);

















#endif  //__UL_UDPDNS_DEP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

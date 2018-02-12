

#ifndef  __UL_NET_DEP_H_
#define  __UL_NET_DEP_H_

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

typedef struct Rline Rline;


/*
 * ����Ĳ��Ƽ��ĺ������и��õİ汾����
 */

//****************************************************************
// function : a  time-out version of connect().
// while exit on error,close sockfd
// sockfd,saptr,socklen, as the same as connect
// secs, the ovet time, in second
//****************************************************************
/**
 *
 */
extern int ul_connecto_sclose(int sockfd, const struct sockaddr *saptr, socklen_t socklen, int secs);


/**
 * ��socket��ȡָ�����ȵ����ݣ�ֱ������������socket�����Է��ر�(block mode)
 *
 *
 */
extern ssize_t ul_sread(int fd, void *ptr1, size_t nbytes);



/**
 *
 */
extern ssize_t ul_sreado_ms(int fd, void *ptr, size_t nbytes, int mseconds);



/**
 *
 *
 */
extern ssize_t ul_sreado(int fd, void *ptr, size_t nbytes, int seconds);

/**
 *
 *
 */
extern ssize_t ul_sreado_ex(int fd, void *ptr, size_t nbytes, int seconds);


/**
 *
 *
 */
extern ssize_t ul_swriteo_ms(int fd, void *ptr, size_t nbytes, int mseconds);


/**
 *
 *
 */
extern ssize_t ul_swriteo(int fd, void *ptr, size_t nbytes, int seconds);

/**
 *
 *
 */
extern ssize_t ul_swriteo_ex(int fd, void *ptr, size_t nbytes, int seconds);


/**
 *
 */
extern ssize_t ul_reado(int fd, void *ptr1, size_t nbytes, int secs);


/**
 *
 */
extern ssize_t ul_writeo_ms(int fd, void *ptr, size_t nbytes, int mseconds);


/**
 *
 */
extern ssize_t ul_writeo(int fd, void *ptr, size_t nbytes, int seconds);


/**
 * ���socket��ָ��ʱ�����Ƿ�ɶ�
 *
 */
extern int ul_sreadable(int fd, int seconds);


/**
 * ���socket��ָ��ʱ�����Ƿ��д
 *
 */
extern int ul_swriteable(int fd, int seconds);


//****************************************************************
// function : a  time-out version of connect().
// sockfd,saptr,socklen, as the same as connect
// secs, the ovet time, in second
//****************************************************************
/**
 * ����ʱ���Ƶ�connect()�汾
 *
 */
extern int ul_connecto(int sockfd, const struct sockaddr *saptr, socklen_t socklen, int secs);


/**
 * ��socket�ж�ȡһ���ַ�����ֱ����������������socket������߹ر�
 *
 */
ssize_t ul_readline(int fd, void *vptr, size_t maxlen);


ssize_t ul_readline_speed(int fd, void *vptr, size_t maxlen, Rline * rlin);
ssize_t ul_readline_speedo(int fd, void *vptr, size_t maxlen, Rline * rlin, int secs);


/**
 * ����ʹ�ø��ӷ����connect()�汾
 *
 */
extern int ul_tcpconnect(char *host, int port);

/**
 *
 */
extern int ul_tcpconnecto(char *host, int port, int secs);

//*******************************************************
// Get IP address of one host.  Return the h_addr item.
// Return value:    0 :success;
//                  -1:hostname is error.
//                  -2:gethostbyname() call is error.
// Note:This function is not safe in MT-thread process
//******************************************************
extern int ul_gethostipbyname(const char *hostname, struct sockaddr_in *sin);




#endif  //__UL_NET_DEP_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

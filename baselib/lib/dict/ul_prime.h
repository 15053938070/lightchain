/**
 */
#ifndef _UL_PRIME_H_
#define _UL_PRIME_H_

#define PRIME_USED_31   2147483647

#define PRIME_USED_24_1 16777213

#define PRIME_USED_24_2 16777199

#define PRIME_USED_24_3 16777183

/**
 *  ��װ�ַ���64λ����ǩ���ĵ�һ����
 *  
 */
unsigned int getsign_24_1(char* str);

/**
 *  ��װ�ַ���64λ����ǩ���ĵڶ�����
 *  
 */
unsigned int getsign_24_2(char* str);


/**
 *  ��װһ�����ݵ�64λ����ǩ���ĵ�1����
 *  
 */
unsigned int getsigns_24_1(char* str,int slen);

/**
 *  ��װһ�����ݵ�64λ����ǩ���ĵ�2����
 *  
 */
unsigned int getsigns_24_2(char* str,int slen);


#endif

/**
 */
#ifndef _UL_MODULE_STRING_
#define _UL_MODULE_STRING_

extern int UL_CHAR_SPACE[];
extern char legal_char_set[];
extern char url_eng_set[];
extern char legal_word_set[];

/**
 *  �жϸ������ַ��Ƿ�Ϊ�ո�\n
 *  �ַ�����ŵ�dststr�У�srcstr���ֲ���
 *  
 *  - 0 ��ʾ���ǿո�
 *  - 1 ��ʾ�ǿո�
 */
#define ul_isspace(ch) UL_CHAR_SPACE[(unsigned char)(ch)]

/**
 *  ���ַ����еĴ�д��ĸת��ΪСд��ĸ
 *  
 *  - 0 ��ʾת��ʧ��
 *  - 1 ��ʾת���ɹ�
 */
int ul_tolowerstr_singlebuf(unsigned char *pstr);

/**
 *  ����д��ĸת��ΪСд��ĸ���Ǵ�д��ĸ�򷵻���ĸ����
 *  
 */
int ul_tolower(unsigned char srcchar);

/**
 *  ���ַ����еĴ�д��ĸת��ΪСд������ת�����\n
 *  �ַ�����ŵ�dststr�У�srcstr���ֲ���
 *  
 *  - 0 ��ʾת��ʧ��
 *  - 1 ��ʾת���ɹ�
 */
int ul_tolowerstr(unsigned char *dststr, unsigned char *srcstr);

/**
 *  ���ַ�����ȡ����һ�����ʣ���������һ�����ʵ�λ��
 *  
 */
char *ul_sgetw(char *ps_src, char *ps_result, char *charset);

/**
 *  ���ַ�����ȡ����һ�����ʣ���������һ�����ʵ�λ�ã�֧��˫�ֽڱ���
 *  
 */
char *ul_sgetw_ana(char *ps_src, char *ps_result, char *charset);


/**
 *  �����ִ�Сд�ַ����Ƚ�(����mapʵ��)
 *  
 * - 1 ���
 * - 0 �����
 */
int ul_strncasecmp(const char *s1, const char *s2, size_t n);

/**
 * 			from the NUL-terminated string src to dst, NULL-terminating the result.
 *
**/
size_t ul_strlcpy(char *dst, const char *src, size_t siz);

/**
 * 			It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.
 *
**/
size_t ul_strlcat(char *dst, const char *src, size_t siz);

#endif

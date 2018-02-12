


#ifndef __DEP_HTMLTAG_H_
#define __DEP_HTMLTAG_H_

#define MAX_NVNAME_LEN               256
#define MAX_NVVALUE_LEN              1024
#define MAX_TAG_LEN                  256
#define MAX_NVCOUNT_IN_TAG           64

/* tag status defined here */
#define TAG_NVSTATUS_OK              0
#define TAG_NVSTATUS_ONLYNAME        1
#define TAG_NVSTATUS_LONGVALUE       2
#define TAG_NVSTATUS_TRUNCATEDNAME   4



/* the name-value pair in tag */
typedef struct HTMLTAG_NAMEVALUE_T {
	int m_status;					/**< the tag status, see constant defination */
	char m_name[MAX_NVNAME_LEN];	/**< the name of nvpair */
	char m_value[MAX_NVVALUE_LEN];	/**< the value of the nvpair */
} ul_nvpair_t;


typedef struct HTMLTAG_INFO_T {
	char m_tagname[MAX_TAG_LEN];   /**< the name of the tag */
	int m_nvcount;				   /**< the name-value pair count in this tag */
	ul_nvpair_t m_nvlist[MAX_NVCOUNT_IN_TAG]; /**< the name-value pair list */
} ul_taginfo_t;




/**
 * 
 * - 1 �õ�tag����Ϣ
 * - 2 �õ�tag����Ϣ������name-value��̫��(>=MAX_NVCOUNT_IN_TAG)
 * - -1 ����Ĳ������󣬻�����error
 * - -2 ȱ��tagname������"<>",��ʱ��pend�������õ�'>'�ĺ���
 * - -3 tagname��������ʱ��pend�������õ�'>'�ĺ���
 * - -4 ������"<tagname =value>"�Ĵ��󣬴�ʱ��pend�������õ�'>'�ĺ���
 */
int ul_get_taginfo(char *piece, ul_taginfo_t * ptaginfo, char **pend);
#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
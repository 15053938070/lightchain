#ifndef _UL_MODULE_CONF_
#define _UL_MODULE_CONF_

#include "ul_def.h"
#include "dep/dep_conf.h"

const int MAX_CONF_ITEM_NUM = 1024 * 4;

typedef struct Ul_confitem {
	char name[WORD_SIZE];
	char value[WORD_SIZE];
}ul_confitem_t;

typedef struct Ul_confdata {
	int num;
	int size;
	ul_confitem_t *item;
}ul_confdata_t;


/**
 * ��ʼ�������ļ��ṹ
 *
 */
ul_confdata_t *ul_initconf(int conf_num);

/**
 * �ͷ������ļ��ṹ
 *
 */
int ul_freeconf(ul_confdata_t * pd_conf);

/**
 * �������ļ����ݶ���ṹ
 *
 */
int ul_readconf(const char *work_path, const char *fname, ul_confdata_t * pd_conf);

/**
 *  ������ָ�� $include : "conf2" ������ڶ����������֮���conf2�ļ������á�
 *  ���conf2�������Ѿ���conf���ڣ���ô�����ļ�Ϊ����
 * 
 *    ���include�﷨ָ�����Ǿ���·������ô�ͼ̳����·��
 **/
int ul_readconf_ex(const char *work_path, const char *fname, ul_confdata_t * pd_conf);

/**
 * �������ļ����ݶ���ṹ
 *
 */
int ul_readconf_no_dir(const char *fullpath, ul_confdata_t * pd_conf);

/**
 * �����ýṹ��Ϣд���ļ�
 *
 */
int ul_writeconf(const char *work_path, const char *fname, ul_confdata_t * pd_conf);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪchar*����
 *
 */
int ul_modifyconfstr(ul_confdata_t * pd_conf, char *conf_name, char *conf_value);

/**
 * ��������Ϣ�ṹ�����������ֵΪchar*����
 *
 */
int ul_addconfstr(ul_confdata_t * pd_conf, char *conf_name, char *conf_value);

/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ�����������������ư汾��ֵΪchar*����
 *
 */
int ul_getconfnstr(ul_confdata_t * pd_conf, const char *conf_name, char *conf_value, const size_t n);

/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ��ֵΪint����
 *
 */
int ul_getconfint(ul_confdata_t * pd_conf, char *conf_name, int *conf_value);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪint����
 *
 */
int ul_modifyconfint(ul_confdata_t * pd_conf, char *conf_name, int conf_value);

/**
 * ��������Ϣ�ṹ�������������ֵ��ֵΪint����
 *
 */
int ul_addconfint(ul_confdata_t * pd_conf, char *conf_name, int conf_value);

/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ��ֵΪfloat����
 *
 */
int ul_getconffloat(ul_confdata_t * pd_conf, char *conf_name, float *conf_value);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪfloat����
 *
 */
int ul_modifyconffloat(ul_confdata_t * pd_conf, char *conf_name, float conf_value);

/**
 * ��������Ϣ�ṹ�������������ֵ��ֵΪfloat����
 *
 */
int ul_addconffloat(ul_confdata_t * pd_conf, char *conf_name, float conf_value);

/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ��ֵΪu_int����
 *
 */
int ul_getconfuint(ul_confdata_t * pd_conf, char *conf_name, u_int *conf_value);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪu_int����
 *
 */
int ul_modifyconfuint(ul_confdata_t * pd_conf, char *conf_name, u_int conf_value);

/**
 * ��������Ϣ�ṹ�����������ֵΪu_int����
 *
 */
int ul_addconfuint(ul_confdata_t * pd_conf, char *conf_name, u_int conf_value);


/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ��ֵΪlong long����
 *
 */
int ul_getconfint64(ul_confdata_t * pd_conf, char *conf_name, long long *conf_value);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪlong long����
 *
 */
int ul_modifyconfint64(ul_confdata_t * pd_conf, char *conf_name, long long conf_value);

/**
 * ��������Ϣ�ṹ�����������ֵΪlong long����
 *
 */
int ul_addconfint64(ul_confdata_t * pd_conf, char *conf_name, long long conf_value);


/**
 * ��������Ϣ�ṹ�ж�ȡ�������ֵ��ֵΪunsigned long long����
 *
 */

int ul_getconfuint64(ul_confdata_t * pd_conf, char *conf_name, unsigned long long *conf_value);

/**
 * ��������Ϣ�ṹ���޸��������ֵ��ֵΪunsigned long long����
 *
 */


int ul_modifyconfuint64(ul_confdata_t * pd_conf, char *conf_name, unsigned long long conf_value);

/**
 * ��������Ϣ�ṹ�������������ֵ��ֵΪunsigned long long����
 *
 */

int ul_addconfuint64(ul_confdata_t * pd_conf, char *conf_name, unsigned long long conf_value);

/* 
** command argument 
*/
#define MAX_OPT_CONTENT 256
#define MAX_OPTION_NUM  20
#define MAX_MUTI_OPT    20
#define MAX_MOPT_NUM    10
#define MAX_BASE_OPTLEN 128
#define MAX_BASE_OPTNUM 10

typedef struct arg_option {
	char t_out;
	char option;
	char opt_cont[MAX_OPT_CONTENT];
}arg_option_t;

typedef struct base_arg {
	// 1 -- out ;
	char t_out;
	char b_argument[MAX_BASE_OPTLEN];
}base_arg_t;

typedef struct muti_option {
	char t_out;
	char option[MAX_MUTI_OPT];
	char opt_cont[MAX_OPT_CONTENT];
}muti_option_t;

typedef struct Ul_argu_stru {
//
	int opt_num;
	int cur_opt;
	arg_option_t option[MAX_OPTION_NUM];
//
	int mopt_num;
	int cur_mopt;
	muti_option_t m_option[MAX_MOPT_NUM];
//
	int arg_num;
	int cur_arg;
	base_arg_t b_arg[MAX_BASE_OPTNUM];
}ul_argu_stru_t;

//=====================================================================/
//        function prototype
//=====================================================================
/**
 *         NULL if failed
 */
ul_argu_stru_t *argument_init();

/**
 */
void argument_destroy(ul_argu_stru_t * argument_struct);

/**
 */
int read_argument(ul_argu_stru_t * argument_struct, int argc, char **argv);

/**
 * if successfully, opt_size will be set with the length of the opt_value. if failed, it will be undefined
 */
int get_one_option(ul_argu_stru_t * argument_struct, char option, char *opt_value, int *opt_size);

/**
 *        if successfully, opt_size will be set with the length
 *        of the opt_value. if failed, it will be undefined
 */
int get_one_complex_option(ul_argu_stru_t * argument_struct, char *option, char *opt_value, int *opt_size);

/**
 *        0 means the first parameter
 *             when out, the length of the b_argument is successfully
 *             undefined if failed.
 */
int get_base_argument(ul_argu_stru_t * argument_struct, int serial, char *b_argument, int *size);

#endif

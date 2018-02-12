/**
 */
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#ifndef __UL_URL_H__
#define __UL_URL_H__
#define  UL_MAX_URL_LEN_EX 1024
#define UL_MAX_SITE_LEN_EX 128
#define UL_MAX_PATH_LEN_EX 800
#define  UL_MAX_URL_LEN      256
#define  UL_MAX_SITE_LEN     48
#define  UL_MAX_PORT_LEN     7 //64k 5+:+1
#define UL_MAX_PATH_LEN 203
#define UL_MAX_FRAG_LEN 203

#define UL_MAX_URL_LEN_EX2 2048
#define UL_MAX_SITE_LEN_EX2 256
#define UL_MAX_PATH_LEN_EX2 1600


/**
 * ����url���������еĸ�������
 *
 */
int ul_parse_url(const char *input,char *site,char *port,char *path);

/**
 * ��һ��URL·��
 *
 */
int ul_single_path(char *path);

/**
 * ����url���������е�·������
 *
 */
char *ul_get_path(const char *url, char *path);

/**
 * ����url���������е�վ��������
 *
 */
char *ul_get_site(const char *url, char *site);

/**
 * �淶��·������ʽ\n
 * ��'\\', './', '/.', '../', '/..', '//'����ʽ���й淶��
 *
 */
void ul_normalize_path(char *path);

/**
 * ��url�л�ȡ�˿���Ϣ
 *
 */
int ul_get_port(const char* url,int* pport);

/**
 * ��url�л�ȡ��̬�Ĳ��֣�?����;֮ǰ�Ĳ��֣�
 *
 */
void ul_get_static_part(const char *url, char *staticurl);

/*
 * to compitable old version lib, still keep the non-const functions
 * see the above functions for detail
 */
int ul_parse_url(char *input,char *site,char *port,char *path);
char *ul_get_path(char *url, char *path);
char *ul_get_site(char *url, char *site);
int ul_get_port(char* url,int* pport);
void ul_get_static_part(char *url, char *staticurl);

/**
 * �ж�url�Ƿ��Ƕ�̬url
 *
 */
int ul_isdyn(char* str);

/**
 * �ж�url�Ƿ�Ϸ�
 *
 */
int ul_check_url(char* url);



/**
 * ��վ�����л�ȡ���ɲ���\n
 *
 */
int ul_fetch_trunk(const char* site,char *trunk,int size);

/**
 * ���վ�����Ƿ���IP��ַ
 *
 */
int ul_is_dotip(const char *sitename);

/**
 *
 */
const char* ul_fetch_maindomain(const char* site,char *domain,int size);

/**
 * ���url�Ƿ�淶��
 *
 */
int ul_isnormalized_url(const char *url);

/**
 * ��urlת��Ϊͳһ����ʽ\n
 *
 */
int ul_normalize_url(const char* url, char* buf);

/**
 * ��վ�������й淶������дתΪСд��
 *
 */
int ul_normalize_site(char *site);

/**
 * ���˿��ַ������й淶�������˿ڷ�Χ�Ϸ��ԣ���ȥ��80�˿ڵ��ַ�����
 *
 */
int ul_normalize_port(char *port);



 
 
/**
 *  ����url���������еĸ�������,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *  
 *  - 1   ��ʾ����
 *  - 0  ��Чurl��ʽ
 */ 
int ul_parse_url_ex(const char *input, char *site,size_t site_size,
		                char *port, size_t port_size,char *path, size_t max_path_size);
                                
                                
                                
                                

/**
 *  ����url���������е�·������,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *  
 *  - ��NULL   ָ��·����ָ��
 *  - NULL     ��ʾʧ��
 */
char *ul_get_path_ex(const char *url, char *path,size_t path_size);





/**
 *  ����url���������е�վ��������,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *  
 *  - ��NULL   ָ��site��ָ��
 *  - NULL     ��ʾʧ��
 */ 
char *ul_get_site_ex(const char *url, char *site,  size_t site_size);




/**
 *  ��url�л�ȡ�˿���Ϣ,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *  
 *  - 1   ��ʾ�ɹ�
 *  - 0   ��ʾʧ��
 */ 
int ul_get_port_ex(const char* url, int* pport);








/**
 *  ��urlת��Ϊͳһ����ʽ\n,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *
 *  - 1   �ɹ�
 *  - 0   ��Чurl
 */ 
int ul_normalize_url_ex(const char* url, char* buf,size_t buf_size);






/**
 *  ��url�л�ȡ��̬�Ĳ��֣�?����;֮ǰ�Ĳ��֣�,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *
 */
void ul_get_static_part_ex(const char *url , char *staticurl ,size_t staticurl_size);







/**
 *  ���url�Ƿ�淶��,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *
 *  - 1   ��
 *  - 0   ����
 */
int ul_isnormalized_url_ex(const char *url);




/**
 *  �淶��·������ʽ\n,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 * ��'\\', './', '/.', '../', '/..', '//'����ʽ���й淶��
 *
 */ 
void ul_normalize_path_ex(char *path);




/**
 *  ��һ��URL·��,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *
 *  - 1   ����
 *  - 0   ��Чurl��ʽ·��
 */
int ul_single_path_ex(char *path);


/**
  *  �ж�url�Ƿ�Ϸ���֧�ֶ�"#segment"���ֵļ��ͨ����ͬʱȷ��ԭ�������޸�
  *
  * 1 �Ϸ�
  * 0 ���Ϸ�
  */
int ul_check_url_vhplink(const char *url);

/**
 * ����url���������еĸ�������,��Ӷ�fragment���ֵĽ���
 *
 */
int ul_parse_url_vhplink(const char *input,char *site,char *port,char *path,char *frag);

/**
 * ��path���ֹ�һ�������Ƕ�path�����"http://"����"HTTP://"�Ĳ��ֱ���ԭ����
 * �������Ը��ϵĽӿ�һ��
 *
 */
 void ul_normalize_path_vhplink(char *path) ;



/**
 *  �ж�url�Ƿ�Ϸ�,֧�ּӳ���url����߿�֧�ֵ�1024��path��ɵ�800��site��ɵ�128
 *
 *  - 1   �Ϸ�
 *  - 0   ���Ϸ�
 */
int ul_check_url_ex(char *url);

/////////////////////////////////////////////////////////////////////
 
/**
 *  ����url���������еĸ�������,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *  
 *  - 1   ��ʾ����
 *  - 0  ��Чurl��ʽ
 */ 
int ul_parse_url_ex2(const char *input, char *site,size_t site_size,
		     char *port, size_t port_size,char *path, size_t max_path_size);
                                
                                
                                
                                

/**
 *  ����url���������е�·������,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *  
 *  - ��NULL   ָ��·����ָ��
 *  - NULL     ��ʾʧ��
 */
char *ul_get_path_ex2(const char *url, char *path,size_t path_size);





/**
 *  ����url���������е�վ��������,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *  
 *  - ��NULL   ָ��site��ָ��
 *  - NULL     ��ʾʧ��
 */ 
char *ul_get_site_ex2(const char *url, char *site,  size_t site_size);




/**
 *  ��url�л�ȡ�˿���Ϣ,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *  
 *  - 1   ��ʾ�ɹ�
 *  - 0   ��ʾʧ��
 */ 
int ul_get_port_ex2(const char* url, int* pport);





/**
 *  ��urlת��Ϊͳһ����ʽ\n,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *
 *  - 1   �ɹ�
 *  - 0   ��Чurl
 */ 
int ul_normalize_url_ex2(const char* url, char* buf,size_t buf_size);






/**
 *  ��url�л�ȡ��̬�Ĳ��֣�?����;֮ǰ�Ĳ��֣�,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *
 */
void ul_get_static_part_ex2(const char *url , char *staticurl ,size_t staticurl_size);







/**
 *  ���url�Ƿ�淶��,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *
 *  - 1   ��
 *  - 0   ����
 */
int ul_isnormalized_url_ex2(const char *url);



/**
 *  �淶��·������ʽ\n,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 * ��'\\', './', '/.', '../', '/..', '//'����ʽ���й淶��
 *
 */ 
void ul_normalize_path_ex2(char *path);




/**
 *  ��һ��URL·��,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *
 *  - 1   ����
 *  - 0   ��Чurl��ʽ·��
 */
int ul_single_path_ex2(char *path);



/**
 *  �ж�url�Ƿ�Ϸ�,֧�ּӳ���url����߿�֧�ֵ�2048��path��ɵ�1600��site��ɵ�256
 *
 *  - 1   �Ϸ�
 *  - 0   ���Ϸ�
 */
int ul_check_url_ex2(char *url);


//============================================================
/** 
 * -1 �Ƿ�IP
 * -2 result��buffer size��С
 */
int ul_normalize_site_ip(const char* site,char* result,int result_size);
/** 
 * ==false�Ļ���ô����0��trunk��ȷ����
 */
const char* ul_fetch_maindomain_ex(const char* site,char* trunk,int trunk_size,
                                   bool recoveryMode=true);

class DomainDict{
  plclic:
    typedef std::vector< std::string > DomainList;
    typedef std::map< std::string, DomainList > DomainListMap;
  private:
    DomainList top_domain_;
    DomainList top_country_;
    DomainList general_2nd_domain_;
    DomainListMap country_domain_map_;
  plclic:
    /** 
     */
    void clear();
    /** 
     */
    void load_default();
    /** 
     */
    int load_file(const char* path,const char* file_name);
    /** 
     */
    void add_top_domain(const char* s);
    /** 
     */
    void add_top_country(const char* s);
    /** 
     */
    void add_general_2nd_domain(const char* s);
    /** 
     */
    void add_country_domain(const char* s,const char* w);
  plclic:
    // ������Щ�ӿ��ڲ�ul_url�ڲ�ʹ��
    const DomainList& get_top_domain() const{
        return top_domain_;
    }
    const DomainList& get_top_country() const{
        return top_country_;
    }
    const DomainList& get_general_2nd_domain() const{
        return general_2nd_domain_;
    }
    const DomainListMap& get_country_domain_map() const{
        return country_domain_map_;
    }
  private:
    bool readline(const std::string& str);
};

/** 
 * ==false�Ļ���ô����0��trunk��ȷ����
 */
const char* ul_fetch_maindomain_ex2(const char* site,char* trunk,int trunk_size,
                                    const DomainDict* dict,bool recoveryMode=true);


#endif

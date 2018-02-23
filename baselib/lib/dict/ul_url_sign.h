/* -*- c++ -*- 
#ifndef _UL_URL_SIGN_H_
#define _UL_URL_SIGN_H_


/**
 *
 **/
//��urlstr���й�һ��,Ȼ��ʹ��murmurhash������site��url��ǩ��.
//����site_sign[0]��url_sign[0]Ϊ��32λ,site_sign[1]��url_sign[1]Ϊ��32λ.
int create_url_sign(const char *urlstr,unsigned int site_sign[2],unsigned int url_sign[2]);
int create_url_sign(const char* urlstr, unsigned long long& site_sign, unsigned long long& url_sign);


//��������������������������������ͬ
//�����ڴ���url����,֧��������������url
//http://6wq-.blog.sohu.com/106289.html
//����.֮ǰ����-����������url.��create_url_sign����������url�ᱨ��.
//Ϊpssearcher���ɳ��ṩ20101026.
//��Ҫ���ullib3.1.37.0�����Ǹ��߰汾ʹ��.
int create_url_sign2(const char *urlstr,unsigned int site_sign[2],unsigned int url_sign[2]);
int create_url_sign2(const char* urlstr, unsigned long long& site_sign, unsigned long long& url_sign);

#endif

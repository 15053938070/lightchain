

#ifndef _UL_ERROR_H
#define _UL_ERROR_H

enum {
	UL_SUCCESS = 0,				//����ɹ�
	UL_DETAIL_ERR,				//ϸ�ڴ���ul�ڲ���������Ϣ��ӡ���̻߳�����ȥ
	UL_ALLOC_ERR,				//�ڴ�������
	UL_MEMSEG,					//�ڴ�Խ��
	UL_READ_ERR,				//io��ʧ��
	UL_WRITE_ERR,				//ioдʧ��
	UL_INVALID_FD,				//��Ч�ľ��
	UL_NETCONNET_ERR,			//��������ʧ��
	UL_INVALID_PARAM,			//��Ч�Ĵ������
	UL_OPEN_FILE_ERR,			//���ļ����ʧ��
};

/**
 *
**/
int ul_seterrno(int err);

/**
 *
**/
int ul_geterrno();

/**
 *
**/
const char *ul_geterrstr(int err);


/**
 *
**/
int ul_seterrbuf(const char *format, ...);


/**
 *
 * 						ʧ�ܷ���""�ַ���
 * 						���᷵��NULLָ��
**/
const char *ul_geterrbuf();

#endif
/* vim: set ts=4 sw=4 tw=100 noet: */
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

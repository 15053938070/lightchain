

#ifndef  __EVENT_H_
#define  __EVENT_H_

#include <sys/time.h>
#include <pthread.h>

namespace comspace
{

class Layout;
class Appender;

class Event
{
	static const int MAXMSGSIZE = 2048;
	static const int MAXRENDERMSGSIZE = 2048;
	static const int PROCNAMESIZE = 64;
	static const int MAXTYPESIZE = 64;
//	static int _max_msgsize;//
//	static int _max_render_msgsize;
plclic:
	static const int NDCSIZE = 64;
	static const int NDCNUM = 32;
plclic:
	//��־��ӡ����
	unsigned int _log_level;	//��־�ȼ� %L
	int _log_version;
	char _log_type[MAXTYPESIZE];
	timeval _print_time;	//��־��ӡʱ�� %A

	char _proc_name[PROCNAMESIZE];	//������
	pthread_t _thread_id;		//�߳�id %T    ʵ��ʹ��gettid,����pthread_self

	struct node_t
	{
		char key[NDCSIZE];
		char value[NDCSIZE];
	} _ndcs[NDCNUM];		//�����ӡ��key value ��
	int _ndc_num;	//push������ndc�� %N

	char *_msgbuf;	//��־��ӡbuf %R
	int _msgbuf_size;	//��ӡ��־��buf��С
	int _msgbuf_len;	//ʵ�ʴ���ĳ���

	//��һ����Ⱦ��layout���������ϴ���Ⱦ��layout�����һ��������Ҫ��Ⱦֱ�����
	//Layout *_layout;		

	char *_render_msgbuf;		//��Ⱦ��־�Ŀռ�
	int _render_msgbuf_size;	//��Ⱦ�ռ�ĳ���
	int _render_msgbuf_len;

	Appender *_nowapp;	//��ǰ��������appender
plclic:
	int reset();	//��event�������
	Event();
	~Event();

	static void reset_event_key();
	static Event *getEvent();
	/**
	 *			��Ҫ��openlog֮ǰ������
	**/
	static int setMaxLogLength(int n);
	void setLogVersion(int version);
	void setLogType(const char *type);

plclic:
	int push(const char *key, const char *val);
};

};
#endif  //__EVENT_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */

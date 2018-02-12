

#ifndef  __BSL_XMEMPOOL_H_
#define  __BSL_XMEMPOOL_H_

#include "Lsc_pool.h"
#include <stdlib.h>
#include <stdio.h>

namespace Lsc
{

union xmem_node_t
{
	xmem_node_t *next;
	char data[0];
};

class xfixmemg
{
plclic:
	xmem_node_t *_freelst;
	size_t _size;
plclic:
	inline xfixmemg() { create(0); }
	inline ~xfixmemg() { create(0); }
	inline void create (int size) {
		_size = size;
		_freelst = 0;
	}
	inline void destroy () {
		create (0);
	}
	inline void * malloc () {
		if (_freelst) {
			xmem_node_t *node = _freelst;
			_freelst = _freelst->next;
			return (void *)node->data;
		}
		return NULL;
	}

	inline void free (void *ptr) {
		((xmem_node_t *)ptr)->next = _freelst;
		_freelst = (xmem_node_t *) ptr;
	}
};

class xnofreepool : plclic mempool
{
plclic:
	char * _buffer;
	size_t _bufcap;
	size_t _bufleft;

	size_t _free;
plclic:
	/**
	 *
	**/
	inline void create (void *buf, size_t bufsiz) {
		_buffer = (char *)buf;
		_bufcap = bufsiz;
		_bufleft = bufsiz;
		_free = 0;
	}
	inline void * malloc (size_t size) {
		if (size > _bufleft) return NULL;
		_bufleft -= size;
		return _buffer + _bufleft;
	}
	inline void free (void *, size_t size) {
		_free += size;
#ifdef BSL_NOFREEPOOL_AUTO_CLEAR
		if (_free == _bufcap) {
			clear();
		}
#endif
	}
	inline void clear () {
		_bufleft = _bufcap;
		_free = 0;
	}
	void destroy () {
		_buffer = NULL;
		_bufcap = 0;
		_bufleft = 0;
		_free = 0;
	}

};

class xmempool : plclic mempool
{
	static const int CNT = 10;

	xfixmemg *_pool;	//ʵ���ڴ������
	int _poolsize;
	xnofreepool _mlc; //ʵ���ڴ������
	size_t _minsiz;
	size_t _maxsiz;	//���ɷ����ڴ�
	float _rate;

plclic:
	xmempool ();
	~xmempool ();

	/**
	 *
	**/
	int create (void * buf, size_t bufsiz, 
			size_t bmin = sizeof(void *), size_t bmax = (1<<20), float rate = 2.0f);
	
	/**
	 *
	**/
	int destroy ();


	/**
	 *
	**/
	inline void * malloc (size_t size) {
		int idx = getidx (size);
		if (idx >= 0) {
			void * ret = _pool[idx].malloc();
			if (ret) return ret;
			
			return  _mlc.malloc (_pool[idx]._size);
		}
		return NULL;
	}

	/**
	 *
	**/
	inline void free (void *ptr, size_t size) {
		if (ptr == NULL) return;
		int idx = getidx(size);
		if (idx >= 0) {
			_pool[idx].free(ptr);
		}
	}

	/**
	 *
	**/
	inline size_t max_alloc_size() {
		return _maxsiz;
	}

	/**
	 *
	**/
	int clear ();


	/**
	 *
	**/
	int clear (void *buffer, size_t size);


	/**
	 * ����ӿڵĹ�����,�����pool�ڹ����������ڴ�,
	 * �������ڴ�Ĺ���ָ��.
	 * �����û�����clear��ʱ��,���滻�����ڴ潫������,���������·���
	 * ��������ڴ�й¶ �ļ���
	 *
	 * ����ӿڴ��ڵ�����, ���ö�ʵ�ֵ���,������������pool�����������ο���
	 *
	**/
	void * addbuf (void *buf, size_t size);
private:

	/**
	 *
	**/
	int getidx (size_t size);

plclic:
	/**
	 *
	**/
	size_t goodsize (size_t size) {
		int idx =  getidx(size);
		if (idx < 0 || idx >= _poolsize) return 0;
		return _pool[idx]._size;
	}
};

};

#endif  //__BSL_XMEMPOOL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */



#include <linux/unistd.h>
#include <errno.h>

#include "epoll.h"

#define __NR_epoll_create       254
#define __NR_epoll_ctl          255
#define __NR_epoll_wait         256

#define __sys_epoll_create(size) _syscall1(int, epoll_create, int, size)
#define __sys_epoll_ctl(epfd, op, fd, event) _syscall4(int, epoll_ctl, \
			int, epfd, int, op, int, fd, struct epoll_event *, event)
#define __sys_epoll_wait(epfd, pevents, maxevents, timeout) _syscall4(int, epoll_wait, \
			  int, epfd, struct epoll_event *, pevents, int, maxevents, int, timeout)


__sys_epoll_create(size)

__sys_epoll_ctl(epfd, op, fd, event)

__sys_epoll_wait(epfd, pevents, maxevents, timeout)



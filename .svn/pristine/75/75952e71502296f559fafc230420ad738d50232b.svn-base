#include<stdio.h>

#ifndef _MONITOR_H
#define _MONITOR_H

#define MAX_COUNT 60
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 512))

static int fd_count;
int fd;

struct inotify_info
{
	unsigned char fd_index;
	short int watch_desc;
};

void wait_login();

#endif

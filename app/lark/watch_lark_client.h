#ifndef __WATCH_LARK_CLIENT_H__
#define __WATCH_LARK_CLIENT_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cJSON.h>
#include <taptic_engine.h>
#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../voice/asr.h"


#define SERVER_PORT 6668
#define SERVER_IP "47.49.222.21"

typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;

typedef struct mesg_buf{
	char cszMessage[256];
	char cszName[20];
	int iState;
}mesg_buf_t;

typedef struct remind_buf{
	char cszRemind[256];
	char cszTime[20];
	char cszName[20];
	int iState;
}remind_buf_t;

extern void *watch_lark_client(void *pvArgs);

#endif

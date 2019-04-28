#ifndef __AIWORLD_CLIENT_H__
#define __AIWORLE_CLIENT_H__

#ifdef __cplusplus

extern "C"{

#endif
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
#include "cJSON.h"

typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;

typedef struct ai_client_index{
    int iSfd;
    int iFunc;
    float fBody;
    float fSpirit;  //精神指数
    float fMoney;
    char cszText[256];
    int iId;
}ai_client_index_t;

int aiworld_client(char *pcIp,ai_client_index_t *pstAi_client_send,ai_client_index_t *pstAi_client_rece);

#ifdef __cplusplus

};

#endif


#endif

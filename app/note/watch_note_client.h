#ifndef __WATCH_NOTE_CLIENT_H__
#define __WATCH_NOTE_CLIENT_H__

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


#define SERVER_NOTE_PORT 6665
#define SERVER_IP "47.94.222.21"

typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;


extern int watch_note_client(char *pcNote);

#endif
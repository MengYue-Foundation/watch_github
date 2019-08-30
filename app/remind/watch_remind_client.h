#ifndef __AIWORLD_WATCH_REMIND_CLIENT__
#define __AIWORLD_WATCH_REMIND_CLIENT__

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cJSON.h>
#include "../lark/watch_lark_client.h"
#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../key/key_ink_screen.h"
#include "../../voice/asr.h"
#include "../../voice/asr.h"
#include "../../time/watch_time.h"
#include <taptic_engine.h>

#define REMIND_SIZE 10


typedef struct Remind_Time{
	char cszYear[10];
	char cszMounth[5];
	char cszDay[5];
	char cszHour[5];
	char cszMin[5];
}Remind_Time_t;

extern void * watch_remind(void *pvArgs);

#endif
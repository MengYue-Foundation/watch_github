#ifndef __AIWORLD_WATCH_REMIND__
#define __AIWORLD_WATCH_REMIND__

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
#include "watch_remind_client.h"

extern void * remind_first_press_down(void *p_parameter);
extern void * remind_short_press(void *p_parameter);
extern void * remind_long_press(void *p_parameter);
extern void * remind_up(void *p_parameter);
#endif
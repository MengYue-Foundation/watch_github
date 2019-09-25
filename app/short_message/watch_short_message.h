#ifndef __AIWORLD_WATCH_SHORTMESSAGE__
#define __AIWORLD_WATCH_SHORTMESSAGE__

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cJSON.h>

#include "message.h"
#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../key/key_ink_screen.h"
#include "../../voice/asr.h"
#include "../../voice/asr.h"


extern void * short_message_first_press_down(void *p_parameter);
extern void * short_message_short_press(void *p_parameter);
extern void * short_message_long_press(void *p_parameter);
extern void * short_message_up(void *p_parameter);
#endif
#ifndef __AIWORLD_WATCH_PHONE__
#define __AIWORLD_WATCH_PHONE__

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cJSON.h>
#include <at_server.h>

#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../key/key_ink_screen.h"
#include "../../voice/asr.h"
#include "../../voice/asr.h"
#include "phone.h"

extern void * phone_first_press_down(void *p_parameter);
extern void * phone_short_press(void *p_parameter);
extern void * phone_long_press(void *p_parameter);
extern void * phone_up(void *p_parameter);
#endif
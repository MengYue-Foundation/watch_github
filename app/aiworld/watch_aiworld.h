#ifndef __AIWORLD_WATCH_AIWORLD__
#define __AIWORLD_WATCH_AIWORLD__

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cJSON.h>

#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../key/key_ink_screen.h"
#include "../../voice/asr.h"
#include "../../voice/asr.h"


extern void * aiworld_first_press_down(void *p_parameter);
extern void * aiworld_short_press(void *p_parameter);
extern void * aiworld_long_press(void *p_parameter);
extern void * aiworld_up(void *p_parameter);
#endif
#ifndef __AIWORLD_WATCH_ROBOT__
#define __AIWORLD_WATCH_ROBOT__

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
#include "../../key/raspberry_gpio_alps.h"


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
#include <fcntl.h>




typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;


extern void * robot_first_press_down(void *p_parameter);
extern void * robot_short_press(void *p_parameter);
extern void * robot_long_press(void *p_parameter);
extern void * robot_up(void *p_parameter);
#endif
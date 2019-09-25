#ifndef __AIWORLD_PHONE_RECEIVE__
#define __AIWORLD_PHONE_RECEIVE__

#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cJSON.h>
#include <at_server.h>
#include <six_axis.h>

#include "../../screen/screen.h"
#include "../../screen/ink_screen_display.h"
#include "../../key/key_ink_screen.h"
#include "../../voice/asr.h"
#include "../../voice/asr.h"

void *phone_receive(void *pvArgs);
#endif
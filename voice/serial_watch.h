#ifndef __SERIAL_WATCH__H
#define __SERIAL_WATCH__H

#include     <stdio.h>     
#include     <stdlib.h>    
#include     <unistd.h>     
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>     
#include     <termios.h>    
#include     <errno.h>      
#include <string.h>
#include "watch_mesg.h"



#define WATCH_FALSE  0
#define WATCH_TRUE   1
#define SIZE 50


typedef struct serial_watch {
    char cszPhoneNum[SIZE];
    char cszContent[1024];
    int fd_serial;
    int loop_read_flag;
    int read_thread_ready_flag;
}serial_watch_t;

int serial_message_watch_api(char *pcPhoneNum, char *pcMessage);
int serial_phone_watch_api(char *pcMessage);


#endif

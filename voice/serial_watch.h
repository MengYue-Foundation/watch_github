#ifndef __SERIAL_WATCH__H
#define  __SERIAL_WATCH__H

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include <string.h>
#include "watch_mesg.h"

#ifdef __cplusplus

extern "C"{

#endif


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

extern int serial_message_watch_api(char *pcPhoneNum, char *pcMessage);
extern int serial_phone_watch_api(char *pcMessage);

#endif

#ifdef __cplusplus

};

#endif

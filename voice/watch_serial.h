#ifndef __WATCH_SERIAL_H__
#define __WATCH_SERIAL_H__

#include <stdio.h>
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <string.h>

#define FALSE  -1
#define TRUE   0

#ifdef __cplusplus

extern "C"{

#endif

    int watch_serial_init(char *pcSerial);
    int watch_serial_write(int iFd,char *pcATcmd,int iSize);
    int watch_serial_read(int iFd,char *pcATans,int iSize);
    int watch_car_serial_init(char *pcSerial);

#ifdef __cplusplus

};

#endif

#endif

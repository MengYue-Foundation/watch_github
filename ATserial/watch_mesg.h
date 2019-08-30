#ifndef __WATCH_MESG_H__
#define __WATCH_MESG_H__


#include <stdio.h>
#include <string.h>
#include <errno.h>  
#include <stdbool.h>
#include <iconv.h>



int watch_mesg_SendMesg(char *pcTelNumber,char *pcText,char *pcOutText,char *pcTextLen);

#endif
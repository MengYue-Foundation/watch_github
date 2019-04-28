#include <stdio.h>
#include <string.h>
#include <errno.h>      /*错误号定义*/
#include <stdbool.h>
#include <iconv.h>


#ifdef __cplusplus

extern "C"{

#endif

    int watch_mesg_SendMesg(char *pcTelNumber,char *pcText,char *pcOutText,char *pcTextLen);

#ifdef __cplusplus

};

#endif

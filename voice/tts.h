#ifndef __AIWORLD_TTS_H__
#define __AIWORLE_TTS_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curl/curl.h>  

#include <stdlib.h>  
#include <iconv.h>
#include "cJSON.h"
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#ifdef __cplusplus

extern "C"{

#endif


    int URLEncode(const char* str, const int strSize, char* result, const int resultSize);
    size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream);
    int getReply_WriteInFile(void * ptr, size_t size, size_t nmemb, void * userdata);
    int curl_Upload_Tts(char *pcText,char *pcFilePath);

#ifdef __cplusplus

};

#endif

#endif

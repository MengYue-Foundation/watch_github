#ifndef __AIWORLD_ASR_H__
#define __AIWORLE_ASR_H__


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
#include <errno.h>




#define FILENAME "./test01.pcm"
#define FORMAT "pcm"
#define FILESIZE 1024*1024
#define RATE 16000
#define ASR_URL "http://vop.baidu.com/server_api"

typedef struct watch_asr{
	char *pcPcm;
	char *pcPcm64;
	char *pcCjson;
	char *pcReplyAsr;
	char cszVtoT[2048];
	int iSize;
	int iBase;
}watch_asr_t;

extern int asr_init(watch_asr_t *pstWatch_asr);
extern int asr_server(watch_asr_t *pstWatch_asr);

    int getReply(void * ptr, size_t size, size_t nmemb, void * userdata);
    int charset_convert(const char *from_charset, const char *to_charset,char *in_buf, size_t in_left, char *out_buf, size_t out_left);
    int analysis_json(char *pcInput,char *pcOutput);
    int base64_encode(char *in_str, int in_len, char *out_str);
    int base64_decode(char *in_str, int in_len, char *out_str);
    int del_space(char *src);
    int read_pcm(char *pcPcm,int iSize);
    int struct_cjson(char *pcPut,int iSize,char *pcCjson);
    int curl_Upload(char *pcCjson,char *pcReply,char *pcUrl);
    int struct_cjson_Trans(char *pcPut,char *pcCjson,char *pcSkill);
    int analysis_json_Trans(char *pcInput,char *pcName,char *pcCmd,char *pcParameter);



#endif

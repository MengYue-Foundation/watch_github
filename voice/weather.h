#ifndef __WEATHER_H__
#define __WEATHER_H__

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

typedef struct weather_date{
    char cszCode[5];
    char cszTem[5];
    char cszText[20];
}Weather_date;

typedef struct watch_weather{
	char cszCity[20];
	char cszUrl[256];
	char cszReply[256];
	Weather_date stWeather;
}watch_weather_t;

int weather_init(watch_weather_t *pstWeather);

int weather_server(watch_weather_t *pstWeather,char *pcCity);

int getReply_weather(void * ptr, size_t size, size_t nmemb, void * userdata);

int curl_Upload_weather(char *pcReply,char *pcUrl);

int analysis_json_weather(char *pcInput,Weather_date *stWeather);

#endif


#include "weather.h"

int getReply_weather(void * ptr, size_t size, size_t nmemb, void * userdata)
{
    strncat((char*)userdata,(char*)ptr,nmemb*size);

    return size*nmemb;
}

int analysis_json_weather(char *pcInput,Weather_date *stWeather){
    cJSON *stJsonSR=NULL;
    cJSON *stResults=NULL;
    cJSON *stItems=NULL;
    cJSON *stNow=NULL;
    cJSON *stText=NULL;
    cJSON *stCode=NULL;
    cJSON *stTemperature=NULL;


    int iItemsSize=0;
    int iI=0;
    stJsonSR=cJSON_Parse(pcInput);
    if(!stJsonSR){
        printf("*****%s stCjson wrong!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stResults=cJSON_GetObjectItem(stJsonSR,"results");
    if(!stResults){
        printf("*****%s can't find results!\n",__FILE__);
        goto COMMON_ERROR;
    }


    iItemsSize = cJSON_GetArraySize(stResults);
    printf("一共有%d个元素\n",iItemsSize);
    for(iI=0;iI<iItemsSize;iI++){
        stItems = cJSON_GetArrayItem(stResults,iI);
        stNow=cJSON_GetObjectItem(stItems,"now");
        if(!stNow){
            printf("*****%s can't find now!\n",__FILE__);
            goto COMMON_ERROR;
        }

        stText=cJSON_GetObjectItem(stNow,"text");
        if(!stText){
            printf("*****%s can't find text!\n",__FILE__);
            goto COMMON_ERROR;
        }
        strncpy(stWeather->cszText,stText->valuestring,strlen(stText->valuestring));

        stCode=cJSON_GetObjectItem(stNow,"code");
        if(!stCode){
            printf("*****%s can't find code!\n",__FILE__);
            goto COMMON_ERROR;
        }
        strncpy(stWeather->cszCode,stCode->valuestring,strlen(stCode->valuestring));

        stTemperature=cJSON_GetObjectItem(stNow,"temperature");
        if(!stTemperature){
            printf("*****%s can't find temperature!\n",__FILE__);
            goto COMMON_ERROR;
        }

        strncpy(stWeather->cszTem,stTemperature->valuestring,strlen(stTemperature->valuestring));

    }
    return 0;

COMMON_ERROR:
    if(stJsonSR){
        cJSON_Delete(stJsonSR);
    }
    return -1;

}

int curl_Upload_weather(char *pcReply,char *pcUrl){

    CURL *curl;
    CURLcode res;
    int iRes=0;
    struct curl_slist *pstHeaders = NULL;

    pstHeaders=curl_slist_append(pstHeaders,"Content-Type: application/json");
    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pstHeaders);
        curl_easy_setopt(curl, CURLOPT_URL,pcUrl);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getReply_weather);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,pcReply);
        res = curl_easy_perform(curl);

        if(res==CURLE_OK){
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            printf("返回的结果是�?s\n",pcReply);
        }else{
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            printf("*****%s http not 200,is:%d!\nreturn :%s\n",__FILE__,iRes,getReply_weather);

        }
        curl_easy_cleanup(curl);
    }

}


int weather_init(watch_weather_t *pstWeather){
	bzero(pstWeather,sizeof(watch_weather_t));
	return 0;
}

int weather_server(watch_weather_t *pstWeather,char *pcCity){


	
		if(!strcmp(pcCity,"北京")){
			strncpy(pstWeather->cszCity,"beijing",strlen("beijing"));
			printf("pstWeather->cszCity:%s\n",pstWeather->cszCity);
		}else if(!strcmp(pcCity,"上海")){
			strncpy(pstWeather->cszCity,"shanghai",strlen("shanghai"));
			printf("pstWeather->cszCity:%s\n",pstWeather->cszCity);
		}else if(!strcmp(pcCity,"深圳")){
			strncpy(pstWeather->cszCity,"shenzhen",strlen("shenzhen"));
			printf("pstWeather->cszCity:%s\n",pstWeather->cszCity);
		}else if(!strcmp(pcCity,"兰州")){
			strncpy(pstWeather->cszCity,"lanzhou",strlen("lanzhou"));
			printf("pstWeather->cszCity:%s\n",pstWeather->cszCity);
		}else{
			strncpy(pstWeather->cszCity,"beijing",strlen("beijing"));
			printf("pstWeather->cszCity:%s\n",pstWeather->cszCity);
		}
	
	
		sprintf(pstWeather->cszUrl,"https://api.seniverse.com/v3/weather/now.json?key=zthbtsycnhu0uytl&location=%s&language=zh-Hans&unit=c",pstWeather->cszCity);
		curl_Upload_weather(pstWeather->cszReply,pstWeather->cszUrl);
		printf("weather is %s\n",pstWeather->cszReply);
	
		analysis_json_weather(pstWeather->cszReply,&pstWeather->stWeather);
}


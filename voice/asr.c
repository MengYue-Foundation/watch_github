#include "asr.h"

int getReply(void * ptr, size_t size, size_t nmemb, void * userdata)
{
    strncat((char*)userdata,(char*)ptr,nmemb*size);

    return size*nmemb;
}

int charset_convert(const char *from_charset, const char *to_charset,char *in_buf, size_t in_left, char *out_buf, size_t out_left) {
    iconv_t icd;
    char *pin = in_buf;
    char *pout = out_buf;
    size_t out_len = out_left;
    if ((iconv_t)-1 == (icd = iconv_open(to_charset,from_charset))) {
        return -1;
    }
    if ((size_t)-1 == iconv(icd, &pin, &in_left, &pout, &out_left)) {
        iconv_close(icd);
        return -1;
    }
    out_buf[out_len - out_left] = 0;
    iconv_close(icd);
    return (int)out_len - out_left;
}

int analysis_json(char *pcInput,char *pcOutput){
    cJSON *stJsonSR=NULL;
    cJSON *stJsonItems=NULL;
    cJSON *stItems=NULL;


    int iItemsSize=0;
    int iI=0;
    stJsonSR=cJSON_Parse(pcInput);
    if(!stJsonSR){
        printf("*****%s stCjson wrong!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stJsonItems=cJSON_GetObjectItem(stJsonSR,"result");
    if(!stJsonItems){
        printf("*****%s can't find stJsonItems!\n",__FILE__);
        goto COMMON_ERROR;
    }
    iItemsSize = cJSON_GetArraySize(stJsonItems); 
    printf("一共有%d个元素\n",iItemsSize);
    for(iI=0;iI<iItemsSize;iI++){
        stItems = cJSON_GetArrayItem(stJsonItems,iI);
        printf("item type is %d\n",stItems->type);
        printf("translate:%s\n",stItems->valuestring);
        strncpy(pcOutput,stItems->valuestring,strlen(stItems->valuestring));
    }
    return 0;

COMMON_ERROR:
    if(stJsonSR){
        cJSON_Delete(stJsonSR);
    }
    return -1;

}

int base64_encode(char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    if (in_str == NULL || out_str == NULL)
        return -1; 

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}
int base64_decode(char *in_str, int in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    int counts;
    int size = 0;

    if (in_str == NULL || out_str == NULL)
        return -1; 

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bio = BIO_new_mem_buf(in_str, in_len);
    bio = BIO_push(b64, bio);

    size = BIO_read(bio, out_str, in_len);
    out_str[size] = '\0';

    BIO_free_all(bio);
    return size;
}

int del_space(char *src)
{
    char *pTmp = src;
    unsigned int iSpace = 0;

    while (*src != '\0') {
        if (*src != '\n') {
            *pTmp++ = *src;
        } else {
            iSpace++;
        }

        src++;
    }

    *pTmp = '\0';

    return iSpace;
}

int read_pcm(char *pcPcm,int iSize){
    int iPcm=0;
    int iR=0;
    printf("name:%s\n",FILENAME);
    iPcm=open(FILENAME,O_RDONLY);
    if(iPcm==-1){
        printf("errno:%d\n",errno);
        char *mesg=strerror(errno);
        printf("mesg:%s\n",mesg);
        return -2;
    }
    iR=read(iPcm,pcPcm,iSize);
    close(iPcm);
    return iR;
}

int struct_cjson(char *pcPut,int iSize,char *pcCjson){
    cJSON *stUploadJson=NULL;
    char *pcFinal=NULL;

    stUploadJson=cJSON_CreateObject();
    cJSON_AddStringToObject(stUploadJson,"format",FORMAT);
    cJSON_AddNumberToObject(stUploadJson,"rate",RATE);
    cJSON_AddNumberToObject(stUploadJson,"dev_pid",1536);
    cJSON_AddNumberToObject(stUploadJson,"channel",1);
    cJSON_AddStringToObject(stUploadJson,"token","24.a903669e6100f4ade1273d485d1f5f53.2592000.1561711055.282335-15249541");
    //curl -i -k 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=MapvabAqXweRy9TOAENSCCXC&client_secret=hSmQMZqVCYht6NSVYbgqdfLyOCYGG1u8'
    cJSON_AddStringToObject(stUploadJson,"cuid","baidu_workshop");
    //cJSON_AddNumberToObject(stUploadJson,"cuid",12345678);
    cJSON_AddNumberToObject(stUploadJson,"len",iSize);
    cJSON_AddStringToObject(stUploadJson,"speech",pcPut);

    pcFinal=cJSON_Print(stUploadJson);

    strncpy(pcCjson,pcFinal,strlen(pcFinal));
    //printf("拼成的Json是：%s\n",pcCjson);
    cJSON_Delete(stUploadJson);    
    free(pcFinal);   

    return 0;
}

int struct_cjson_Trans(char *pcPut,char *pcCjson,char *pcSkill){
    cJSON *stUploadJson=NULL;
    cJSON *stRequest=NULL;
    cJSON *stClient=NULL;
    cJSON *stCandidate=NULL;
    cJSON *stQuery_info=NULL;
    cJSON *stAsr_candidate=NULL;
    char *pcFinal=NULL;
    printf("hahaha\n");
    stUploadJson=cJSON_CreateObject();
    cJSON_AddStringToObject(stUploadJson,"bot_session","");
    cJSON_AddStringToObject(stUploadJson,"log_id","24480");
    cJSON_AddItemToObject(stUploadJson,"request",stRequest=cJSON_CreateObject());
    cJSON_AddNumberToObject(stRequest,"bernard_level",1);

    cJSON_AddStringToObject(stRequest,"client_session","{\"client_results\":\"\", \"candidate_options\":[]");
    cJSON_AddStringToObject(stRequest,"query",pcCjson);
    cJSON_AddItemToObject(stRequest,"query_info",stQuery_info=cJSON_CreateObject());
    cJSON_AddItemToObject(stQuery_info,"asr_candidates",stAsr_candidate=cJSON_CreateArray());
    cJSON_AddStringToObject(stQuery_info,"source","KEYBOARD");
    cJSON_AddStringToObject(stQuery_info,"type","TEXT");
    cJSON_AddStringToObject(stRequest,"updates","");
    cJSON_AddStringToObject(stRequest,"user_id","88888");
    cJSON_AddStringToObject(stUploadJson,"bot_id",pcSkill);
    cJSON_AddStringToObject(stUploadJson,"version","2.0");


    pcFinal=cJSON_Print(stUploadJson);
    printf("hahaha\n");
    printf("Translate:%s\n",pcFinal);
    //pcFinal=cJSON_PrintUnformatted(stUploadJson);
    strncpy(pcPut,pcFinal,strlen(pcFinal));
    //printf("Translate too:%s\n",pcPut);
    cJSON_Delete(stUploadJson);
    stUploadJson=NULL;

    pcFinal=NULL;
    return 0;
}

int analysis_json_Trans(char *pcInput,char *pcName,char *pcCmd,char *pcParameter){
    cJSON *stJsonSR=NULL;
    cJSON *stResponse=NULL;
    cJSON *stResult=NULL;
    cJSON *stSchema=NULL;
    cJSON *stSlots=NULL;
    cJSON *stAction_list=NULL;
    cJSON *stSay=NULL;

    cJSON *stItems=NULL;
    cJSON *stName=NULL;
    cJSON *stOriginal_word=NULL;

    cJSON *stItems2=NULL;


    int iItemsSize=0;
    int iI=0;
    stJsonSR=cJSON_Parse(pcInput);
    if(!stJsonSR){
        printf("*****%s stCjson wrong!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stResult=cJSON_GetObjectItem(stJsonSR,"result");
    if(!stResult){
        printf("*****%s can't find result!\n",__FILE__);
        goto COMMON_ERROR;
    }

    stResponse=cJSON_GetObjectItem(stResult,"response");
    if(!stResponse){
        printf("*****%s can't find Response!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stSchema=cJSON_GetObjectItem(stResponse,"schema");
    if(!stSchema){
        printf("*****%s can't find Schema!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stSlots=cJSON_GetObjectItem(stSchema,"slots");
    if(!stSlots){
        printf("*****%s can't find Slots!\n",__FILE__);
        goto COMMON_ERROR;
    }

    iItemsSize = cJSON_GetArraySize(stSlots);
    printf("一共有%d个元素\n",iItemsSize);
    for(iI=0;iI<iItemsSize;iI++){
        stItems = cJSON_GetArrayItem(stSlots,iI);
        stName= cJSON_GetObjectItem(stItems,"name");
        if(!stName){
            printf("*****%s can't find Name!\n",__FILE__);
            goto COMMON_ERROR;
        }
        printf("name is:%s\n",stName->valuestring);
        if(!strcmp(stName->valuestring,pcParameter)){
            printf("jin ru xun huan le\n");
            stOriginal_word= cJSON_GetObjectItem(stItems,"original_word");
            if(!stOriginal_word){
                printf("*****%s can't find original_world!\n",__FILE__);
                goto COMMON_ERROR;
            }
            printf("call to %s\n",stOriginal_word->valuestring);
            strncpy(pcName,stOriginal_word->valuestring,strlen(stOriginal_word->valuestring));
        }

    }
    stAction_list=cJSON_GetObjectItem(stResponse,"action_list");
    if(!stAction_list){
        printf("*****%s can't find action_list!\n",__FILE__);
        goto COMMON_ERROR;
    }

    iItemsSize = cJSON_GetArraySize(stAction_list);
    printf("一共有%d个元素\n",iItemsSize);
    for(iI=0;iI<iItemsSize;iI++){
        stItems2 = cJSON_GetArrayItem(stAction_list,iI);
        stSay=cJSON_GetObjectItem(stItems2,"say");
        if(!stSay){
            printf("*****%s can't find say!\n",__FILE__);
            goto COMMON_ERROR;
        }
        printf("baidu say:%s\n",stSay->valuestring);
        strncpy(pcCmd,stSay->valuestring,strlen(stSay->valuestring));
    }




    return 0;

COMMON_ERROR:
    if(stJsonSR){
        cJSON_Delete(stJsonSR);
    }
    return -1;

}

int curl_Upload(char *pcCjson,char *pcReply,char *pcUrl){

    CURL *curl;
    CURLcode res;
    int iRes=0;
    struct curl_slist *pstHeaders = NULL;

    pstHeaders=curl_slist_append(pstHeaders,"Content-Type: application/json");
    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pstHeaders);
        curl_easy_setopt(curl, CURLOPT_URL,pcUrl);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,pcCjson);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getReply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,pcReply);
        res = curl_easy_perform(curl);

        if(res==CURLE_OK){
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            printf("返回的结果是�?s\n",pcReply);
        }else{
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            printf("*****%s http not 200,is:%d!\nreturn :%s\n",__FILE__,iRes,getReply);

        }
        curl_easy_cleanup(curl);
    }

}

int asr_init(watch_asr_t *pstWatch_asr){
	bzero(pstWatch_asr,sizeof(watch_asr_t));
	return 0;
}


int asr_server(watch_asr_t *pstWatch_asr){
#if 0
		system("arecord -D hw:1,0 -f S16_LE -V mono -r16000 -c 1 -t wav  ./test.wav");
		printf("222222222222222222****************\n");
		system("ffmpeg -y  -i test.wav	-acodec pcm_s16le -f s16le -ac 1 -ar 16000 test01.pcm");
#endif
		printf("################111111\n");
		#if 0
		FILE *pstFp1;		
		FILE *pstFp2;

		pstFp1 = popen("arecord -D hw:1,0 -f S16_LE -V mono -r16000 -c 1 -t wav  ./test.wav", "r");
		if(pstFp1==NULL){
			perror("popen");
			printf("0000000000000000000000000000000000\n");
		}
		pclose(pstFp1);
		
		printf("1234567890000000000000000000000\n");
		pstFp2 = popen("ffmpeg -y  -i test.wav	-acodec pcm_s16le -f s16le -ac 1 -ar 16000 test01.pcm", "r");
		if(pstFp1==NULL){
			perror("popen");
		}
		pclose(pstFp2);
#endif
		system("arecord -D hw:1,0 -f S16_LE -V mono -r16000 -c 1 -t wav  ./test.wav");
		system("ffmpeg -y  -i test.wav	-acodec pcm_s16le -f s16le -ac 1 -ar 16000 test01.pcm");

		pstWatch_asr->pcPcm=(char *)malloc(FILESIZE*sizeof(char));
		pstWatch_asr->pcPcm64=(char *)malloc(3*FILESIZE*sizeof(char));
		pstWatch_asr->pcCjson=(char *)malloc(4*FILESIZE*sizeof(char));
		pstWatch_asr->pcReplyAsr=(char *)malloc(1024*sizeof(char));
	
		pstWatch_asr->iSize=read_pcm(pstWatch_asr->pcPcm,3*FILESIZE*sizeof(char));
		printf("iSzie=%d\n",pstWatch_asr->iSize);
	
		pstWatch_asr->iBase=base64_encode(pstWatch_asr->pcPcm,pstWatch_asr->iSize,pstWatch_asr->pcPcm64);
		del_space(pstWatch_asr->pcPcm64);
	
		struct_cjson(pstWatch_asr->pcPcm64,pstWatch_asr->iSize,pstWatch_asr->pcCjson);
		bzero(pstWatch_asr->pcReplyAsr,1024*sizeof(char));
		curl_Upload(pstWatch_asr->pcCjson,pstWatch_asr->pcReplyAsr,ASR_URL);
	
		analysis_json(pstWatch_asr->pcReplyAsr,pstWatch_asr->cszVtoT);
	
		printf("cszVtoT is :%s\n",pstWatch_asr->cszVtoT);

		free(pstWatch_asr->pcPcm);
		pstWatch_asr->pcPcm=NULL;
		free(pstWatch_asr->pcPcm64);
		pstWatch_asr->pcPcm64=NULL;
		free(pstWatch_asr->pcCjson);
		pstWatch_asr->pcCjson=NULL;
		free(pstWatch_asr->pcReplyAsr);
		pstWatch_asr->pcReplyAsr=NULL;

	
	return 0;
}


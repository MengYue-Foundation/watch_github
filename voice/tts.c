#include "tts.h"

int URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
    int i;
    int j = 0;//for result index
    char ch;

    if ((str== NULL) || (result== NULL) || (strSize<=0) || (resultSize<=0)) {
        return 0;
    }

    for ( i=0; (i<strSize)&&(j<resultSize); ++i) {
        ch = str[i];
        if (((ch>='A') && (ch<'Z')) ||
                ((ch>='a') && (ch<'z')) ||
                ((ch>='0') && (ch<'9'))) {
            result[j++] = ch;
        } else if (ch == ' ') {
            result[j++] = '+';
        } else if (ch == '.' || ch == '-' || ch == '_' || ch == '*') {
            result[j++] = ch;
        } else {
            if (j+3 < resultSize) {
                sprintf(result+j, "%%%02X", (unsigned char)ch);
                j += 3;
            } else {
                return 0;
            }
        }
    }

    result[j] = '\0';
    return j;
}
#if 0
static int len=0;

size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {

    int r=1;

    long len = 0;


    r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);

    if (r) /* Microsoft: we don't read the specs */
        *((long *) stream) = len;

    return size * nmemb;

}
#endif

int getReply_WriteInFile(void * ptr, size_t size, size_t nmemb, void * userdata)
{
    //strncpy((char*)userdata,(char*)ptr,nmemb);

    return fwrite(ptr,size,nmemb,(FILE*)userdata);
}

int curl_Upload_Tts(char *pcText,char *pcFilePath){

    CURL *curl;
    CURLcode res;
    int iRes=0;
    struct curl_slist *pstHeaders = NULL;
    char cszURL[512]={0};

    FILE *stFile;
#if 0
    curl_off_t local_file_len = -1 ;
    long filesize =0 ;
    struct stat file_info; 
    int use_resume = 0;
    if(stat(pcFilePath, &file_info) == 0)  

    {   

        local_file_len =  file_info.st_size;  

        use_resume  = 1;  

    }   

#endif



    stFile=fopen(pcFilePath,"w+");
    if(stFile==NULL){
        perror(NULL);
        printf("打开失败！\n");

        return 0;
    }

    //sprintf(cszURL,"http://tsn.baidu.com/text2audio?per=1&aue=6&lan=zh&ctp=1&cuid=abcdxxx&tok=lan=zh&ctp=1&cuid=abcdxxx&tok=24.0a51fc945ae123d2533d60f2fac974bc.2592000.1549530904.282335-15082345&tex=%s",pcText);
    sprintf(cszURL,"http://tsn.baidu.com/text2audio?per=1&aue=6&lan=zh&ctp=1&cuid=abcdxxx&tok=lan=zh&ctp=1&cuid=abcdxxx&tok=24.55bcd428c76eb7991472c90ee61a9bc9.2592000.1553935490.282335-15082345&tex=%s",pcText);
//curl -i -k 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=1K2WT5e4ZcOVvhilGKkf6VNq&client_secret=8e0OMkACPoEvaxfaC6rs4yVg9EZcwisl'
    pstHeaders=curl_slist_append(pstHeaders,"Content-Type: application/json");
    curl = curl_easy_init();
    if(curl){
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pstHeaders);
        curl_easy_setopt(curl, CURLOPT_URL,cszURL);

        //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);

        //curl_easy_setopt(curl, CURLOPT_HEADERDATA, &filesize);

        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,10);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, stFile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getReply_WriteInFile);
        res = curl_easy_perform(curl);

        if(res==CURLE_OK){
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            //printf("返回的结果是：%s\n长度是:%d\n",pcVoice,strlen(pcVoice));
        }else{
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &iRes);
            printf("*****%s http not 200,is:%d!\nreturn :%s\n",__FILE__,iRes,getReply_WriteInFile);

        }
        curl_easy_cleanup(curl);
    }

}



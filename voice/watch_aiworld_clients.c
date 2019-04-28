#include "watch_aiworld_client.h"

int client_cjson_struct(char* pcJsonBuf,ai_client_index_t *pstAi_client){
    cJSON *stJsonSend;
    char *pCjsonOutput=NULL;

    stJsonSend=cJSON_CreateObject();
    cJSON_AddNumberToObject(stJsonSend,"function",pstAi_client->iFunc);
    cJSON_AddNumberToObject(stJsonSend,"id",pstAi_client->iId);
    cJSON_AddStringToObject(stJsonSend,"text",pstAi_client->cszText);

    pCjsonOutput=cJSON_Print(stJsonSend);
    strncpy(pcJsonBuf,pCjsonOutput,strlen(pCjsonOutput));

    cJSON_Delete(stJsonSend);
    free(pCjsonOutput);

    return 0;
}

int client_cjson_analysis(char *pcJsonBuf,ai_client_index_t *pstAi_client){


    cJSON *stJsonRece;
    cJSON *stFunc;
    cJSON *stBody;
    cJSON *stSpirit;
    cJSON *stMoney;
    cJSON *stText;


    stJsonRece=cJSON_Parse(pcJsonBuf);

    stFunc=cJSON_GetObjectItem(stJsonRece,"function");
    if(!stFunc){
        printf("*****%s can't find function!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stBody=cJSON_GetObjectItem(stJsonRece,"body");
    if(!stBody){
        printf("*****%s can't find body!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stSpirit=cJSON_GetObjectItem(stJsonRece,"spirit");
    if(!stSpirit){
        printf("*****%s can't find spirit!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stMoney=cJSON_GetObjectItem(stJsonRece,"money");
    if(!stMoney){
        printf("*****%s can't find money!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stText=cJSON_GetObjectItem(stJsonRece,"text");
    if(!stText){
        printf("*****%s can't find text!\n",__FILE__);
        goto COMMON_ERROR;
    }

    pstAi_client->iFunc=stFunc->valueint;
   

    pstAi_client->fBody=stBody->valuedouble;
    pstAi_client->fSpirit=stSpirit->valuedouble;
    pstAi_client->fMoney=stMoney->valuedouble;

    strncpy(pstAi_client->cszText,stText->valuestring,strlen(stText->valuestring));


    if(stJsonRece){
        cJSON_Delete(stJsonRece);
    }   
    return 0;

COMMON_ERROR:
    if(stJsonRece){
        cJSON_Delete(stJsonRece);
    }   
    return -1; 


}

int client_send(int iSfd,ai_client_index_t *pstAi_client){
    char cszJsonBuf[256]={0};
        //向服务器发送请

    client_cjson_struct(cszJsonBuf,pstAi_client);
    printf("发送的Json:%s\niSfd=%d\n",cszJsonBuf,iSfd);
    write(iSfd,cszJsonBuf,strlen(cszJsonBuf));

    printf("已退出\n");
    return 0;
}
int client_rece(int iSfd,ai_client_index_t *pstAi_client){
    char cszReceBuf[256]={0};
    char cszText[256]={0};


    int r=read(iSfd,cszReceBuf,256);
    printf("iSfd=%d\n",iSfd);
    if(r==0){
        return -1;
    }
        //处理响应消息
    printf("%d：%s\n",r,cszReceBuf);
    client_cjson_analysis(cszReceBuf,pstAi_client);

    return 0;
}

int aiworld_client(char *pcIp,ai_client_index_t *pstAi_client_send,ai_client_index_t *pstAi_client_rece){
    int iSfd=0;
    SA4 stServ;//指定服务器的地址
    //创建socket

    iSfd=socket(AF_INET,SOCK_STREAM,0);

    if(iSfd==-1){
        perror("socket");
        return -1;
    }
    //初始化服务器的ip地址和端口号
    stServ.sin_family=AF_INET;
    stServ.sin_port=htons(6666);
    //服务器的ip地址就是本机的ip地址。127.0.0.1
    inet_pton(AF_INET,pcIp,&stServ.sin_addr);
    //将iSfd连接到服务器
    int iC=connect(iSfd,(SA *)&stServ,sizeof(stServ));
    if(iC==-1){
        perror("connect");
        return -1;
    }
    //已经连接到服务器了
    client_send(iSfd,pstAi_client_send);
    client_rece(iSfd,pstAi_client_rece);
    //关闭连接

    close(iSfd);
    return 0;
}

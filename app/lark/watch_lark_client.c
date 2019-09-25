#include "watch_lark_client.h"


mesg_buf_t g_stMessage_buf;
remind_buf_t g_stRemind_buf;
remind_buf_t g_stRemind_SetSuccess;


pthread_cond_t stCond;
pthread_mutex_t stMutex;

pthread_cond_t stCondRemind;
pthread_mutex_t stMutexRemind;

extern int g_iWhich_screen;



int struct_send_message(char *pcName,char *pcMessage,char *pcCjson){
	 cJSON *stUploadJson=NULL;
    char *pcFinal=NULL;

    stUploadJson=cJSON_CreateObject();
    cJSON_AddStringToObject(stUploadJson,"name",pcName);
	cJSON_AddStringToObject(stUploadJson,"message",pcMessage);

    pcFinal=cJSON_Print(stUploadJson);

    strncpy(pcCjson,pcFinal,strlen(pcFinal));
    //printf("拼成的Json是：%s\n",pcCjson);
    cJSON_Delete(stUploadJson);    
    free(pcFinal);   

    return 0;
}

int analysis_receive_message(char *pcInput,char *pcName,char *pcMessage,char *pcTime,char *pcReminder){
	cJSON *stJsonSR=NULL;
    cJSON *stJsonName=NULL;
    cJSON *stJsonMessage=NULL;
	cJSON *stJsonTime=NULL;
	cJSON *stJsonReminder=NULL;

    int iItemsSize=0;
    int iI=0;
    stJsonSR=cJSON_Parse(pcInput);
    if(!stJsonSR){
        printf("*****%s stCjson wrong!\n",__FILE__);
        goto COMMON_ERROR;
    }
    stJsonName=cJSON_GetObjectItem(stJsonSR,"name");
    if(!stJsonName){
        printf("*****%s can't find stJsonName!\n",__FILE__);
        goto COMMON_ERROR;
    }
	strncpy(pcName,stJsonName->valuestring,strlen(stJsonName->valuestring));
	
	if(!strncmp(stJsonName->valuestring,"remind",6)){
		
		stJsonMessage=cJSON_GetObjectItem(stJsonSR,"message");
		if(!stJsonMessage){
			printf("*****%s can't find stJsonMessage!\n",__FILE__);
			goto COMMON_ERROR;
		}
		strncpy(pcMessage,stJsonMessage->valuestring,strlen(stJsonMessage->valuestring));
		
		stJsonTime=cJSON_GetObjectItem(stJsonSR,"time");
		if(!stJsonTime){
			printf("*****%s can't find stJsonTime!\n",__FILE__);
			goto COMMON_ERROR;
		}
		strncpy(pcTime,stJsonTime->valuestring,strlen(stJsonTime->valuestring));
		
		stJsonReminder=cJSON_GetObjectItem(stJsonSR,"reminder");
		if(!stJsonReminder){
			printf("*****%s can't find stJsonReminder!\n",__FILE__);
			goto COMMON_ERROR;
		}
		strncpy(pcReminder,stJsonReminder->valuestring,strlen(stJsonReminder->valuestring));
		return 1;
		
	}else{
	
		stJsonMessage=cJSON_GetObjectItem(stJsonSR,"message");
		if(!stJsonMessage){
			printf("*****%s can't find stJsonMessage!\n",__FILE__);
			goto COMMON_ERROR;
		}
		strncpy(pcMessage,stJsonMessage->valuestring,strlen(stJsonMessage->valuestring));
	}
	
    return 0;

COMMON_ERROR:
    if(stJsonSR){
        cJSON_Delete(stJsonSR);
    }
    return -1;

}

void *client_send(void *pvArgs){
    int iSfd=*((int *)pvArgs);
	
	
	bzero(&g_stRemind_SetSuccess,sizeof(remind_buf_t));
	printf("%s,iSfd:%d\n",__FUNCTION__,iSfd);
	
	while(1){
		char cszSendBuf[256]={0};
		pthread_mutex_lock(&stMutex);
		pthread_cond_wait(&stCond, &stMutex); 
		pthread_mutex_unlock(&stMutex);
		
		
		if(g_stMessage_buf.iState == 1){
			printf("****%s,%s,联系人：%s,消息内容:%s\n",__FILE__,__FUNCTION__,g_stMessage_buf.cszName,g_stMessage_buf.cszMessage);
			struct_send_message(g_stMessage_buf.cszName,g_stMessage_buf.cszMessage,cszSendBuf);
			
			write(iSfd,cszSendBuf,strlen(cszSendBuf));
			bzero(&g_stMessage_buf,sizeof(mesg_buf_t));
		}else if(g_stRemind_SetSuccess.iState == 1){
			struct_send_message(g_stRemind_SetSuccess.cszName,"设置成功!",cszSendBuf);
			write(iSfd,cszSendBuf,strlen(cszSendBuf));
			bzero(&g_stRemind_SetSuccess,sizeof(remind_buf_t));
		}
	}

	close(iSfd);
	
}

void *client_rece(void *pvArgs){
	
	int iSfd=*((int *)pvArgs);
	printf("%s,iSfd:%d\n",__FUNCTION__,iSfd);
	while(1){
		char cszText[256]={0};
		char cszReceBuf[256]={0};
		char cszName[20] = {0};
		char cszMessage[256] = {0};
		char cszReminder[20] = {0};
		char cszTime[64] = {0};
		char cszName_GB[40] = {0};
		char cszMessage_GB[256] = {0};
		int iReturn = 0;
		int iR=read(iSfd,cszReceBuf,256);  
		printf("JSON:%s\n",cszReceBuf);
		
		iReturn = analysis_receive_message(cszReceBuf,cszName,cszMessage,cszTime,cszReminder);
		if(iReturn == 1){
			
			strncpy(g_stRemind_buf.cszName,cszReminder,strlen(cszReminder));
			strncpy(g_stRemind_buf.cszRemind,cszMessage,strlen(cszMessage));
			strncpy(g_stRemind_buf.cszTime,cszTime,strlen(cszTime));
			g_stRemind_buf.iState = 1;
			
			memcpy(&g_stRemind_SetSuccess,&g_stRemind_buf,sizeof(remind_buf_t));
			
			pthread_mutex_lock(&stMutexRemind);
			pthread_cond_signal(&stCondRemind); 
			pthread_mutex_unlock(&stMutexRemind);
		}
		
		
		if(g_iWhich_screen == 1000){
			char cszPrint[40] = {0};
			//char cszGN_Print[512] = {0};
			sprintf(cszPrint,"消息来自%s",cszName,cszMessage);
			charset_convert("UTF-8","GB2312",cszPrint,strlen(cszPrint),cszName_GB,40);
			charset_convert("UTF-8","GB2312",cszMessage,strlen(cszMessage),cszMessage_GB,512);
			
			enqueue_display(20, 15 ,PART_REFRESH,cszName_GB, PRIORITY_2, TEXT_CHINESE);
			enqueue_display(20, 40 ,PART_REFRESH,cszMessage_GB, PRIORITY_2, TEXT_CHINESE);

			
			printf("****%s,%s,%s:%s\n",__FILE__,__FUNCTION__,cszName,cszMessage);
			
			
			
			do_taptic_engine(1, 1);
			
			
			//printf("****%s,%s,%s\n",__FILE__,__FUNCTION__,cszPrint);
		}else if(g_iWhich_screen == 1200){
			printf("****%s,%s,g_iWhich_screen=%d\n",__FILE__,__FUNCTION__,g_iWhich_screen);
			charset_convert("UTF-8","GB2312",cszMessage,strlen(cszMessage),cszMessage_GB,256);
			
			printf("1111111\n");
			if(!strncmp("高也",cszName,strlen(cszName))){
				printf("1****111***+111\n");
				enqueue_display(70, 8 ,PART_REFRESH,cszMessage_GB, PRIORITY_2, TEXT_CHINESE);

				
			}else if(!strncmp("上官悦",cszName,strlen(cszName))){
				enqueue_display(70, 40 ,PART_REFRESH,cszMessage_GB, PRIORITY_2, TEXT_CHINESE);

				
			}else if(!strncmp("闫繁琪",cszName,strlen(cszName))){
				enqueue_display(70, 72 ,PART_REFRESH,cszMessage_GB, PRIORITY_2, TEXT_CHINESE);

				
			}
		}
		if(iR==0){
			break;
		}
	}
	close(iSfd);
}

void *watch_lark_client(void *pvArgs){
	    SA4 stServ;//指定服务器的地址
        //创建socket

        int iSfd=0;
		
		bzero(&g_stMessage_buf,sizeof(mesg_buf_t));
		bzero(&g_stRemind_buf,sizeof(remind_buf_t));
		bzero(&g_stRemind_SetSuccess,sizeof(remind_buf_t));
		
		

		
		
		pthread_mutex_init(&stMutex, NULL);
		pthread_cond_init(&stCond, NULL);
		
		pthread_mutex_init(&stMutexRemind, NULL);
		pthread_cond_init(&stCondRemind, NULL);

		printf("111111\n");
        iSfd=socket(AF_INET,SOCK_STREAM,0);
        if(iSfd==-1){
            perror("socket");
            return -1;
        }
        //初始化服务器的ip地址和端口号

        stServ.sin_family=AF_INET;
        stServ.sin_port=htons(6668);
        //服务器的ip地址就是本机的ip地址。127.0.0.1
        inet_pton(AF_INET,"47.94.222.21",&stServ.sin_addr);
        //将iSfd连接到服务器
		
		printf("******%s,%s,iPort:%d,Ip:%s\n",__FILE__,__FUNCTION__,SERVER_PORT,SERVER_IP);
		printf("111111\n");
        int iC=connect(iSfd,(SA *)&stServ,sizeof(stServ));
        if(iC==-1){
            perror("connect");
            return -1;;
        }
        //已经连接到服务器了
		printf("111111\n");
        pthread_t stTid0;
		pthread_t stTid1;

            printf("快跳快跳！\n");
            pthread_create(&stTid0,NULL,client_send,&iSfd);
            pthread_create(&stTid1,NULL,client_rece,&iSfd);
            sleep(5);
		  

		while(1){
			sleep(1);
		}
        close(iSfd);


    return 0;
}
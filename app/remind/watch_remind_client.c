#include "watch_remind_client.h"

extern pthread_cond_t stCondRemind;
extern pthread_mutex_t stMutexRemind;

extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;

extern pthread_cond_t stCond;
extern pthread_mutex_t stMutex;

extern remind_buf_t g_stRemind_buf;
extern int g_iWhich_screen;

int iRemind;
remind_buf_t g_stszRemind[REMIND_SIZE];

char * Is_NULL(char *pcInput){
	
	char *pcBuf;
	pcBuf=strstr(pcInput,"-");
	if(pcBuf == NULL){
		pcBuf=strstr(pcInput,",");
		if(pcBuf == NULL){
			pcBuf=strstr(pcInput,":");
			if(pcBuf == NULL){
				return NULL;
			}
		}
	}
	return pcBuf;
}

int analysis_time(char *pcTime,Remind_Time_t *pstRemindTime){
	
	char *pcYear = NULL;
	char *pcMounth = NULL;
	char *pcDay = NULL;
	char *pcHour = NULL;

		
	pcYear = Is_NULL(pcTime);
	if(pcYear == NULL){
		return -1;
	}
	strncpy(pstRemindTime->cszYear,pcTime,(pcYear - pcTime));
	printf("*****%s,%s,year=%s\n",__FILE__,__FUNCTION__,pstRemindTime->cszYear);
	
	pcMounth = Is_NULL(pcYear + 1);
		if(pcMounth == NULL){
		return -1;
	}
	strncpy(pstRemindTime->cszMounth,pcYear + 1,(pcMounth - pcYear)-1);
	printf("*****%s,%s,mounth=%s\n",__FILE__,__FUNCTION__,pstRemindTime->cszMounth);
	
	pcDay = Is_NULL(pcMounth + 1);
		if(pcDay == NULL){
		return -1;
	}
	strncpy(pstRemindTime->cszDay,pcMounth + 1,(pcDay - pcMounth)-1);
	printf("*****%s,%s,day=%s\n",__FILE__,__FUNCTION__,pstRemindTime->cszDay);
	
	pcHour = Is_NULL(pcDay + 1);
		if(pcHour == NULL){
		return -1;
	}
	strncpy(pstRemindTime->cszHour,pcDay + 1,(pcHour - pcDay)-1);
	printf("*****%s,%s,hour=%s\n",__FILE__,__FUNCTION__,pstRemindTime->cszHour);
	
	//pcMin = Is_NULL(pcHour);
		//if(pcMin == NULL){
		//return -1;
	//}
	
	strncpy(pstRemindTime->cszMin,pcHour + 1,(strlen(pcHour) - 1));
	printf("*****%s,%s,min=%s\n",__FILE__,__FUNCTION__,pstRemindTime->cszMin);
	
	return 0;
}

int atoi_time(Remind_Time_t *pstRemindTime,watchTime_t *pstWatchTime){
	
	pstWatchTime->iYear = atoi(pstRemindTime->cszYear);	
	printf("*****%s,%s,%d,year=%d\n",__FILE__,__FUNCTION__,__LINE__,pstWatchTime->iYear);
	
	pstWatchTime->iMounth = atoi(pstRemindTime->cszMounth);
	printf("*****%s,%s,%d,mounth=%d\n",__FILE__,__FUNCTION__,__LINE__,pstWatchTime->iMounth);
	
	pstWatchTime->iDay = atoi(pstRemindTime->cszDay);	
	printf("*****%s,%s,%d,day=%d\n",__FILE__,__FUNCTION__,__LINE__,pstWatchTime->iDay);
	
	pstWatchTime->iHour = atoi(pstRemindTime->cszHour);	
	printf("*****%s,%s,%d,hour=%d\n",__FILE__,__FUNCTION__,__LINE__,pstWatchTime->iHour);
	
	pstWatchTime->iMin = atoi(pstRemindTime->cszMin);	
	printf("*****%s,%s,%d,min=%d\n",__FILE__,__FUNCTION__,__LINE__,pstWatchTime->iMin);
	
	return 0;
}


int remind_min(watchTime_t *pstCurrentTime,watchTime_t *pstRemindTime){
	
	int iMin = 0;
	int iCurrentTime = 0;
	int iRemindTime = 0;
	
	printf("*****%s,%s,%d,pstCurrentTime->iMin=%d,pstRemindTime->iMin=%d\n",__FILE__,__FUNCTION__,__LINE__,pstCurrentTime->iMin,pstRemindTime->iMin);
	printf("*****%s,%s,%d,pstCurrentTime->iHour=%d,pstRemindTime->iHour=%d\n",__FILE__,__FUNCTION__,__LINE__,pstCurrentTime->iHour,pstRemindTime->iHour);
	printf("*****%s,%s,%d,pstCurrentTime->iDay=%d,pstRemindTime->iDay=%d\n",__FILE__,__FUNCTION__,__LINE__,pstCurrentTime->iDay,pstRemindTime->iDay);
	printf("*****%s,%s,%d,pstCurrentTime->iMounth=%d,pstRemindTime->iMounth=%d\n",__FILE__,__FUNCTION__,__LINE__,pstCurrentTime->iMounth,pstRemindTime->iMounth);
	

	iCurrentTime = pstCurrentTime->iMin + (pstCurrentTime->iHour + (pstCurrentTime->iDay + pstCurrentTime->iMounth * 30) * 24)*60;
	iRemindTime = pstRemindTime->iMin + (pstRemindTime->iHour + (pstRemindTime->iDay + pstRemindTime->iMounth * 30) * 24)*60;
	
	iMin = iRemindTime - iCurrentTime;
	printf("*****%s,%s,%d,Current=%d,Remind=%d,iMin=%d\n",__FILE__,__FUNCTION__,__LINE__,iCurrentTime,iRemindTime,iMin);
	
	
	return iMin;
}

void * remind_time(void *pvArgs){
	int iMin = *(int *)pvArgs;
	int iRemind_Now = iRemind;
	watchTime_t stWatchTime;
	
	bzero(&stWatchTime,sizeof(watchTime_t));
	watch_currenttime(&stWatchTime);
	
	
	printf("*****%s,%s,%d,stRemind.cszName=%s,stRemind.cszTime=%s,stRemind.cszRemind=%s\n",__FILE__,__FUNCTION__,__LINE__,g_stszRemind[iRemind].cszName,g_stszRemind[iRemind].cszTime,g_stszRemind[iRemind].cszRemind);
	
	
	while(1){
		if(iMin == 0){
			break;
		}
		iMin --;
		printf("*****%s,%s,%d,iMin=%d,stWatchTime.iSec=%d\n",__FILE__,__FUNCTION__,__LINE__,iMin,stWatchTime.iSec);
		if(stWatchTime.iSec != 0){
			sleep(60 - stWatchTime.iSec);
			stWatchTime.iSec = 0;
		}else{
			sleep(60);
		}
	}
	
	printf("*****%s,%s,%d,提醒时间到！\n",__FILE__,__FUNCTION__,__LINE__);
	
	char cszName_GB[32] = {0}; 
	char cszTime_GB[32] = {0};
	char cszRemind_GB[128] = {0};
			
	sleep(1);
	
	charset_convert("UTF-8","GB2312",g_stszRemind[iRemind].cszName,strlen(g_stszRemind[iRemind].cszName),cszName_GB,32);
	charset_convert("UTF-8","GB2312",g_stszRemind[iRemind].cszTime,strlen(g_stszRemind[iRemind].cszTime),cszTime_GB,32);
	charset_convert("UTF-8","GB2312",g_stszRemind[iRemind].cszRemind,strlen(g_stszRemind[iRemind].cszRemind),cszRemind_GB,128);
	enqueue_display(40, 15 ,PART_REFRESH,cszName_GB, PRIORITY_3, TEXT_CHINESE);			
	enqueue_display(40, 40 ,PART_REFRESH,cszTime_GB, PRIORITY_3, TEXT_ENGLISH);
	enqueue_display(20, 70 ,PART_REFRESH,cszRemind_GB, PRIORITY_3, TEXT_CHINESE);
	enqueue_display(20, 70 ,PART_REFRESH,"end", PRIORITY_3, TEXT_CHINESE);
	
	bzero(&g_stszRemind[iRemind_Now],sizeof(remind_buf_t));
	
	do_taptic_engine(1, 3);
	
	sleep(5);
	
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
	
	
}

int Remind_which_empty(remind_buf_t * pstRemind){
	int iI = 0;
	for(iI=0;iI<REMIND_SIZE;iI++){
		if(pstRemind[iI].iState ==0){
			return iI;
		}
	}
	
	return -1;
}

void * watch_remind(void *pvArgs){
	int iMin = 0;
	bzero(&g_stszRemind,sizeof(remind_buf_t) * REMIND_SIZE);
	
	while(1){
		pthread_t stTid_time;
		
		Remind_Time_t stRemindTime;
		bzero(&stRemindTime,sizeof(Remind_Time_t));
		
		watchTime_t stWatchTime;
		watchTime_t stWatchTime_Remind;
		bzero(&stWatchTime,sizeof(watchTime_t));
		bzero(&stWatchTime_Remind,sizeof(watchTime_t));
		
		
		pthread_mutex_lock(&stMutexRemind);
		pthread_cond_wait(&stCondRemind, &stMutexRemind); 
		pthread_mutex_unlock(&stMutexRemind);
		
		iRemind = Remind_which_empty(g_stszRemind);
		printf("*****%s,%s,%d,iRemind = %d！\n",__FILE__,__FUNCTION__,__LINE__,iRemind);
		
		memcpy(&g_stszRemind[iRemind],&g_stRemind_buf,sizeof(remind_buf_t));
		g_stszRemind[iRemind].iState = 1;
		
		printf("*****%s,%s,%d,g_stszRemind.cszName=%s,g_stszRemind.cszTime=%s,g_stszRemind.cszRemind=%s,g_stszRemind[iRemind].iState=%d\n",__FILE__,__FUNCTION__,__LINE__,g_stszRemind[iRemind].cszName,g_stszRemind[iRemind].cszTime,g_stszRemind[iRemind].cszRemind,g_stszRemind[iRemind].iState);
		
		bzero(&g_stRemind_buf,sizeof(remind_buf_t));
		
		iMin = 0;
		
		
		int iReturn = analysis_time(g_stszRemind[iRemind].cszTime,&stRemindTime);
		if(iReturn == -1){
			printf("时间格式设置错误!");
		}
		
		
		
		if(g_iWhich_screen == 1000){
			char cszName_GB[32] = {0}; 
			char cszTime_GB[32] = {0};
			char cszRemind_GB[128] = {0};
			
			
			charset_convert("UTF-8","GB2312",g_stRemind_buf.cszName,strlen(g_stRemind_buf.cszName),cszName_GB,32);
			charset_convert("UTF-8","GB2312",g_stRemind_buf.cszTime,strlen(g_stRemind_buf.cszTime),cszTime_GB,32);
			charset_convert("UTF-8","GB2312",g_stRemind_buf.cszRemind,strlen(g_stRemind_buf.cszRemind),cszRemind_GB,128);
			enqueue_display(20, 5 ,PART_REFRESH,cszName_GB, PRIORITY_2, TEXT_CHINESE);			
			enqueue_display(20, 20 ,PART_REFRESH,cszTime_GB, PRIORITY_2, TEXT_ENGLISH);
			enqueue_display(20, 45 ,PART_REFRESH,cszRemind_GB, PRIORITY_2, TEXT_CHINESE);	
			
		}
		
		
		watch_currenttime(&stWatchTime);
		atoi_time(&stRemindTime,&stWatchTime_Remind);
		iMin = remind_min(&stWatchTime,&stWatchTime_Remind);
		
		pthread_create(&stTid_time,NULL,remind_time,&iMin);
		
		pthread_mutex_lock(&stMutex);
		pthread_cond_signal(&stCond); 
		pthread_mutex_unlock(&stMutex);
		
		
		
	}
}
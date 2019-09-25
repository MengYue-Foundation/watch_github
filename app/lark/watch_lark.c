#include "watch_lark.h"

extern mesg_buf_t g_stMessage_buf;
extern pthread_cond_t stCond;
extern pthread_mutex_t stMutex;

extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;

static int iLarkState;

extern int g_iWhich_screen;

extern int g_iNum;

char cszMeaasge_show[256];
static void * key0_first_press_down(void *p_parameter){
	printf("*******123\n");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_gaoye.bmp" , PRIORITY_4, PICTURE);

}

static void * key0_short_press(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_happy.bmp" , PRIORITY_4, PICTURE);	
}

static void * key0_long_press(void *p_parameter){
	char cszSpeakContent[256]={0};
	bzero(cszMeaasge_show,256);
	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);
	
	strncpy(g_stMessage_buf.cszMessage,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
	strncpy(g_stMessage_buf.cszName,"高也",strlen("高也"));
	
	strncpy(cszMeaasge_show,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
	g_stMessage_buf.iState = 1;
	
		
	pthread_mutex_lock(&stMutex);
	pthread_cond_signal(&stCond); 
	pthread_mutex_unlock(&stMutex);
	



	
}

static void * key0_up(void *p_parameter){
	char cszSpeakContent[256]={0};
	charset_convert("UTF-8","GB2312",cszMeaasge_show,strlen(cszMeaasge_show),cszSpeakContent,256);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_list.bmp" , PRIORITY_3, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,cszSpeakContent , PRIORITY_3, TEXT_CHINESE);
	enqueue_display(0 ,0 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
}

static void * key1_first_press_down(void *p_parameter){


}

static void * key1_short_press(void *p_parameter){

}

static void * key1_long_press(void *p_parameter){
	
}

static void * key1_up(void *p_parameter){

}


static void * key2_first_press_down(void *p_parameter){


}

static void * key2_short_press(void *p_parameter){

}

static void * key2_long_press(void *p_parameter){
	
}

static void * key2_up(void *p_parameter){

}



static void * key3_first_press_down(void *p_parameter){
	printf("****%s,%s\n",__FILE__,__FUNCTION__);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_return.bmp" , PRIORITY_4, PICTURE);

}

static void * key3_short_press(void *p_parameter){

	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
	

}

static void * key3_long_press(void *p_parameter){
	
}

static void * key3_up(void *p_parameter){

}


int key_lark_init(void){
#if 1
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key0_first_press_down,NULL);
	//key_sys_register(KEY0_SHORT_PRESS,key0_short_press,pstWatch);
	key_sys_register(KEY0_LONG_PRESS,key0_long_press,NULL);
	key_sys_register(KEY0_UP,key0_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key1_first_press_down,NULL);
	//key_sys_register(KEY1_SHORT_PRESS,key1_short_press,pstWatch);
	key_sys_register(KEY1_LONG_PRESS,key1_long_press,NULL);
    key_sys_register(KEY1_UP,key1_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key2_first_press_down,NULL);
	//key_sys_register(KEY2_SHORT_PRESS,key2_short_press,pstWatch);
	key_sys_register(KEY2_LONG_PRESS,key2_long_press,NULL);
    key_sys_register(KEY2_UP,key2_up,NULL);

	key_sys_register(KEY3_FIRST_PRESS_DOWN,key3_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key3_short_press,NULL);
	//key_sys_register(KEY3_LONG_PRESS,key3_long_press,pstWatch);
	//key_sys_register(KEY3_UP,key3_up,pstWatch);
#endif
	
	return 0;
}



void * lark_first_press_down(void *p_parameter)
{
	printf("****%s,%s\n",__FILE__,__FUNCTION__);

	
	switch(g_iNum){
	case 0:
		break;
	case 1:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_1_lark.bmp" , PRIORITY_4, PICTURE);
		break;
	case 2:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_2_lark.bmp" , PRIORITY_4, PICTURE);
		break;
	case 3:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pq" , PRIORITY_4, PICTURE);
		break;
	default:
		break;
	}

}


void * lark_short_press(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	
	g_iWhich_screen = 1200;
	
	key_lark_init();
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/dack.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/white.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_list.bmp" , PRIORITY_4, PICTURE);
	
	
}

void * lark_long_press(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	
	char cszSpeakContent[256]={0};
	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);
	
	if(iLarkState==0){
		strncpy(g_stMessage_buf.cszMessage,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
		
		charset_convert("UTF-8","GB2312",stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT),cszSpeakContent,256);
		enqueue_display(20 ,15,PART_REFRESH,"发送内容", PRIORITY_2, TEXT_CHINESE);
		enqueue_display(20 ,40,PART_REFRESH,cszSpeakContent, PRIORITY_2, TEXT_CHINESE);

		iLarkState = 1;
	}else{
		char cszName[40] = {0};
		strncpy(g_stMessage_buf.cszName,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
		g_stMessage_buf.iState = 1;
		
		pthread_mutex_lock(&stMutex);
		pthread_cond_signal(&stCond); 
		pthread_mutex_unlock(&stMutex);
		
		charset_convert("UTF-8","GB2312",stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT),cszName,40);

		enqueue_display(20 ,15,PART_REFRESH,"消息发送至", PRIORITY_2, TEXT_CHINESE);
		enqueue_display(20 ,40,PART_REFRESH,cszName, PRIORITY_2, TEXT_CHINESE);

		printf("speak content!!!!\n");
		iLarkState = 0;
	}
	
}

void * lark_up(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	switch(g_iNum){
	case 0:
		break;
	case 1:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_1.bmp" , PRIORITY_4, PICTURE);
		break;
	case 2:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_2.bmp" , PRIORITY_4, PICTURE);
		break;
	case 3:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_3.bmp" , PRIORITY_4, PICTURE);
		break;
	default:
		break;
	}
}




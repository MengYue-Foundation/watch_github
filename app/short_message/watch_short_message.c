#include "watch_short_message.h"

watch_mesg_t stWatchMesg;

extern int g_iNum;
void * short_message_first_press_down(void *p_parameter)
{
    printf("short_message_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_mesg.bmp" , PRIORITY_1, PICTURE);
	    printf("weather_short_press, para:%s\n", (char *)p_parameter);
		
	switch(g_iNum){
	case 0:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_mesg.bmp" , PRIORITY_4, PICTURE);
		break;
	case 1:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_1_mesg.bmp" , PRIORITY_4, PICTURE);
		break;
	case 2:

		break;
	case 3:

		break;
	default:
		break;
	}
}

void * short_message_short_press(void *p_parameter)
{
    printf("short_message_short_press, para:%s\n", (char *)p_parameter);
}

void * short_message_long_press(void *p_parameter)
{
    printf("short_message_long_press\n");
	
	char cszSpeakContent[256]={0};
	
	enqueue_display(0 ,64,PART_REFRESH,"./pic/standby_mesg.bmp" , PRIORITY_1, PICTURE);


	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);
	
	

	if(stWatchMesg.iState == MESG_CONTENT){
		
		bzero(&stWatchMesg,sizeof(watch_mesg_t));
		strncpy(stWatchMesg.cszMesg,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
		
		charset_convert("UTF-8","GB2312",stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT),cszSpeakContent,256);
	
		enqueue_display(20 ,20,PART_REFRESH,cszSpeakContent, PRIORITY_2, TEXT_CHINESE);

		
		stWatchMesg.iState = MESG_NAME;
		printf("speak name!!!!\n");
	}else if(stWatchMesg.iState == MESG_NAME){
		strncpy(stWatchMesg.cszName,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));
		watch_mesg_send(&stWatchMesg);
		stWatchMesg.iState = MESG_CONTENT;
		printf("speak content!!!!\n");
	}



	return 0;
}



void * short_message_up(void *p_parameter){
	switch(g_iNum){
	case 0:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_4, PICTURE);
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
  return 0;
}

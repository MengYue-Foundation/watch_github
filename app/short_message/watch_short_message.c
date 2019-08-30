#include "watch_short_message.h"

watch_mesg_t stWatchMesg;

void * short_message_first_press_down(void *p_parameter)
{
    printf("short_message_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_mesg.bmp" , PRIORITY_1, PICTURE);
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
		enqueue_display(0 ,64,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
		enqueue_display(20 ,20,PART_REFRESH,cszSpeakContent, PRIORITY_3, TEXT_CHINESE);
		enqueue_display(0 ,0 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
		
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
	printf("1111111111111111111111\n");
    enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);
  return 0;
}

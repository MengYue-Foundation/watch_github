#include "watch_note.h"

extern int g_iNum;

void * note_first_press_down(void *p_parameter)
{
	printf("****%s,%s\n",__FILE__,__FUNCTION__);
	switch(g_iNum){
	case 0:
		break;
	case 1:
		break;
	case 2:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_2_text.bmp" , PRIORITY_1, PICTURE);
		break;
	case 3:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_3_text.bmp" , PRIORITY_1, PICTURE);
		break;
	default:
		break;
	}

}


void * note_short_press(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	

	
}

void * note_long_press(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	
	char cszSpeakContent[256]={0};
	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);

		
	charset_convert("UTF-8","GB2312",stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT),cszSpeakContent,256);

	enqueue_display(20 ,15,PART_REFRESH,"±ãÇ©ÄÚÈÝ", PRIORITY_2, TEXT_CHINESE);
	enqueue_display(20 ,40,PART_REFRESH,cszSpeakContent, PRIORITY_2, TEXT_CHINESE);

	
	watch_note_client(stWatchAsr.cszVtoT);
	
}

void * note_up(void *p_parameter)
{
    printf("****%s,%s\n",__FILE__,__FUNCTION__);
	
	switch(g_iNum){
	case 0:
		break;
	case 1:
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
#include "watch_phone.h"


extern int g_iWhich_screen;
extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;

static void * key_connect_first_press_down(void *p_parameter)
{
    printf("key1_first_press_down\n");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/phone/phone_connect_off.bmp" , PRIORITY_4, PICTURE);
}

static void * key_connect_press(void *p_parameter)
{
    printf("phone_short_press, para:%s\n", (char *)p_parameter);
	at_send_command("ATH", NULL);
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
}

static int phone_connect_key_init(void){
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key_connect_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,key_connect_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,key_connect_press,NULL);
	key_sys_register(KEY0_UP,NULL,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key_connect_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,key_connect_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,key_connect_press,NULL);
	key_sys_register(KEY1_UP,NULL,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key_connect_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,key_connect_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,key_connect_press,NULL);
	key_sys_register(KEY2_UP,NULL,NULL);
	
	key_sys_register(KEY3_FIRST_PRESS_DOWN,key_connect_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key_connect_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,key_connect_press,NULL);
	key_sys_register(KEY3_UP,NULL,NULL);

	
	return 0;
}

static void *phone_dropped_2(void *pvArgs){
	
	printf("%s,%d,XXXXXXXXXXXXXXXXXXX\n",__FUNCTION__,__LINE__);
	at_send_command("ATH", NULL);
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
}

static void *phone_dropped_1(void *pvArgs){
	
	phone_connect_key_init();
	On_Request(CALLING_HANG_UP, phone_dropped_1,NULL);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/phone/phone_connect.bmp" , PRIORITY_4, PICTURE);
	printf("%s,%d,XXXXXXXXXXXXXXXXXXX\n",__FUNCTION__,__LINE__);

}



void * phone_first_press_down(void *p_parameter)
{
    printf("key1_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_phone.bmp" , PRIORITY_1, PICTURE);
}


void * phone_short_press(void *p_parameter)
{
    printf("phone_short_press, para:%s\n", (char *)p_parameter);
}

void * phone_long_press(void *p_parameter)
{
    printf("phone_long_press\n");
	char cszPrint[40] = {0};

	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_phone.bmp" , PRIORITY_1, PICTURE);

	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);

	watch_phone_call(stWatchAsr.cszVtoT);
		
	charset_convert("UTF-8","GB2312",cszPrint,strlen(cszPrint),stWatchAsr.cszVtoT,40);
	enqueue_display(20 ,15 ,PART_REFRESH,"ÕýÔÚºô½Ð" , PRIORITY_3, PICTURE);
	enqueue_display(20 ,15 ,PART_REFRESH,cszPrint, PRIORITY_3, PICTURE);
	
	On_Request(PHONE_CALL_CONNECT, phone_dropped_1,NULL);

	return 0;
}


void * phone_up(void *p_parameter)
{
    printf("phone_up\n");
	

	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_4, PICTURE);
	
}


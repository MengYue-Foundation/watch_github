#include "watch_phone_receive.h"

extern int g_iWhich_screen;
extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;

int g_iPhone_state;


void * key_connect_first_press_down(void *p_parameter)
{
    printf("key1_first_press_down\n");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/phone/phone_connect_off.bmp" , PRIORITY_4, PICTURE);
}

void * key_connect_press(void *p_parameter)
{
    printf("phone_short_press, para:%s\n", (char *)p_parameter);
	at_send_command("ATH", NULL);
	g_iPhone_state = 1;
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
}


int phone_connect_key_init(void){
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


void * key1_first_press_down(void *p_parameter)
{
    printf("key1_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/phone/phone_receive_on.bmp" , PRIORITY_4, PICTURE);
}


void * key1_press(void *p_parameter)
{
    printf("phone_short_press, para:%s\n", (char *)p_parameter);
	printf("电话接通了！！！\n");
	g_iPhone_state = 1;
	
}


void * key1_up(void *p_parameter)
{
    printf("phone_up\n");

}

void * key2_first_press_down(void *p_parameter)
{
    printf("key1_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/phone/phone_receive_off.bmp" , PRIORITY_4, PICTURE);

}


void * key2_press(void *p_parameter)
{
	
	g_iPhone_state = 2;

}




int phone_receive_key_init(void){
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key1_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,key1_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,key1_press,NULL);
	key_sys_register(KEY0_UP,key1_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key1_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,key1_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,key1_press,NULL);
	key_sys_register(KEY1_UP,key1_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key2_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,key2_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,key2_press,NULL);
	key_sys_register(KEY2_UP,NULL,NULL);
	
	key_sys_register(KEY3_FIRST_PRESS_DOWN,key2_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key2_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,key2_press,NULL);
	key_sys_register(KEY3_UP,NULL,NULL);

	
	return 0;
}


static void *phone_dropped_1(void *pvArgs){
	
	g_iPhone_state = 2;
	printf("%s,%d,XXXXXXXXXXXXXXXXXXX,g_iPhone_state =%d\n",__FUNCTION__,__LINE__,g_iPhone_state);

}

static void *phone_dropped_2(void *pvArgs){
	
	printf("%s,%d,XXXXXXXXXXXXXXXXXXX,g_iPhone_state =%d\n",__FUNCTION__,__LINE__,g_iPhone_state);
	at_send_command("ATH", NULL);
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
}


void *phone_receive(void *pvArgs){
	
	g_iWhich_screen = 9000;
	
	g_iPhone_state = 0;

	phone_receive_key_init();

	printf("收到的电话%s\n",(char *)pvArgs);
	
	On_Request(NO_CALLING_HANG_UP, phone_dropped_1,NULL);

	enqueue_display(0 ,64,PART_REFRESH,"./pic/phone/phone_receive.bmp" , PRIORITY_4, PICTURE);	
	enqueue_display(20, 20 ,PART_REFRESH,(char *)pvArgs, PRIORITY_2, TEXT_ENGLISH);

	six_axis_module_init();

	while(1){
		printf("!!!!!!!!!!!!\n");
		do_taptic_engine(2, 1);
		int iSix = get_six_axis_status();
		printf("!!!!!!!!!!!!!!!!!!六轴返回值：%d\n",iSix);
		if(g_iPhone_state == 1 || iSix){
			printf("电话接通了！！！\n");
			at_send_command("ATA", NULL);
			enqueue_display(0 ,0 ,PART_REFRESH,"./pic/phone/phone_connect.bmp" , PRIORITY_4, PICTURE);
			On_Request(CALLING_HANG_UP, phone_dropped_2,NULL);
			phone_connect_key_init();
			break;
		}else if(g_iPhone_state == 2){
			at_send_command("ATH", NULL);
			pthread_mutex_lock(&stMutex_key_init);
			pthread_cond_signal(&stCond_key_init); 
			pthread_mutex_unlock(&stMutex_key_init);
			break;
		}
		sleep(2);
	}
	
}
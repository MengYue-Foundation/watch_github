#include "watch_aiworld.h"


extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;
extern int g_iWhich_screen;

void * key30_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_food.bmp" , PRIORITY_4, PICTURE);

}

void * key30_short_press(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_happy.bmp" , PRIORITY_4, PICTURE);	
}

void * key30_long_press(void *p_parameter){
	
}

void * key30_up(void *p_parameter){

}


void * key31_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_water.bmp" , PRIORITY_4, PICTURE);	
}

void * key31_short_press(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_happy.bmp" , PRIORITY_4, PICTURE);	
}

void * key31_long_press(void *p_parameter){
	
}

void * key31_up(void *p_parameter){
	
}



void * key32_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_back.bmp" , PRIORITY_4, PICTURE);	
}

void * key32_short_press(void *p_parameter){

	
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);
	
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/dack.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/white.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,64,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_4, PICTURE);

	
}

void * key32_long_press(void *p_parameter){
	
}

void * key32_up(void *p_parameter){

}



void * key33_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_chat.bmp" , PRIORITY_4, PICTURE);	
}

void * key33_short_press(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_happy.bmp" , PRIORITY_4, PICTURE);	
}

void * key33_long_press(void *p_parameter){
	
}

void * key33_up(void *p_parameter){
	
}





int aiworld_second(void){
	
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key30_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,key30_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,key30_long_press,NULL);
	key_sys_register(KEY0_UP,key30_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key31_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,key31_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,key31_long_press,NULL);
    key_sys_register(KEY1_UP,key31_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key32_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,key32_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,key32_long_press,NULL);
    key_sys_register(KEY2_UP,key32_up,NULL);

	key_sys_register(KEY3_FIRST_PRESS_DOWN,key33_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key33_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,key33_long_press,NULL);
	key_sys_register(KEY3_UP,key33_up,NULL);
	return 0;
}


void * aiworld_first_press_down(void *p_parameter)
{
    printf("aiworld_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_AI.bmp" , PRIORITY_1, 
PICTURE);
}



void * aiworld_short_press(void *p_parameter)
{
    printf("aiworld_short_press\n");

	aiworld_second();
	
	g_iWhich_screen = 1400;
	#if 1
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/dack.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/white.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AIworld.bmp" , PRIORITY_4
, PICTURE);
	#endif
	

}

void * aiworld_long_press(void *p_parameter)
{
    printf("aiworld_long_press\n");
	
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_AI.bmp" , PRIORITY_1, 
PICTURE);
	
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing_white.bmp" , PRIORITY_3, 
PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, 
PICTURE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
			
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing.bmp" , PRIORITY_3, PICTURE
);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, 
PICTURE);
			enqueue_display(133 ,10 ,PART_REFRESH,"ѱ", PRIORITY_3, TEXT_CHINESE);
			enqueue_display(133 ,25 ,PART_REFRESH,"ީ", PRIORITY_3, TEXT_CHINESE);
			enqueue_display(225, 45 ,PART_REFRESH,"25" , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
}


void * aiworld_up(void *p_parameter)
{
    printf("aiworld_up\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, 
PICTURE);

}
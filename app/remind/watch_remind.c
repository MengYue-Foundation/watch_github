#include "watch_remind.h"


extern int g_iWhich_screen;
extern remind_buf_t g_stszRemind[REMIND_SIZE];
remind_buf_t g_stszRemind_show[REMIND_SIZE];

extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;

static void * key0_first_press_down(void *p_parameter){
	printf("*******123\n");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_gaoye.bmp" , PRIORITY_4, PICTURE);

}

static void * key0_short_press(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/AI_world/AI_happy.bmp" , PRIORITY_4, PICTURE);	
}

static void * key0_long_press(void *p_parameter){

	


}

static void * key0_up(void *p_parameter){

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
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/Lark/lark_return.bmp" , PRIORITY_4, PICTURE);

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


int key_remind_init(void){
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



void * remind_first_press_down(void *p_parameter){}
void * remind_short_press(void *p_parameter){
	printf("****%s,%s\n",__FILE__,__FUNCTION__);
	int iI = 0;
	int iShow = 0;
	
	key_remind_init();
	g_iWhich_screen = 1300;
	
	bzero(&g_stszRemind_show,sizeof(remind_buf_t) * REMIND_SIZE);
	

	
	for(iI=0;iI<REMIND_SIZE;iI++){
		if(g_stszRemind[iI].iState==1){
			memcpy(&g_stszRemind_show[iShow],&g_stszRemind[iI],sizeof(remind_buf_t));
			iShow ++;
		}
	}
	
	
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/dack.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/white.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/Lark/lark_list.bmp" , PRIORITY_4, PICTURE);
	
}
void * remind_long_press(void *p_parameter){}
void * remind_up(void *p_parameter){}
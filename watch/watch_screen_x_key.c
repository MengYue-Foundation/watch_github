#include "watch.h"

int screen_5_key(void){
	key_sys_register(KEY0_FIRST_PRESS_DOWN,note_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,note_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,note_long_press,NULL);
	key_sys_register(KEY0_UP,note_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,lark_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,lark_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,lark_long_press,NULL);
	key_sys_register(KEY1_UP,lark_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,remind_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,remind_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,remind_long_press,NULL);
	key_sys_register(KEY2_UP,remind_up,NULL);

	key_sys_register(KEY3_FIRST_PRESS_DOWN,aiworld_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,aiworld_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,aiworld_long_press,NULL);
	key_sys_register(KEY3_UP,aiworld_up,NULL);

	
	return 0;	
}

int screen_4_key(void){
	key_sys_register(KEY0_FIRST_PRESS_DOWN,aiworld_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,aiworld_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,aiworld_long_press,NULL);
	key_sys_register(KEY0_UP,aiworld_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,lark_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,lark_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,lark_long_press,NULL);
	key_sys_register(KEY1_UP,lark_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,note_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,note_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,note_long_press,NULL);
	key_sys_register(KEY2_UP,note_up,NULL);

	
	return 0;	
}
int screen_3_key(void){
	
/*
	key_sys_register(KEY0_FIRST_PRESS_DOWN,weather_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,weather_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,weather_long_press,NULL);
	key_sys_register(KEY0_UP,weather_up,NULL);
*/
	key_sys_register(KEY0_FIRST_PRESS_DOWN,robot_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,robot_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,robot_long_press,NULL);
	key_sys_register(KEY0_UP,robot_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,aiworld_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,aiworld_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,aiworld_long_press,NULL);
	key_sys_register(KEY1_UP,aiworld_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,lark_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,lark_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,lark_long_press,NULL);
	key_sys_register(KEY2_UP,lark_up,NULL);
	
	key_sys_register(KEY3_FIRST_PRESS_DOWN,note_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,note_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,note_long_press,NULL);
	key_sys_register(KEY3_UP,note_up,NULL);

	
	return 0;
}

int screen_2_key(void){

	key_sys_register(KEY0_FIRST_PRESS_DOWN,short_message_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,short_message_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,short_message_long_press,NULL);
	key_sys_register(KEY0_UP,short_message_up,NULL);
/*	
	key_sys_register(KEY1_FIRST_PRESS_DOWN,weather_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,weather_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,weather_long_press,NULL);
	key_sys_register(KEY1_UP,weather_up,NULL);
*/
	key_sys_register(KEY1_FIRST_PRESS_DOWN,robot_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,robot_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,robot_long_press,NULL);
	key_sys_register(KEY1_UP,robot_up,NULL);


	key_sys_register(KEY2_FIRST_PRESS_DOWN,aiworld_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,aiworld_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,aiworld_long_press,NULL);
	key_sys_register(KEY2_UP,aiworld_up,NULL);
	
	key_sys_register(KEY3_FIRST_PRESS_DOWN,lark_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,lark_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,lark_long_press,NULL);
	key_sys_register(KEY3_UP,lark_up,NULL);

	
	return 0;
}
int screen_1_key(void){
	key_sys_register(KEY0_FIRST_PRESS_DOWN,phone_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,phone_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,phone_long_press,NULL);
	key_sys_register(KEY0_UP,phone_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,short_message_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,short_message_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,short_message_long_press,NULL);
	key_sys_register(KEY1_UP,short_message_up,NULL);

/*
	key_sys_register(KEY2_FIRST_PRESS_DOWN,weather_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,weather_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,weather_long_press,NULL);
	key_sys_register(KEY2_UP,weather_up,NULL);

*/
	key_sys_register(KEY2_FIRST_PRESS_DOWN,robot_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,robot_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,robot_long_press,NULL);
	key_sys_register(KEY2_UP,robot_up,NULL);
	
	key_sys_register(KEY3_FIRST_PRESS_DOWN,aiworld_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,aiworld_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,aiworld_long_press,NULL);
	key_sys_register(KEY3_UP,aiworld_up,NULL);

	
	return 0;
}
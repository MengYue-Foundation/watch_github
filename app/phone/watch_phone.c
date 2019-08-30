#include "watch_phone.h"

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

	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_phone.bmp" , PRIORITY_1, PICTURE);

	watch_asr_t stWatchAsr;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);

	watch_phone_call(stWatchAsr.cszVtoT);

	return 0;
}

void * phone_up(void *p_parameter)
{
    printf("phone_up\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);
}


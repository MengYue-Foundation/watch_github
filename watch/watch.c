#include "watch.h"void  Handler(int signo){    //System Exit    printf("\r\nHandler:Goto Sleep mode\r\n");    EPD_Sleep();    DEV_ModuleExit();    exit(0);}int watch_init(watch_t *pstWatch){	bzero(pstWatch,sizeof(watch_t));	return 0;}void * key0_short_press(void *p_parameter){    printf("key0_short_press, para:%s\n", (char *)p_parameter);}void * key0_long_press(void *p_parameter){    printf("key0_long_press\n");	//UBYTE *BlackImage = (UBYTE *)p_parameter ;	watch_asr_t stWatchAsr;	watch_weather_t stWatchWeather;	asr_init(&stWatchAsr);	asr_server(&stWatchAsr);	printf("*****************************1111111\n");	weather_init(&stWatchWeather);	weather_server(&stWatchWeather,stWatchAsr.cszVtoT);	return 0;}int watch_exec(watch_t *pstWatch){	key_module_init();	key_sys_register(KEY0_SHORT_PRESS,key0_short_press,pstWatch);	key_sys_register(KEY0_LONG_PRESS,key0_long_press,pstWatch);	while(1){		sleep(1);	}}
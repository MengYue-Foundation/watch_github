#include "watch.h"void  Handler(int signo){    //System Exit    printf("\r\nHandler:Goto Sleep mode\r\n");    EPD_Sleep();    DEV_ModuleExit();    exit(0);}int watch_init(watch_t *pstWatch){	bzero(pstWatch,sizeof(watch_t));	return 0;}void * key0_short_press(void *p_parameter){    printf("key0_short_press, para:%s\n", (char *)p_parameter);}void * key0_long_press(void *p_parameter){    printf("key0_long_press\n");	//UBYTE *BlackImage = (UBYTE *)p_parameter ;	watch_asr_t stWatchAsr;	watch_weather_t stWatchWeather;	asr_init(&stWatchAsr);	asr_server(&stWatchAsr);	weather_init(&stWatchWeather);	weather_server(&stWatchWeather,stWatchAsr.cszVtoT);	return 0;}void * key1_short_press(void *p_parameter){    printf("key1_short_press, para:%s\n", (char *)p_parameter);}void * key1_long_press(void *p_parameter){    printf("key1_long_press\n");	//UBYTE *BlackImage = (UBYTE *)p_parameter ;  enqueue_display(0 ,64,PART_REFRESH,"./pic/standby_phone.bmp" , PRIORITY_1, PICTURE);	watch_asr_t stWatchAsr;	asr_init(&stWatchAsr);	asr_server(&stWatchAsr);	watch_phone_call(stWatchAsr.cszVtoT);	return 0;}void * key1_up(void *p_parameter){	printf("1111111111111111111111\n");    enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);  return 0;}void * key2_short_press(void *p_parameter){    printf("key2_short_press, para:%s\n", (char *)p_parameter);}#if 0void * key2_long_press(void *p_parameter){    printf("key2_long_press\n");	//UBYTE *BlackImage = (UBYTE *)p_parameter ;		watch_t *pstWatch = (watch_t *)p_parameter;	watch_asr_t stWatchAsr;	asr_init(&stWatchAsr);	asr_server(&stWatchAsr);	if(pstWatch->stWatchMesg.iState == MESG_CONTENT){		strncpy(pstWatch->stWatchMesg.cszMesg,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));		pstWatch->stWatchMesg.iState = MESG_NAME;		printf("speak name!!!!\n");	}else if(pstWatch->stWatchMesg.iState == MESG_NAME){		strncpy(pstWatch->stWatchMesg.cszName,stWatchAsr.cszVtoT,strlen(stWatchAsr.cszVtoT));		watch_mesg_send(&pstWatch->stWatchMesg);		pstWatch->stWatchMesg.iState = MESG_CONTENT;		printf("speak content!!!!\n");	}	return 0;}#endifvoid * key2_up(void *p_parameter){	printf("key2_up\n");    //enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);  return 0;}void * key2_long_press(void *p_parameter){    printf("key2_long_press\n");}int watch_exec(watch_t *pstWatch){		key_module_init();	#if 0	key_sys_register(KEY0_SHORT_PRESS,key0_short_press,pstWatch);	key_sys_register(KEY0_LONG_PRESS,key0_long_press,pstWatch);	key_sys_register(KEY1_SHORT_PRESS,key1_short_press,pstWatch);	key_sys_register(KEY1_LONG_PRESS,key1_long_press,pstWatch);    key_sys_register(KEY1_UP,key1_up,pstWatch);#endif	//key_sys_register(KEY2_SHORT_PRESS,key2_short_press,pstWatch);	//key_sys_register(KEY2_LONG_PRESS,key2_long_press,pstWatch);    key_sys_register(KEY2_UP,key2_up,pstWatch);	//key_sys_register(KEY3_UP,key3_up,pstWatch);	//key_sys_register(KEY3_LONG_PRESS,key3_long_press,pstWatch);	//display_module_init();	//enqueue_display(0 ,0 , FULL_REFRESH,"./pic/standby.bmp" , PRIORITY_1, PICTURE);	while(1){		sleep(1);	}}
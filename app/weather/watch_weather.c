#include "watch_weather.h"

void * weather_first_press_down(void *p_parameter)
{
    printf("weather_first_press_down\n");
	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_home.bmp" , PRIORITY_1, PICTURE);
}

void * weather_short_press(void *p_parameter)
{
    printf("weather_short_press, para:%s\n", (char *)p_parameter);
}

static int show_weather(char * pcWeather,char *pcCity1,char *pcCity2,char *pcTem){
		char cszTem[5]={0};
		char cszCity1[5]={0};
		char cszCity2[5]={0};
		charset_convert("UTF-8","GB2312",pcTem,strlen("pcTem"),cszTem,5);
		charset_convert("UTF-8","GB2312",pcCity1,strlen("pcTem"),cszCity1,5);
		charset_convert("UTF-8","GB2312",pcCity2,strlen("pcTem"),cszCity2,5);
		if(!strcmp("晴",pcWeather)){
			printf("1111111111111111\n");
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing_white.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
			
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(133 ,10 ,PART_REFRESH,cszCity1, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(133 ,25 ,PART_REFRESH,cszCity2 , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(225, 45 ,PART_REFRESH,cszTem , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
		}else if(!strcmp("阴",pcWeather)){
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/yin_white.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
			
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/yin.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(133 ,10 ,PART_REFRESH,cszCity1, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(133 ,25 ,PART_REFRESH,cszCity2, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(225, 45 ,PART_REFRESH,cszTem , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);		
		}else if(!strcmp("多云",pcWeather)){
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/duoyun_white.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
			
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/duoyun.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(133 ,10 ,PART_REFRESH,cszCity1, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(133 ,25 ,PART_REFRESH,cszCity2, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(225, 45 ,PART_REFRESH,cszTem , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
		}else{
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing_white.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
			
			enqueue_display(130 ,5 ,PART_REFRESH,"./pic/qing.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_3, PICTURE);
			enqueue_display(133 ,10 ,PART_REFRESH,cszCity1, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(133 ,25 ,PART_REFRESH,cszCity2, PRIORITY_3, TEXT_CHINESE);
			enqueue_display(225, 45 ,PART_REFRESH,cszTem , PRIORITY_3, TEXT_CHINESE);
			enqueue_display(130 ,5 ,PART_REFRESH,"end" , PRIORITY_3, PICTURE);
		}
		
		return 0;
		
}


void * weather_long_press(void *p_parameter)
{
    printf("weather_long_press\n");

	enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_home.bmp" , PRIORITY_1, PICTURE);

	
	watch_asr_t stWatchAsr;
	watch_weather_t stWatchWeather;

	asr_init(&stWatchAsr);
	asr_server(&stWatchAsr);

	weather_init(&stWatchWeather);
	weather_server(&stWatchWeather,stWatchAsr.cszVtoT);
	
	if(!strcmp("北京",stWatchAsr.cszVtoT)){
		show_weather(stWatchWeather.stWeather.cszText,"北","京",stWatchWeather.stWeather.cszTem);
	}else if(!strcmp("上海",stWatchAsr.cszVtoT)){
		show_weather(stWatchWeather.stWeather.cszText,"上","海",stWatchWeather.stWeather.cszTem);
	}else if(!strcmp("深圳",stWatchAsr.cszVtoT)){
		show_weather(stWatchWeather.stWeather.cszText,"深","圳",stWatchWeather.stWeather.cszTem);
	}else if(!strcmp("兰州",stWatchAsr.cszVtoT)){
		show_weather(stWatchWeather.stWeather.cszText,"兰","州",stWatchWeather.stWeather.cszTem);
	}else{
		show_weather(stWatchWeather.stWeather.cszText,"北","京",stWatchWeather.stWeather.cszTem);
	}

	return 0;
}


void * weather_up(void *p_parameter){
	printf("weather_up\n");
    enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);
  return 0;
}
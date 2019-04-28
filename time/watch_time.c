#include "watch_time.h"

int watch_currenttime(watchTime_t *pstWatchTime){
	time_t tTime;
    struct tm *pstTime;
    time(&tTime);
    pstTime=localtime(&tTime);

	pstWatchTime->iYear=1900+pstTime->tm_year;
	pstWatchTime->iMounth=1+pstTime->tm_mon;

    pstWatchTime->iHour=(pstTime->tm_hour+8)%24;
    if(pstTime->tm_hour+8>=24){
        pstWatchTime->iDay=pstTime->tm_mday+1;
    }
	
	pstWatchTime->iDay=pstTime->tm_mday;
	
	pstWatchTime->iMin=pstTime->tm_min;
	pstWatchTime->iSec=pstTime->tm_sec;
	return 0;
}

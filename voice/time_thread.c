#include "time_thread.h"

void *time_thread(void *args){
    time_t stTime;
    struct tm* pstTime;

    time(&stTime);
    pstTime=localtime(&stTime);
    printf("year:%02d month:%02d day:%02d \n%02d:%02d:%02d",pstTime->tm_year,pstTime->tm_mon,pstTime->tm_mday,pstTime->tm_hour,pstTime->tm_min,pstTime->tm_sec);

}

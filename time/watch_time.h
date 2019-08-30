#ifndef __WATCH_TIME_H__
#define __WATCH_TIME_H__

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct watchTime{
	int iYear;
	int iMounth;
	int iDay;
	int iHour;
	int iMin;
	int iSec;
}watchTime_t;

int watch_currenttime(watchTime_t *pstWatchTime);
#endif

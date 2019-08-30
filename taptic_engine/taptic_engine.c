#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h> 
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "taptic_engine.h"

pthread_mutex_t  taptic_data_status_chage_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  queue_taptic_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tapic_data_status_change_cond = PTHREAD_COND_INITIALIZER;

taptic_engine_info_t g_taptic_engine_info;

int iHave_thread_create = false;
pthread_t created_thread_tid;

int g_iStart = 0;

int vibrate_mode1(int iTimes)
{
	int i = 0;
	int j =0;
	
	for (i = 0; i < iTimes; i++) {	
		for (i = 0; i < 3; i++) {
		digitalWrite(VIBRATE_PIN, 1);
		delay(150);
		digitalWrite(VIBRATE_PIN, 0);
		delay(100);


		}
		delay(1000);
	}
	return 0;
}

int vibrate_mode2(int iTimes)
{
	int i = 0;
	for (i = 0; i < iTimes; i++) {
		digitalWrite(VIBRATE_PIN, 1);
		delay(1000);
		digitalWrite(VIBRATE_PIN, 0);
		delay(800);
	}
	return 0;
}

int vibrate_mode3()
{
	int i = 0;
	digitalWrite(VIBRATE_PIN, 1);
	return 0;
}

void * start_vibrate (void *p_arg)
{
	taptic_engine_info_t *p_taptic_engine_info = (taptic_engine_info_t *)p_arg;
	int iMode = p_taptic_engine_info->iMode;
	int iTimes = p_taptic_engine_info->iTimes;
	//printf("g_iStart:%d, iMode:%d\n", g_iStart++, iMode);
	if (MODE1 == iMode) {
		vibrate_mode1(iTimes);
	} else if (MODE2 == iMode) {
		vibrate_mode2(iTimes);
	} else if (MODE3 == iMode) {
		vibrate_mode3();
	}
	
}

void * handle_tapic_engine_queue_thread (void *p_arg)
{
	
	taptic_engine_info_t sz_taptic_engine_info[LENGTH];
	int iHave_data = false;
	int iRet = 0;
	
    struct timespec outtime;
	while (1) {
        clock_gettime(CLOCK_REALTIME, &outtime);
        outtime.tv_nsec = outtime.tv_nsec + PER_INC_TIME_NS;
        //outtime.tv_sec = outtime.tv_sec + 2;
        pthread_mutex_lock(&taptic_data_status_chage_mutex);
        pthread_cond_timedwait(&tapic_data_status_change_cond, &taptic_data_status_chage_mutex, &outtime);
        pthread_mutex_unlock(&taptic_data_status_chage_mutex);

		if (true == g_taptic_engine_info.iHave_data) {
			/*之前有线程创建， 得取消掉去*/
			if (true == iHave_thread_create) {
				pthread_cancel(created_thread_tid);
				pthread_join(created_thread_tid, NULL);	
			}
			iRet = pthread_create(&created_thread_tid,NULL, 
				start_vibrate, 
				(void *)(&g_taptic_engine_info));
			g_taptic_engine_info.iHave_data = false;
			
		}
		
	}
}
int taptic_engine_module_init(void)
{
	
    pinMode(19, OUTPUT);
    pinMode(VIBRATE_PIN, OUTPUT);
	
	pthread_t tid;
	int iRet = 0;
    iRet = pthread_create(&tid,NULL, handle_tapic_engine_queue_thread, NULL);
    if (iRet != 0) {
        printf("create handle_queue_thread failed!\n");
    }
	return 0;
}

int start_taptic_engine(taptic_engine_info_t *p_taptic_engine_info)
{
	int i = 0;
	int iTotal_num = 0;
	
	pthread_mutex_lock(&queue_taptic_mutex);
	memcpy(&g_taptic_engine_info, p_taptic_engine_info, sizeof(g_taptic_engine_info));
	g_taptic_engine_info.iHave_data = true;	
	pthread_mutex_unlock(&queue_taptic_mutex);


	pthread_mutex_lock(&taptic_data_status_chage_mutex);
	pthread_cond_signal(&tapic_data_status_change_cond);
	pthread_mutex_unlock(&taptic_data_status_chage_mutex);
	return 0;
}


int do_taptic_engine(int iMode, int iTimes)
{
	taptic_engine_info_t taptic_engine_info;

	taptic_engine_info.iHave_data = false;
	taptic_engine_info.iMode = iMode;
	taptic_engine_info.iTimes = iTimes;
	start_taptic_engine(&taptic_engine_info);
	return 0;
}

//#define SGY_DEBUG 
#ifdef SGY_DEBUG

int main ()
{
	
    if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
        printf("set wiringPi lib failed	!!! \r\n");
        return 1;
    }
	taptic_engine_module_init();
	sleep(2);
	taptic_engine_info_t taptic_engine_info;

	taptic_engine_info.iMode = MODE1;
	taptic_engine_info.iTimes= 6;
	taptic_engine_info.iHave_data = false;

	start_taptic_engine(&taptic_engine_info);

	sleep(2);

	taptic_engine_info.iMode = MODE3;
	taptic_engine_info.iTimes= 2;
	taptic_engine_info.iHave_data = false;

	start_taptic_engine(&taptic_engine_info);
	sleep(2);

	
	taptic_engine_info.iMode = MODE2;
	taptic_engine_info.iTimes= 2;
	taptic_engine_info.iHave_data = false;

	start_taptic_engine(&taptic_engine_info);
	while (1);
	return 0;
}

#endif


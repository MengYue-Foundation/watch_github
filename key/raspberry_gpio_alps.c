#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <raspberry_gpio_alps.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

alps_data_need_transfer_t st_alps_data_need_transfer;

 //printf("level_28_A:%d, level_29_B:%d\n", level_28_A, level_29_B);
int iCurrent_state = IDLE;//这是片刻性的
int iPrev_state = IDLE;
int iReal_state = IDLE;//实际的状态，持续一段时间的状态，以这个状态为准
int level_28_A_cur = OFF;
int level_29_B_cur = OFF;

int level_28_A_pre = OFF;
int level_29_B_pre = OFF;
int iRotate_counter = 0;
int g_iNum_of_trigger_cw = 1;
int g_iNum_of_trigger_ccw = 1;

int ialps_have_data = false;
pthread_mutex_t  alps_have_data_mutex = PTHREAD_MUTEX_INITIALIZER;

alps_need_reg_data_t alps_need_reg_data_cb;


#if 1
    int iCounter_10ms = 0;
#endif

void alps_sys_register(int num_of_trigger, void *(*start_routine) (void *), void *p_parameter, int direction) 
{
    if (CW == direction) {
        g_iNum_of_trigger_cw = num_of_trigger;
    } else if (CCW == direction) {
        g_iNum_of_trigger_ccw = num_of_trigger;
    }
    if (start_routine != NULL) {
        alps_need_reg_data_cb.p_alps_reg_data[direction] = (alps_func_callback_t*)start_routine;
        alps_need_reg_data_cb.p_szParameter[direction] = p_parameter;
        alps_need_reg_data_cb.is_reg[direction] = true;
    }
}

int check_direc_stage() 
{

    if (level_28_A_pre == OFF && ON == level_28_A_cur) {
        return CW_JUDGE;
    } 

    if (OFF == level_29_B_pre && ON == level_29_B_cur) {
        return CCW_JUDGE;
    }
    return false;
}

int check_cw_stage()
{
    if (level_29_B_pre == OFF && ON == level_29_B_cur) {
        return true;
    } else {
        return false;
    }
}

int check_ccw_stage()
{
    if (level_28_A_pre == OFF && ON == level_28_A_cur) {
        return true;
    } else {
        return false;
    }
}

int check_still_keep()
{
    static int iIdle_counter_10ms = 0;
    if (OFF == level_28_A_cur && OFF == level_29_B_cur) {
        iIdle_counter_10ms++;
        if (iIdle_counter_10ms >= 40) {
            iIdle_counter_10ms = 0;
            return false;
        } else {
            return true;
        }
    }

    return true;
}

int alps_data_read()
{
    
    int iRet = false;
    level_28_A_pre = level_28_A_cur;
    level_29_B_pre = level_29_B_cur;
    
    level_28_A_cur = digitalRead(PIN_A);
    level_29_B_cur = digitalRead(PIN_B);

    switch (iCurrent_state) {
    case IDLE:
        if (iReal_state != IDLE) {
            iRet = check_still_keep();
            if (false == iRet) {
                iReal_state = IDLE;
                iCurrent_state = IDLE;
                iRotate_counter = 0;
                pthread_mutex_lock(&alps_have_data_mutex);
                ialps_have_data = false;            
                pthread_mutex_unlock(&alps_have_data_mutex);
            }
        } 
        //每一次IDLE的时候都应该判断的
        iRet = check_direc_stage();//判断是不是进入方向判断，顺时针还是逆时针
        //printf("iRet:%d\n", iRet);
        
        if (CW_JUDGE == iRet) {
            iCurrent_state = CW_JUDGE;           
        } else if (CCW_JUDGE == iRet) {
            iCurrent_state = CCW_JUDGE;
        }


        
        break;
    case CW_JUDGE: //顺时针方向预判断
        if (iReal_state != IDLE) {
            iRet = check_still_keep();
            if (false == iRet) {
                iReal_state = IDLE;
                iCurrent_state = IDLE;
                iRotate_counter = 0;
                pthread_mutex_lock(&alps_have_data_mutex);
                ialps_have_data = false;            
                pthread_mutex_unlock(&alps_have_data_mutex);
            }
        } 
        iRet = check_cw_stage();    
        if (true == iRet) {
            iCurrent_state = CW_KEEP;           
        }
        break;
    case CCW_JUDGE:
        if (iReal_state != IDLE) {
            iRet = check_still_keep();
            if (false == iRet) {
                iReal_state = IDLE;
                iCurrent_state = IDLE;
                iRotate_counter = 0;
                pthread_mutex_lock(&alps_have_data_mutex);
                ialps_have_data = false;            
                pthread_mutex_unlock(&alps_have_data_mutex);
            }
        } 

        iRet = check_ccw_stage();    
        if (true == iRet) {
            iCurrent_state = CCW_KEEP;           
        }
        break;
    case CW_KEEP: //顺时针方向判断完成， 看一看是不是还是保持的
    case CCW_KEEP: //顺时针方向判断完成， 看一看是不是还是保持的
        iRet = check_still_keep();
        if (false == iRet) {
            iReal_state = IDLE;
            iCurrent_state = IDLE;
            iRotate_counter = 0;
            pthread_mutex_lock(&alps_have_data_mutex);
            ialps_have_data = false;            
            pthread_mutex_unlock(&alps_have_data_mutex);
        } else if (true == iRet) {
            iRotate_counter++;
            iReal_state = iCurrent_state;
            iCurrent_state = IDLE;
        }//不管转的有多快，都会出现一个问题， 就是他们都会两个电平变成OFF一段时间
        
        break;
        
    default:
        break;
    }


    return iReal_state;
}
void * alps_sensor_is_change_thread (void *p_arg)
{
    int iPrev_state = IDLE;
    int iReal_state = IDLE;
    while (1) {
        iReal_state = alps_data_read();        
        pthread_mutex_lock(&alps_have_data_mutex); 
    
        alps_data_need_transfer_t *p_alps_data = (alps_data_need_transfer_t*)(p_arg);
        if (iReal_state == CCW_KEEP || iReal_state == CW_KEEP) {
            p_alps_data->iCurrent_state = iReal_state;
            ialps_have_data = true;
        }

#if 0        
        iCounter_10ms++;
        if (10 == iCounter_10ms) {
            printf("iCurrent_state:%d, ialps_have_data:%d, iRotate_counter:%d\n", iCurrent_state, ialps_have_data, iRotate_counter);
            iCounter_10ms = 0;
        }
#endif        
        pthread_mutex_unlock(&alps_have_data_mutex);
        delay(10);
        
    }
}

void * handle_alps_sensor_data_thread (void *p_arg)
{
    
    pthread_t tid;
    int iCurrent_state = IDLE;
    while (1) {
        //pthread_mutex_lock(&alps_have_data_mutex);
        alps_data_need_transfer_t *p_alps_data = (alps_data_need_transfer_t*)(p_arg);
        iCurrent_state = p_alps_data->iCurrent_state;
        if (true == ialps_have_data) {
            
            if (CW_KEEP == iCurrent_state) {
//                printf("iRotate_counter:%d, g_iNum_of_trigger_cw:%d, iCurrent_state:%d\n", iRotate_counter, g_iNum_of_trigger_cw, iCurrent_state);
#if 1
                if ((0 == (iRotate_counter % g_iNum_of_trigger_cw) && (iRotate_counter >= g_iNum_of_trigger_cw))) {
                    if (true == alps_need_reg_data_cb.is_reg[CW]) {
                        printf("we should call  CW_KEEP callback function, iRotate_counter:%d\n", iRotate_counter);
                        iRotate_counter = 0;    
                        pthread_create(&tid,NULL, 
                            alps_need_reg_data_cb.p_alps_reg_data[CW], 
                            alps_need_reg_data_cb.p_szParameter[CW]);
                    }
                }
#endif
            } else if (CCW_KEEP == iCurrent_state) {
                    //printf("iRotate_counter:%d, g_iNum_of_trigger_ccw:%d\n", iRotate_counter, g_iNum_of_trigger_ccw);

                if ((0 == (iRotate_counter % g_iNum_of_trigger_ccw)) && (iRotate_counter >= g_iNum_of_trigger_ccw)) {
                    if (true == alps_need_reg_data_cb.is_reg[CCW]) {
                        printf("we should call  CCW_KEEP callback function, iRotate_counter:%d\n", iRotate_counter);
                        iRotate_counter = 0;    
                        pthread_create(&tid,NULL, 
                            alps_need_reg_data_cb.p_alps_reg_data[CCW], 
                            alps_need_reg_data_cb.p_szParameter[CCW]);
                    }
                }
            }
        }
        //pthread_mutex_unlock(&alps_have_data_mutex);    
        delay(10);
    }
}
void * test (void *p_arg)
{
    printf("str:%s\n", (char *)p_arg);
    return (void *)0;
}

int alps_rotate_module_init()
{
    int iRet = 0;
    pthread_t tid_detect_key_is_change;
    pthread_t tid_handle_key_data;
    
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);
#if 0
    key_data_need_transfer_t st_Key_data_need_transfer;//多定义了一个数组
    memset(&st_Key_data_need_transfer, 0, sizeof(st_Key_data_need_transfer));
    memset(&key_need_reg_data_cb, 0, sizeof(key_need_reg_data_cb));
#endif
    memset(&st_alps_data_need_transfer, 0, sizeof(st_alps_data_need_transfer));
    memset(&alps_need_reg_data_cb, 0, sizeof(alps_need_reg_data_cb));
    iRet = pthread_create(&tid_detect_key_is_change,NULL, alps_sensor_is_change_thread, (void *)&st_alps_data_need_transfer);
    if (iRet != 0) {
        printf("create key_data_is_change_thread failed!\n");
    }

    iRet = pthread_create(&tid_handle_key_data,NULL, handle_alps_sensor_data_thread, (void *)&st_alps_data_need_transfer);
    if (iRet != 0) {
        printf("create key_data_is_change_thread failed!\n");
    }
#if 0    
    sleep(3);
    alps_sys_register(3, test, "123sgy", CW);
    while (1);
#endif
    return 0;
}

#if 0
int main() {
    wiringPiSetup();
    alps_rotate_module_init();

    return 0;
}
#endif

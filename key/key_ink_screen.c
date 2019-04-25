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
#include <key_ink_screen.h>


pthread_mutex_t  key_data_status_change_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t key_data_status_change_cond = PTHREAD_COND_INITIALIZER;
key_need_reg_data_t key_need_reg_data_cb;
static int32_t i2c_smbus_write_byte(int fd, int val);
static int32_t i2c_smbus_access(int fd, char read_write, int cmd,
				int size, union i2c_smbus_data *data);

int key_init(void)
{
    int iRet = 0;
    int iFd = open(TTP229_I2C_BUS_NAME, O_RDWR);
    
	if (iRet < 0) {
		printf("can't open '%s'\n", TTP229_I2C_BUS_NAME);
        return -1;
	}

    iRet = ioctl(iFd, I2C_SLAVE, TTP229_ADDR);
	if (iRet < 0) {
		printf("can't open '%s'\n", TTP229_I2C_BUS_NAME);
        return -1;
	}
    return iFd;
}

int which_key_press(int iFd, key_read_i2c_data_t *key_read_i2c_data)
{
    int iData = 0;
    static int iPrev_status = 0x0;
    static int iNow_status = 0x0;
    int iKey_status = UP;
    int iStatus_change = NO;
    char szBuf[2] = {0};
    enum Key_index key_index = NO_KEY_PRESS;
    
    i2c_smbus_write_byte(iFd, 0x57);
    szBuf[0] = 0;
    szBuf[1] = 0;
    read(iFd, szBuf, 2);
    iData = (szBuf[1] << 8) | (szBuf[0]);
    //printf("buf[0]:0x%x, buf[1]:0x%x, iData:0x%x\n", szBuf[0], szBuf[1], iData);
    int per = 0;
	iData &= 0xffff;

    /*这个地方是检测按键是否发生变化的通知*/
    iPrev_status = iNow_status;
    iNow_status = 0x0;//表示没有按键按下
	switch(iData)
	{	
		case 0x0001:printf("KEY7 PRESS\r\n");break;
		case 0x0002:printf("KEY6 PRESS\r\n");break;
		case 0x0004:printf("KEY5 PRESS\r\n");break;
		case 0x0008:printf("KEY4 PRESS\r\n");break;
		case 0x0010:
            //printf("KEY3 PRESS\r\n");
            key_index = KEY3;
            iNow_status |= (0x1 << key_index);
            iKey_status = DOWN;
            break;
		case 0x0020:
            //printf("KEY2 PRESS\r\n");
            key_index = KEY2; 
            iNow_status |= (0x1 << key_index);
            iKey_status = DOWN;
        break;
		case 0x0040:
            //printf("KEY1 PRESS\r\n");
            key_index = KEY1;
        
            iNow_status |= (0x1 << key_index);
            iKey_status = DOWN;
            break;
		case 0x0080:
            //printf("KEY0 PRESS\r\n");
            key_index = KEY0;
        
            iNow_status |= (0x1 << key_index);
            iKey_status = DOWN;
            break;
		case 0x0100:per=100;printf("100%%\r\n");break;
        case 0x0300:per=93;printf("93%%\r\n");break;
		case 0x0200:per=86;printf("86%%\r\n");break;
        case 0x0600:per=79;printf("79%%\r\n");break;
		case 0x0400:per=71;printf("71%%\r\n");break;
        case 0x0C00:per=64;printf("64%%\r\n");break;
		case 0x0800:per=57;printf("57%%\r\n");break;
        case 0x1800:per=50;printf("50%%\r\n");break;
		case 0x1000:per=43;printf("43%%\r\n");break;
        case 0x3000:per=36;printf("36%%\r\n");break;
		case 0x2000:per=29;printf("29%%\r\n");break;
        case 0x6000:per=21;printf("21%%\r\n");break;
		case 0x4000:per=14;printf("14%%d\r\n");break;
        case 0xC000:per=7;printf("7%%d\r\n");break;
		case 0x8000:per=0;printf("0%%d\r\n");break;
		default:
		    //printf("No KEY Press  per=%d\r\n",per);
		    iNow_status = 0x0;
            iKey_status = UP;
            if (iPrev_status & 0x1) {
                key_index = KEY0;
            }
            break;	
	}	

    if (iPrev_status != iNow_status) {
        iStatus_change = YES;
    } else if (iPrev_status == iNow_status) {
        iStatus_change = NO;
    }

    key_read_i2c_data->iKey_index = key_index;
    key_read_i2c_data->iPress_Status = iKey_status;
    key_read_i2c_data->iStatus_change = iStatus_change;
    //printf("iStatus_change�?d, iPrev_status:0x%x, iNow_status:0x%x, iData:0x%x\n", iStatus_change, iPrev_status, iNow_status, iData);

    return 0;
}
void * key_data_is_change_thread (void *p_arg)
{
    int iRet = 0;
    int iKey_index = NO_KEY_PRESS;
    key_read_i2c_data_t key_read_i2c_data;
    key_data_need_transfer_t *p_key_data = (key_data_need_transfer_t*)(p_arg);
    iRet = key_init();
    if (iRet < 0) {
        printf("key_init failed!\n");
    }
    while (1) {
        which_key_press(iRet, &key_read_i2c_data);
        iKey_index = key_read_i2c_data.iKey_index;
        if (iKey_index >= KEY0 && iKey_index <= KEY3 && key_read_i2c_data.iStatus_change == YES) {
            pthread_mutex_lock(&key_data_status_change_mutex);
            p_key_data->sz_per_key_data[iKey_index].iPress_Status = key_read_i2c_data.iPress_Status;
            p_key_data->iCurrent_key_index = key_read_i2c_data.iKey_index;
            //printf("key status changed!\n");
            pthread_cond_signal(&key_data_status_change_cond);
            pthread_mutex_unlock(&key_data_status_change_mutex);
        }
        //usleep(1000*1000);
        usleep(10*1000);
    }
}


void * handle_key_data_thread (void *p_arg)
{
    int iTime_cnt = 0;
    struct timespec outtime;
    int iKey_have_pressed = UP;//用来标记之前是有按下的动作发生的
    pthread_t tid_first_down;
    pthread_t tid_short_press;
    pthread_t tid_long_press;
    pthread_t tid_press_up;
    key_data_need_transfer_t *p_key_data = (key_data_need_transfer_t*)(p_arg);
    while (1) {
        
        clock_gettime(CLOCK_REALTIME, &outtime);
        outtime.tv_nsec = outtime.tv_nsec + PER_INC_TIME_NS;
        //outtime.tv_sec = outtime.tv_sec + 2;
        pthread_mutex_lock(&key_data_status_change_mutex);
        pthread_cond_timedwait(&key_data_status_change_cond, &key_data_status_change_mutex, &outtime);//超时函数
        pthread_mutex_unlock(&key_data_status_change_mutex);

        int i = 0;
        
        pthread_mutex_lock(&key_data_status_change_mutex);        
        int iCurrent_handle_key_index = p_key_data->iCurrent_key_index;
        //printf("iTime_cnt:%d\n", iTime_cnt);
        int iCurrent_key_status = p_key_data->sz_per_key_data[iCurrent_handle_key_index].iPress_Status;
        if (iCurrent_key_status == DOWN && iTime_cnt < LONG_PRESS_TIME) {//第一次进入这个函数
            /*刚按下的时候应该调用注册的回调函数*/
            if (0 == iTime_cnt) {
                
                iKey_have_pressed = DOWN;
                /*
                    如果是注册了回调函数就调用
                */
                check_key_call_back(iCurrent_handle_key_index, FIRST_PRESS_DOWN);
            }
            iTime_cnt++;
        } else if (iCurrent_key_status == DOWN && iTime_cnt == LONG_PRESS_TIME) {//判断是不是长按
            iTime_cnt++;
            check_key_call_back(iCurrent_handle_key_index, LONG_PRESS);
        }

        /*
            如果是抬起的话，应该只进一次的
        */       
        if (iCurrent_key_status == UP && iKey_have_pressed == DOWN) {
            int iRet = 0;
            iKey_have_pressed = UP;
            //printf("we should call  UP function\n");
            
            check_key_call_back(iCurrent_handle_key_index, PRESS_UP);

            /*判断是短按的抬起还是长按的抬起*/
            if (iTime_cnt < LONG_PRESS_TIME) {
                check_key_call_back(iCurrent_handle_key_index, SHORT_PRESS);
            } 

            
            iTime_cnt = 0;

        }

        pthread_mutex_unlock(&key_data_status_change_mutex);
        
    }
 
        

}


int key_module_init()
{
    int iRet = 0;
    pthread_t tid_detect_key_is_change;
    pthread_t tid_handle_key_data;
    key_data_need_transfer_t st_Key_data_need_transfer;
    memset(&st_Key_data_need_transfer, 0, sizeof(st_Key_data_need_transfer));
    memset(&key_need_reg_data_cb, 0, sizeof(key_need_reg_data_cb));

    iRet = pthread_create(&tid_detect_key_is_change,NULL, key_data_is_change_thread, (void *)&st_Key_data_need_transfer);
    if (iRet != 0) {
        printf("create key_data_is_change_thread failed!\n");
    }

    iRet = pthread_create(&tid_handle_key_data,NULL, handle_key_data_thread, (void *)&st_Key_data_need_transfer);
    if (iRet != 0) {
        printf("create key_data_is_change_thread failed!\n");
    }
    return 0;
}


static int32_t i2c_smbus_write_byte(int fd, int val)
{
	return i2c_smbus_access(fd, I2C_SMBUS_WRITE,
				val, I2C_SMBUS_BYTE, NULL);
}


static int32_t i2c_smbus_access(int fd, char read_write, int cmd,
				int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = cmd;
	args.size = size;
	args.data = data;

	return ioctl(fd, I2C_SMBUS, &args);
}


void key_sys_register(int iAction_id, void *(*start_routine) (void *), void *p_parameter) 
{
    if (start_routine != NULL) {
        key_need_reg_data_cb.p_key_reg_data[iAction_id] = (key_func_callback_t*)start_routine;
        key_need_reg_data_cb.p_szParameter[iAction_id] = p_parameter;
        key_need_reg_data_cb.is_reg[iAction_id] = true;
    }
}


void key_sys_deregister(int iAction_id) 
{ 
    key_need_reg_data_cb.is_reg[iAction_id] = false; 
}


int find_action_id(int iCurrent_key_index, int iActionOffset)
{
    int iRet = 0;
    iRet = iCurrent_key_index * ACTION_NUM_PER_KEY + iActionOffset; 
    return iRet;
}


int check_key_call_back(int iCurrent_key_index, int iActionOffset)
{
    
    int iRet = 0;
    pthread_t tid;
    iRet = find_action_id(iCurrent_key_index, iActionOffset);
    if (true == key_need_reg_data_cb.is_reg[iRet]) {
        
        
        printf("we should call  key_index:%d, iActionOffset:%d, function\n", iCurrent_key_index,  iActionOffset);
            
        iRet = pthread_create(&tid,NULL, 
            key_need_reg_data_cb.p_key_reg_data[iRet], 
            key_need_reg_data_cb.p_szParameter[iRet]);

        if (iRet != 0) {
            printf("create check_key_call_back thread failed!\n");
        }
    }
    return 0;
}


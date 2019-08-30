
#include "serial_watch.h"


int Is_Read_Thread_Ready(serial_watch_t *tmp)
{
    printf("while <<< tmp->read_thread_ready_flag:*%d*, tmp:0x%x\n", tmp->read_thread_ready_flag, tmp);
    while(1) {

        //sleep(3);
        //printf("sleep <<< tmp->read_thread_ready_flag:*%d*\n", tmp->read_thread_ready_flag);

        if (tmp->read_thread_ready_flag == WATCH_TRUE)
            break;
    }

    printf("tmp->read_thread_ready_flag:*%d*\n", tmp->read_thread_ready_flag);
    return 0;
}

void *thread_read_serial(void *arg)
{
    int ret = 0;
    struct timeval tv;
    fd_set fds;
    int j = 0;
    int i = 0;
    char buff[SIZE];
    serial_watch_t *p_stSerialWatch = (serial_watch_t *)arg;
    while (p_stSerialWatch->loop_read_flag) {
        tv.tv_sec = 5;
        tv.tv_usec = 5000;
        FD_ZERO(&fds);
        FD_SET(p_stSerialWatch->fd_serial, &fds);
        ret = select(p_stSerialWatch->fd_serial+1,&fds,NULL,NULL,&tv);
        if(ret == 0) //超时
        {
            p_stSerialWatch->read_thread_ready_flag = WATCH_TRUE;
            printf("select time out!, p_stSerialWatch:0x%x, p_stSerialWatch->read_thread_ready_flag:%d\n", p_stSerialWatch, p_stSerialWatch->read_thread_ready_flag);
        }
        else if(ret == -1)  //失败
        {
            printf("fail to select!\n");
        }
        else if (FD_ISSET(p_stSerialWatch->fd_serial, &fds)) //成功
        {
            usleep(1000);
                p_stSerialWatch->read_thread_ready_flag = WATCH_FALSE;
            j = read(p_stSerialWatch->fd_serial, buff, SIZE-1);
            buff[j] = '\0';
            printf("***buff:%s\n", buff);
            for (i = 0; i < j; i++) {
                printf("buff[%d]:%04d\n", i, buff[i]);
            }
            printf("data p_stSerialWatch->fd_serial available!\n");
        }
    }
    return ((void *)0);
}

void *thread_write_serial(void *arg)
{
    int temp;
    pthread_t ntid_read;
    char buff_result[512] = {0};
    char buff_tmp[512] = {0};
    char len[5] = {0};
    char ch = 26;
    serial_watch_t *p_stSerialWatch = (serial_watch_t *)arg;


    p_stSerialWatch->loop_read_flag = WATCH_TRUE;
    if((temp=pthread_create(&ntid_read,NULL,thread_read_serial, p_stSerialWatch)!= 0))
    {
        printf("can't create thread: %s\n",strerror(temp));
        return (void*)(-1);
    }
    usleep(10000);

    write(p_stSerialWatch->fd_serial, "AT+CMGF=0\r", strlen("AT+CMGF=0\r"));
    usleep(10000);


    write(p_stSerialWatch->fd_serial, "AT+CSCS=\"UCS2\"\r", strlen("AT+CSCS=\"UCS2\"\r"));
    usleep(10000);

    watch_mesg_SendMesg(p_stSerialWatch->cszPhoneNum, p_stSerialWatch->cszContent, buff_result, len);
    sprintf(buff_tmp, "AT+CMGS=%s\r", len);

    write(p_stSerialWatch->fd_serial, buff_tmp, strlen(buff_tmp));
    usleep(10000);



    write(p_stSerialWatch->fd_serial, buff_result, strlen(buff_result));
    usleep(10000);

    write(p_stSerialWatch->fd_serial, &ch, 1);
    usleep(10000);
    p_stSerialWatch->loop_read_flag = WATCH_FALSE;
    printf("func:%s, _stSerialWatch->loop_read_flag @@@@ begin false", __func__);
    pthread_join(ntid_read, NULL);
    return ((void *)0);
}


void *thread_write_phone_serial(void *arg)
{
     int temp;
     pthread_t ntid_read;
     char buff_result[512] = {0};
     char buff_tmp[512] = {0};
     char len[5] = {0};
     char ch = 26;
     serial_watch_t *p_stSerialWatch = (serial_watch_t *)arg;

    printf("!!!!!!!!!!!!!!!!!!!!begin true\n");
     p_stSerialWatch->loop_read_flag = WATCH_TRUE;
     if((temp=pthread_create(&ntid_read,NULL,thread_read_serial, p_stSerialWatch)!= 0))
     {
         printf("can't create thread: %s\n",strerror(temp));
         return (void*)(-1);
     }
     usleep(10000);


     write(p_stSerialWatch->fd_serial, "ATH\r", strlen("ATH\r"));
     usleep(10000);

     sprintf(buff_result, "ATD%s;\r", p_stSerialWatch->cszPhoneNum);
     write(p_stSerialWatch->fd_serial, buff_result, strlen(buff_result));
     usleep(10000);

     p_stSerialWatch->loop_read_flag = WATCH_FALSE;
     printf("func:%s, _stSerialWatch->loop_read_flag @@@@ begin false", __func__);
     pthread_join(ntid_read, NULL);
     return ((void *)0);
}


int serial_init(char *dev_name, int *fd_result)
{
    *fd_result  = open(dev_name, O_RDWR| O_NOCTTY);         //| O_NOCTTY | O_NDELAY
    if (-1 == *fd_result)
    {
        perror("Can't Open Serial Port");
        return -1;
    }
    struct termios newttys1,oldttys1;
    oldttys1.c_iflag=0x1    ;
    oldttys1.c_oflag=0x0    ;
    oldttys1.c_cflag=0xcbd  ;
    oldttys1.c_lflag=0x0    ;
    oldttys1.c_cc[0]=0x3             ;
    oldttys1.c_cc[1]=0x1c            ;
    oldttys1.c_cc[2]=0x7f            ;
    oldttys1.c_cc[3]=0x15            ;
    oldttys1.c_cc[4]=0x4             ;
    oldttys1.c_cc[5]=0x5             ;
    oldttys1.c_cc[6]=0x1             ;
    oldttys1.c_cc[7]=0x0             ;
    oldttys1.c_cc[8]=0x11            ;
    oldttys1.c_cc[9]=0x13            ;
    oldttys1.c_cc[10]=0x1a           ;
    oldttys1.c_cc[11]=0x0            ;
    oldttys1.c_cc[12]=0x12           ;
    oldttys1.c_cc[13]=0xf            ;
    oldttys1.c_cc[14]=0x17           ;
    oldttys1.c_cc[15]=0x16           ;
    oldttys1.c_cc[16]=0x0            ;
    oldttys1.c_cc[17]=0x0            ;
    oldttys1.c_cc[18]=0x0            ;
    oldttys1.c_cc[19]=0x0            ;
    oldttys1.c_cc[20]=0x0            ;
    oldttys1.c_cc[21]=0x0            ;
    oldttys1.c_cc[22]=0x0            ;
    oldttys1.c_cc[23]=0x0            ;
    oldttys1.c_cc[24]=0x0            ;
    oldttys1.c_cc[25]=0x0            ;
    oldttys1.c_cc[26]=0x0            ;
    oldttys1.c_cc[27]=0x0            ;
    oldttys1.c_cc[28]=0x0            ;
    oldttys1.c_cc[29]=0x0            ;
    oldttys1.c_cc[30]=0x0            ;
    oldttys1.c_cc[31]=0x0            ;

    if (tcsetattr(*fd_result,TCSANOW,&oldttys1) != 0)
    {
        perror("SetupSerial 3");
        return (-1);
    }
    return 0;
}




int serial_message_watch_api(char *pcPhoneNum, char *pcMessage){

    char buff_send[SIZE] = {0};
    int nread;
    int i = 0;
    int ret = 0;
    int temp;
    pthread_t ntid_write;
    serial_watch_t stSerialWatch;
    memset(&stSerialWatch, 0, sizeof(stSerialWatch));
    memcpy(stSerialWatch.cszPhoneNum,  pcPhoneNum, strlen(pcPhoneNum));
    stSerialWatch.cszPhoneNum[strlen(pcPhoneNum)] = '\0';


    memcpy(stSerialWatch.cszContent,  pcMessage, strlen(pcMessage));
    stSerialWatch.cszContent[strlen(pcMessage)] = '\0';

    ret = serial_init("/dev/ttyUSB2", &stSerialWatch.fd_serial);
    if (ret < 0) {
        return -1;
    }
    if((temp=pthread_create(&ntid_write,NULL,thread_write_serial, (void *)(&stSerialWatch))!= 0))
    {
        printf("can't create thread: %s\n",strerror(temp));
        return 1;
    }


#if 0
    struct termios newttys1,oldttys1;
    if(tcgetattr(p_stSerialWatch->fd_serial,&oldttys1)!=0);
    printf("oldttys1.c_iflag:0x%x\n", oldttys1.c_iflag);
    printf("oldttys1.c_oflag:0x%x\n", oldttys1.c_oflag);
    printf("oldttys1.c_cflag:0x%x\n", oldttys1.c_cflag);
    printf("oldttys1.c_lflag:0x%x\n", oldttys1.c_lflag);


    printf("NCCS:%d\n", NCCS);
    for (i = 0; i < NCCS; i++) {
        printf("c_cc[%d]:0x%x\n", i, oldttys1.c_cc[i]);
    }
    printf("\n");
#endif
    pthread_join(ntid_write, NULL);
    close(stSerialWatch.fd_serial);
    return 0;
}

int serial_phone_watch_api(char *pcMessage)
{
    int nread;
    int i = 0;
    int ret = 0;
    int temp;
    pthread_t ntid_write;
    serial_watch_t stSerialWatch;
    memset(&stSerialWatch, 0, sizeof(stSerialWatch));



    serial_init("/dev/ttyUSB2", &stSerialWatch.fd_serial);
/*判断第一个字符是不是电话号码*/
    if ((pcMessage[0] >= '0')  && (pcMessage[0] <= '9')) {
        //这里说明是电话号码。
        memcpy(stSerialWatch.cszPhoneNum,  pcMessage, strlen(pcMessage));
        stSerialWatch.cszPhoneNum[strlen(pcMessage)] = '\0';
    } else {
        //说明是联系人的名字
        if (0 == strcmp(pcMessage, "上官悦")) {
            memcpy(stSerialWatch.cszPhoneNum,  "13693199230", strlen("13693199230"));
            stSerialWatch.cszPhoneNum[strlen("13693199230")] = '\0';
        } else if (0 == strcmp(pcMessage, "高也")) {
            memcpy(stSerialWatch.cszPhoneNum,  "15652964522", strlen("15652964522"));
            stSerialWatch.cszPhoneNum[strlen("15652964522")] = '\0';
        } else if (0 == strcmp(pcMessage, "谢思桐")) {
            memcpy(stSerialWatch.cszPhoneNum,  "13269217250", strlen("13269217250"));
            stSerialWatch.cszPhoneNum[strlen("13269217250")] = '\0';
        }
    }

    if((temp=pthread_create(&ntid_write,NULL,thread_write_phone_serial, (void *)(&stSerialWatch))!= 0))
    {
        printf("can't create thread: %s\n",strerror(temp));
        return 1;
    }


#if 0
    struct termios newttys1,oldttys1;
    if(tcgetattr(p_stSerialWatch->fd_serial,&oldttys1)!=0);
    printf("oldttys1.c_iflag:0x%x\n", oldttys1.c_iflag);
    printf("oldttys1.c_oflag:0x%x\n", oldttys1.c_oflag);
    printf("oldttys1.c_cflag:0x%x\n", oldttys1.c_cflag);
    printf("oldttys1.c_lflag:0x%x\n", oldttys1.c_lflag);


    printf("NCCS:%d\n", NCCS);
    for (i = 0; i < NCCS; i++) {
        printf("c_cc[%d]:0x%x\n", i, oldttys1.c_cc[i]);
    }
    printf("\n");
#endif
    pthread_join(ntid_write, NULL);
    close(stSerialWatch.fd_serial);
    return 0;
}

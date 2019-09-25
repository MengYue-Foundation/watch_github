#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <at_server.h>

static int fd_serial = 0;
static int g_iState = CMD_SEND_END;
static int g_iAt_cmd_result = OK;
static int g_iPhone_state = IDLE_PHONE;
static int g_iCurrent_Type = NO_CMD;
static st_modem_reg_info_t g_st_modem_info;

/*搜索第一个不是\r或者\n的字符*/
int search_first_valid_char(char *tmp_buf)
{
	int i_index = 0;
	int i_str_num = strlen(tmp_buf);
	while (i_index < i_str_num) {
		if (tmp_buf[i_index] != '\r' && tmp_buf[i_index] != '\n') {
			return i_index;
		}
		i_index++;
	}
	return 0;
}


/*找到ok， error的最后位置*/
int search_last_valid_char(char *tmp_buf)
{
	int i_index = 0;
	int i_str_num = strlen(tmp_buf);
	i_index = i_str_num;
	/*ok, error 返回的位置不一样*/
	while (i_index-- > 0) {
		if (tmp_buf[i_index] != '\r' && tmp_buf[i_index] != '\n') {
			/*把那些没有用的字符串去掉去*/
			return i_index;
		}
	}
	return 0;
}

int search_result_index(char *tmp_buf, int i_last_char_index)
{
	if ('K' == tmp_buf[i_last_char_index]) {
		return (i_last_char_index - 1);
	} else if ('R' == tmp_buf[i_last_char_index]) {
		return (i_last_char_index - 4);
	}
	return 0;
}
int process_reply_info(char *p_reply_info)
{
	
}
int process_result(char *p_result_buf, char *p_reply_info) 
{
	printf("func:%s, line:%d, ****cmd flow begin***\n", __func__, __LINE__);
	int i = 0;
	int i_first_valid_char_index = 0;
	int i_last_valid_char_index = 0;
	int iResult_index = 0;
	int i_str_num = strlen(p_result_buf);
	char tmp_store[SIZE];
	memset(tmp_store, 0, SIZE);
	i_first_valid_char_index = search_first_valid_char(p_result_buf);
	i_last_valid_char_index = search_last_valid_char(p_result_buf);

	/*把第一个之前的字符串全部删除掉去*/
	sprintf(tmp_store, "%s", p_result_buf+i_first_valid_char_index);
	sprintf(p_result_buf, "%s", tmp_store);

	/*把末尾的无意义的字符删除掉去*/
	p_result_buf[i_last_valid_char_index - i_first_valid_char_index + 1] = '\0';

	/*把命令执行正确还是错误存储下来*/
	iResult_index = search_result_index(p_result_buf, i_last_valid_char_index - i_first_valid_char_index);


#ifdef SGY_DEBUG

	i = 0;
	while (p_result_buf[i] != '\0') {
		printf("p_result_buf[%d]:%04d\n", i, p_result_buf[i]);
		i++;
	}
#endif

	/*把结果删除掉去，抽出来的部门就是命令的查询结果*/
	snprintf(tmp_store, iResult_index+1, "%s", p_result_buf);
	tmp_store[iResult_index] = '\0';
	sprintf(p_reply_info, "%s", tmp_store);
#ifdef SGY_DEBUG
	i = 0;
	while (p_reply_info[i] != '\0') {
		printf("p_reply_info[%d]:%04d\n", i, p_reply_info[i]);
		i++;
	}
#endif

	/*把命令执行正确还是错误存储下来*/
	sprintf(tmp_store, "%s", p_result_buf+iResult_index);
	sprintf(p_result_buf, "%s", tmp_store);
#ifdef SGY_DEBUG

	//i = 
	printf("result:%s\n", p_result_buf);
	printf("str_num:%d, i_first:%d, i_last:%d, iResult_index:%d\n", i_str_num, i_first_valid_char_index, i_last_valid_char_index, iResult_index);

#endif
	printf("func:%s, line:%d, ****cmd flow end***\n", __func__, __LINE__);

#if 0
	sprintf(tmp_store, "%s", p_result_buf+i_last_valid_char_index);
	sprintf(p_result_buf, "%s", tmp_store);
	printf("p_result_buf:%s, strlen(p_result_buf):%d\n", p_result_buf, strlen(p_result_buf));
	process_reply_info();
#endif
	return 0;
}
int get_phone_number(char *tmp_buf)
{
	char *p_process_tmp_buf = NULL;
	char tmp_store[SIZE];
	memset(tmp_store, 0, SIZE);
	p_process_tmp_buf = strchr(tmp_buf, '"');
	snprintf(tmp_store, 12, p_process_tmp_buf+1);
	tmp_store[11] = '\0';

	sprintf(tmp_buf, "%s", tmp_store);
	printf("line:%d, phone_number:%s\n", __LINE__, tmp_buf);

	return 0;
}

int execute_callback(int iRequest, char *tmp_buf)
{
	pthread_t tid;
	int iRet = 0;
	switch (iRequest) {
		case REQUEST_RING:
		case CALLING_HANG_UP:
		case NO_CALLING_HANG_UP:
		case PHONE_CALL_CONNECT:	

			if (true == g_st_modem_info.is_reg[iRequest]) {				
				char *para = (char *)(g_st_modem_info.p_szParameter[iRequest]);
				if (tmp_buf != NULL) {
					sprintf(para, "%s", tmp_buf);
				}
				
				iRet = pthread_create(&tid, NULL, 
					g_st_modem_info.p_sz_request_callback[iRequest], 
					g_st_modem_info.p_szParameter[iRequest]);
				
				if (iRet != 0) {
					printf("create check_key_call_back thread failed!\n");
				}
			}
			break;
		default:
			break;
	}
	return 0;
}
int process_no_reply(char *tmp_buf)
{
	int i = 0;
	int iStr_num = strlen(tmp_buf);
	char tmp_store[SIZE];
	int i_first_valid_char_index = 0;
	int i_last_valid_char_index = 0;
	memset(tmp_store, 0, SIZE);
	printf("func:%s, line:%d, ****data flow begin***\n", __func__, __LINE__);

	
	printf("tmp_buf_before_handle:%s\n", tmp_buf);
	i_first_valid_char_index = search_first_valid_char(tmp_buf);
	i_last_valid_char_index = search_last_valid_char(tmp_buf);
	tmp_buf[i_last_valid_char_index+1] = '\0';
	sprintf(tmp_store, "%s", tmp_buf+i_first_valid_char_index);
	sprintf(tmp_buf, "%s", tmp_store);


/*有的时候ring， 和来电一起读取回来， 有的时候只会读取回来一次*/
	switch (g_iPhone_state) {
		case IDLE_PHONE:
			if (strstr(tmp_buf, "RING") != NULL) {
				if (strstr(tmp_buf, "+CLIP:") != NULL) {
					/*说明这个地方有来电显示的电话号码*/
					get_phone_number(tmp_buf);
					g_iPhone_state = RING_READY;
					execute_callback(REQUEST_RING ,tmp_buf);
				} else {
					g_iPhone_state = RING;
				}
			}
			break;
		case RING:
			if (strstr(tmp_buf, "+CLIP:") != NULL) {
				/*说明这个地方有来电显示的电话号码*/
				get_phone_number(tmp_buf);
				g_iPhone_state = RING_READY;
				execute_callback(REQUEST_RING ,tmp_buf);
			}
			break;
		case RING_READY:
			/*ring， 和来电显示的那种情况不需要处理*/
			if (tmp_buf[0] != '+' && tmp_buf[0] != 'R') {
			} 
			break;
		case PHONE_CALLING:
			break;
		default:
			break;
	}
	
#ifdef SGY_DEBUG
		
		//i = 
		printf("tmp_buf_after_handle:%s\n", tmp_buf);
		printf("str_num:%d, i_first:%d, i_last:%d, g_iPhone_state:%d\n", iStr_num, i_first_valid_char_index, i_last_valid_char_index, g_iPhone_state);
		
#endif
	
	printf("func:%s, line:%d, ****data flow end***\n", __func__, __LINE__);
	return 0;
}

int check_result(char *buff)
{
	if (0 == strcmp(buff, "OK")) {
		g_iAt_cmd_result = OK;
	} else if (0 == strcmp(buff, "ERROR")){
		g_iAt_cmd_result = ERROR;
	}
	return 0;
}
int at_send_command(char *p_cmd, int *p_iResult)
{
	int i = 0;
	char sz_tmp_cmd_buf[SIZE];
	memset(sz_tmp_cmd_buf, 0, SIZE);
	g_iCurrent_Type = CMD;

	printf("***func:%s, line:%d, *********at_send_command flow begin\n", __func__, __LINE__);

	sprintf(sz_tmp_cmd_buf, "%s", p_cmd);

	while (sz_tmp_cmd_buf[i] != '\0') {
		write(fd_serial, &sz_tmp_cmd_buf[i], 1);
		i++;
		usleep(5*1000);
	}
	g_iState = CMD_READY;

	write(fd_serial, "\r", 1);

	
	while (g_iState != CMD_SEND_END) {
		usleep(1000);
	}

	g_iCurrent_Type = NO_CMD;

	if (p_iResult != NULL) {
		*p_iResult = g_iAt_cmd_result;
	}

	/*说明来了电话要接通*/
	if (0 == strcmp("ATA", p_cmd)) {
		g_iPhone_state = PHONE_CALLING;
	} else if (0 == strcmp("ATH", p_cmd)) {
		g_iPhone_state = IDLE_PHONE;
	}

	printf("g_iAt_cmd_result:%d\n", g_iAt_cmd_result);
	
	printf("***func:%s, line:%d, *********at_send_command flow end\n", __func__, __LINE__);
	return 0;
}

void *thread_at_server_read(void *arg)
{
    int ret = 0;
    struct timeval tv;
    fd_set fds;
    int j = 0;
    int i = 0;
    char buff[SIZE];
	char reply_info[SIZE];

	memset(buff, 0, SIZE);
	memset(reply_info, 0, SIZE);
		
    while (1) {
        tv.tv_sec = 1;
        tv.tv_usec = 1000;
        FD_ZERO(&fds);
        FD_SET(fd_serial, &fds);
        ret = select(fd_serial+1,&fds,NULL,NULL,&tv);
        if(ret == 0) 
        {
        }
        else if(ret == -1) 
        {
            printf("fail to select!\n");
        }
        else if (FD_ISSET(fd_serial, &fds)) 
        {
            j = read(fd_serial, buff, SIZE-1);
            buff[j] = '\0';
            printf("***func:%s, line:%d, buff:%s\n", __func__, __LINE__, buff);
            for (i = 0; i < j; i++) {
                printf("buff[%d]:%04d\n", i, buff[i]);
            }
			/*只有返回结果的开头是 \r \n 的时候才需要开始处理数据, 而且必须过滤掉\r*/
			if (strlen(buff) > 1) {
				/*开始处理结果数据, 这里是发送命令*/
				if (g_iCurrent_Type == CMD) {
					process_result(buff, reply_info);
					check_result(buff);
					//printf();
					g_iState = CMD_SEND_END;
				} else if (g_iCurrent_Type == NO_CMD) {//这里是数据的处理流程， 如ring
					process_no_reply(buff);
					//说明是手机的那
					if (strstr(buff, "NO") != NULL) {
						if (PHONE_CALLING == g_iPhone_state) {							
							g_iPhone_state = IDLE_PHONE;
							execute_callback(CALLING_HANG_UP,NULL);
						} else {
						
							g_iPhone_state = IDLE_PHONE;
							execute_callback(NO_CALLING_HANG_UP,NULL);
						}
					} else if (strstr(buff, "CONNECT") != NULL) { //说明是手表主动拨打的电话
						g_iPhone_state = PHONE_CALLING;
						execute_callback(PHONE_CALL_CONNECT,NULL);
					}
				}
			}
			
        }
    }
    return ((void *)0);
}



int serial_init_at_server(char *dev_name, int *fd_result)
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
	memset(&g_st_modem_info, 0 ,  sizeof(st_modem_reg_info_t));
    return 0;
}

#if 0
void *test(void * p_para)
{
	int iRet = 0;
	printf("func:%s, request:%s\n", __func__, (char *)p_para);
	
	at_send_command("ATA", &iRet);
	if (iRet != OK) {
		printf("send command error\n");
		
	}
}
#endif

#ifdef SGY_DEBUG_MAIN

int main(int argc, char *argv[])
{
	pthread_t tid;
	int iRet = 0;
	char sz_para[24];
	printf("r:%d, n:%d\n", '\r', '\n');
	serial_init("/dev/ttyUSB2", &fd_serial);
    iRet = pthread_create(&tid,NULL, thread_read_serial, NULL);
    if (iRet != 0) {
        printf("create handle_queue_thread failed!\n");
    }
	usleep(500*1000);

#if 1
#endif

	On_Request(REQUEST_RING, test, (void *)sz_para);

	//at_send_command("ATD13693199230;", &iRet);
	//printf("iRet:%d\n", iRet);
	while (1);
	return 0;
}

#endif


int On_Request(int iRequest, void *(*start_routine) (void *), void *p_parameter)
{
	int iRet = 0;
	char tmp_cmd[SIZE];
	memset(tmp_cmd, 0, SIZE);

	char *p_tmp_para = (char *)p_parameter;
	
	if (start_routine != NULL) {
		switch (iRequest) {
			case REQUEST_PHONE:
				sprintf(tmp_cmd, "ATD%s;", p_tmp_para);
				at_send_command(tmp_cmd,&iRet);
				if (iRet != OK) {
					printf("send command error, please retry\n");
					return -1;
				}
				break;
			case REQUEST_RING:
			case NO_CALLING_HANG_UP:
			case CALLING_HANG_UP:
			case PHONE_CALL_CONNECT:	
				if (start_routine != NULL) {
					g_st_modem_info.p_sz_request_callback[iRequest] = start_routine;
					g_st_modem_info.p_szParameter[iRequest] =  p_parameter;
					g_st_modem_info.is_reg[iRequest] = true;
				}
				break;
			default:
				break;
		}
	}
	printf("func:%s, line:%d, On_Request end\n", __func__, __LINE__);
	return 0;
}


int at_server_module_init()
{
	pthread_t tid;
	int iRet = 0;
	serial_init_at_server("/dev/ttyUSB2", &fd_serial);
	
    iRet = pthread_create(&tid,NULL, thread_at_server_read, NULL);
    if (iRet != 0) {
        printf("create handle_queue_thread failed!\n");
    }
	
	usleep(10000);
	
	//开启来电显示
	at_send_command("AT+CLIP=1", NULL);
	printf("func:%s, line:%d, at_server_module_init end\n", __func__, __LINE__);
	return 0;
}


#include "watch_robot.h"


extern pthread_cond_t stCond_key_init;
extern pthread_mutex_t stMutex_key_init;
extern int g_iWhich_screen;
extern int g_iNum;





int robot_send_cmd(char *pcCmd){
	    SA4 stServ;//指定服务器的地址
        //创建socket
        int iSfd=0;
		

		printf("111111\n");
        iSfd=socket(AF_INET,SOCK_STREAM,0);
        if(iSfd==-1){
            perror("socket");
            return -1;
        }
        //初始化服务器的ip地址和端口号

        stServ.sin_family=AF_INET;
        stServ.sin_port=htons(6666);
        //服务器的ip地址就是本机的ip地址。127.0.0.1  "47.94.222.21"
        inet_pton(AF_INET,"47.94.222.21",&stServ.sin_addr);
        //将iSfd连接到服务器
		
		printf("******%s,%s\n",__FILE__,__FUNCTION__);
		printf("111111\n");
		
		
		
        int iC=connect(iSfd,(SA *)&stServ,sizeof(stServ));
        if(iC==-1){
            perror("connect");
            return -1;;
        }
        //已经连接到服务器了


		write(iSfd,pcCmd,1);

        close(iSfd);


    return 0;
}


static void * key30_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_up.bmp" , PRIORITY_4, PICTURE);

}

static void * key30_short_press(void *p_parameter){
	
}

static void * key30_long_press(void *p_parameter){
	robot_send_cmd("t");	
}

static void * key30_up(void *p_parameter){
	robot_send_cmd("y");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
}


static void * key31_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_down.bmp" , PRIORITY_4, PICTURE);	
}

static void * key31_short_press(void *p_parameter){
	
}

static void * key31_long_press(void *p_parameter){
	robot_send_cmd("g");
}

static void * key31_up(void *p_parameter){
	robot_send_cmd("y");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
}



static void * key32_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_left.bmp" , PRIORITY_4, PICTURE);	
}

static void * key32_short_press(void *p_parameter){


	
}

static void * key32_long_press(void *p_parameter){
	robot_send_cmd("f");
}

static void * key32_up(void *p_parameter){
	robot_send_cmd("y");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
}



static void * key33_first_press_down(void *p_parameter){
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_right.bmp" , PRIORITY_4, PICTURE);
}

static void * key33_short_press(void *p_parameter){


}

static void * key33_long_press(void *p_parameter){
	robot_send_cmd("h");
}

static void * key33_up(void *p_parameter){
	robot_send_cmd("y");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
}



static void * key30_r_first_press_down(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_up.bmp" , PRIORITY_4, PICTURE);

}

static void * key30_r_short_press(void *p_parameter){
	
}

static void * key30_r_long_press(void *p_parameter){
	robot_send_cmd("q");
}

static void * key30_r_up(void *p_parameter){
	robot_send_cmd("s");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_right.bmp" , PRIORITY_4, PICTURE);
}


static void * key31_r_first_press_down(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_down.bmp" , PRIORITY_4, PICTURE);	
}

static void * key31_r_short_press(void *p_parameter){
	
}

static void * key31_r_long_press(void *p_parameter){
	robot_send_cmd("w");
}

static void * key31_r_up(void *p_parameter){
	robot_send_cmd("s");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_right.bmp" , PRIORITY_4, PICTURE);
}



static void * key32_r_first_press_down(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_left.bmp" , PRIORITY_4, PICTURE);	
}

static void * key32_r_short_press(void *p_parameter){


	
}

static void * key32_r_long_press(void *p_parameter){
	robot_send_cmd("e");
}

static void * key32_r_up(void *p_parameter){
	robot_send_cmd("s");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_right.bmp" , PRIORITY_4, PICTURE);
}



static void * key33_r_first_press_down(void *p_parameter){
	//enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_right.bmp" , PRIORITY_4, PICTURE);
}

static void * key33_r_short_press(void *p_parameter){
	pthread_mutex_lock(&stMutex_key_init);
	pthread_cond_signal(&stCond_key_init); 
	pthread_mutex_unlock(&stMutex_key_init);

}

static void * key33_r_long_press(void *p_parameter){
	robot_send_cmd("r");
}

static void * key33_r_up(void *p_parameter){
	robot_send_cmd("s");
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_right.bmp" , PRIORITY_4, PICTURE);
}








static int aiworld_second_left(void){
	
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key30_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,key30_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,key30_long_press,NULL);
	key_sys_register(KEY0_UP,key30_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key31_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,key31_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,key31_long_press,NULL);
    key_sys_register(KEY1_UP,key31_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key32_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,key32_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,key32_long_press,NULL);
    key_sys_register(KEY2_UP,key32_up,NULL);

	key_sys_register(KEY3_FIRST_PRESS_DOWN,key33_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key33_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,key33_long_press,NULL);
	key_sys_register(KEY3_UP,key33_up,NULL);
	return 0;
}


static int aiworld_second_right(void){
	
	key_sys_register(KEY0_FIRST_PRESS_DOWN,key30_r_first_press_down,NULL);
	key_sys_register(KEY0_SHORT_PRESS,key30_r_short_press,NULL);
	key_sys_register(KEY0_LONG_PRESS,key30_r_long_press,NULL);
	key_sys_register(KEY0_UP,key30_r_up,NULL);

	key_sys_register(KEY1_FIRST_PRESS_DOWN,key31_r_first_press_down,NULL);
	key_sys_register(KEY1_SHORT_PRESS,key31_r_short_press,NULL);
	key_sys_register(KEY1_LONG_PRESS,key31_r_long_press,NULL);
    key_sys_register(KEY1_UP,key31_r_up,NULL);

	key_sys_register(KEY2_FIRST_PRESS_DOWN,key32_r_first_press_down,NULL);
	key_sys_register(KEY2_SHORT_PRESS,key32_r_short_press,NULL);
	key_sys_register(KEY2_LONG_PRESS,key32_r_long_press,NULL);
    key_sys_register(KEY2_UP,key32_r_up,NULL);

	key_sys_register(KEY3_FIRST_PRESS_DOWN,key33_r_first_press_down,NULL);
	key_sys_register(KEY3_SHORT_PRESS,key33_r_short_press,NULL);
	key_sys_register(KEY3_LONG_PRESS,key33_r_long_press,NULL);
	key_sys_register(KEY3_UP,key33_r_up,NULL);
	return 0;
}

void * robot_first_press_down(void *p_parameter)
{
    printf("aiworld_first_press_down\n");
	
	
	switch(g_iNum){
	case 0:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_home.bmp" , PRIORITY_1, PICTURE);
		break;
	case 1:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_1_home.bmp" , PRIORITY_1, PICTURE);
		break;
	case 2:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_2_home.bmp" , PRIORITY_1, PICTURE);
		break;
	case 3:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_3_home.bmp" , PRIORITY_1, PICTURE);
		break;
	default:
		break;
	}
}



void *robot_rotate_cw(void *pvArgs){
	aiworld_second_left();
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
}

void *robot_rotate_ccw(void *pvArgs){
	aiworld_second_right();
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_right.bmp" , PRIORITY_4, PICTURE);
}



void *robot_short_press(void *p_parameter)
{
    printf("aiworld_short_press\n");

	aiworld_second_left();
	
	g_iWhich_screen = 1400;
	
	alps_sys_register(2,robot_rotate_cw,NULL,CW);
	alps_sys_register(2,robot_rotate_ccw,NULL,CCW);
	
	#if 1
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/dack.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/white.bmp" , PRIORITY_4, PICTURE);
	enqueue_display(0 ,0 ,PART_REFRESH,"./pic/robot/robot_standy_left.bmp" , PRIORITY_4, PICTURE);
	#endif
	

}

void * robot_long_press(void *p_parameter)
{
    printf("aiworld_long_press\n");
	

}


void * robot_up(void *p_parameter)
{
    printf("aiworld_up\n");
	
	switch(g_iNum){
	case 0:
		enqueue_display(0 ,64 ,PART_REFRESH,"./pic/standby_down.bmp" , PRIORITY_1, PICTURE);
		break;
	case 1:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_1.bmp" , PRIORITY_4, PICTURE);
		break;
	case 2:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_2.bmp" , PRIORITY_4, PICTURE);
		break;
	case 3:
		enqueue_display(0 ,64,PART_REFRESH,"./pic/rotate/standby_down_3.bmp" , PRIORITY_4, PICTURE);
		break;
	default:
		break;
	}

}
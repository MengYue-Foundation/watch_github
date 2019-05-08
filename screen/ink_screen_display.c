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
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "EPD_2in13.h"
#include "ink_screen_display.h"

pthread_mutex_t  queue1_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  queue2_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  queue3_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  queue4_data_mutex = PTHREAD_MUTEX_INITIALIZER;


queue_info_t queue1;
queue_info_t queue2;
queue_info_t queue3;
queue_info_t queue4;

static int ink_display_interface(int iWhich_priority, display_info_t *p_display_info, int iValid_data_num);

UBYTE *BlackImage;



static int test(void);

//void *memcpy(void *dest, const void *src, size_t n);
static void * handle_queue_thread (void *p_arg)
{
	display_info_t st_display_info[QUEUE_LENGTH];
	int iWhich_priority;
	int iValid_data_num = 0;
	int iQueue4_have_data;
	int iQueue3_have_data;
	int iQueue2_have_data;	
	int iQueue1_have_data;
	while (1) {
		memset(&st_display_info, 0,  sizeof(st_display_info) * QUEUE_LENGTH);
		int iTotal_num = 0;
		int iCurrent_index = 0;
		iQueue4_have_data = false;
		iQueue3_have_data = false;
		iQueue2_have_data = false;	
		iQueue1_have_data = false;

		pthread_mutex_lock(&queue4_data_mutex);
		iTotal_num = queue4.iTotal_num;
		iCurrent_index = queue4.iRead_index;
		if (iTotal_num > 0) {
			/*需要执行这个函数*/
			iWhich_priority = PRIORITY_4;
			iQueue4_have_data = true;
			//其他队列只需要拷贝一个图片就可以了
			memcpy(&st_display_info[0], &queue4.sz_display_info[iCurrent_index], sizeof(display_info_t));
			iValid_data_num = 1;
			queue4.iRead_index++;
			if (queue4.iRead_index >= QUEUE_LENGTH) {
				queue4.iRead_index = 0;
			}
			
			queue4.iTotal_num--;
		} else {
			iWhich_priority = 0;
			iQueue4_have_data = false;
		}
		pthread_mutex_unlock(&queue4_data_mutex);

		if (false == iQueue4_have_data) {
			pthread_mutex_lock(&queue3_data_mutex);
			iTotal_num = queue3.iTotal_num;
			iCurrent_index = queue3.iRead_index;
			/*队列3需要一次性把数据全部取出来*/
			
			if (iTotal_num > 0) {
				int i = 0;
				iWhich_priority = PRIORITY_3;
				iQueue3_have_data = true;
				while (queue3.iTotal_num > 0) {//当total num = 0的时候，退出循环
					memcpy(&st_display_info[i], &queue3.sz_display_info[iCurrent_index], sizeof(display_info_t));
					//读取一个之后，read位置向后面挪一个，total_num减少一个
					queue3.iRead_index++;					
					queue3.iTotal_num--;
					if (queue3.iRead_index >= QUEUE_LENGTH) {
						queue3.iRead_index = 0;
					}
					iCurrent_index = queue3.iRead_index; //数组的下标需要更新
					i++;//存储的下标也需要更新
				}
				iValid_data_num = i;
			} else {
				iWhich_priority = 0;
				iQueue3_have_data = false;
			}
			pthread_mutex_unlock(&queue3_data_mutex);
		}
		
		/*
		if (true == iQueue3_have_data)	
			printf("queue3RRRRRRRRRRRRRRiRead_index:%d, iTotal_num:%d, iCurrent_index:%d\n", queue3.iRead_index, queue3.iTotal_num, iCurrent_index);
		*/

		//队列2检测有没有数据
		if (iQueue4_have_data == false && iQueue3_have_data == false) {
			pthread_mutex_lock(&queue2_data_mutex);
			iTotal_num = queue2.iTotal_num;
			iCurrent_index = queue2.iRead_index;
			if (iTotal_num > 0) {
				/*需要执行这个函数*/
				iWhich_priority = PRIORITY_2;
				iQueue2_have_data = true;
				iValid_data_num = 1;
				memcpy(&st_display_info[0], &queue2.sz_display_info[iCurrent_index], sizeof(display_info_t));
				queue2.iRead_index++;
				if (queue2.iRead_index >= QUEUE_LENGTH) {
					queue2.iRead_index = 0;
				}
				
				if (queue2.iTotal_num > 0) {
					queue2.iTotal_num--;
				}

			} else {
				iWhich_priority = 0;
				iQueue2_have_data = false;
			}
			/*
			if (true == iQueue2_have_data)
				printf("queue2RRRRRRRRRRRRRRiRead_index:%d, iTotal_num:%d, iCurrent_index:%d\n", queue2.iRead_index, queue2.iTotal_num, iCurrent_index);
			*/
			pthread_mutex_unlock(&queue2_data_mutex);
		}

//队列1检测有没有数据
		if (iQueue4_have_data == false && iQueue3_have_data == false && iQueue2_have_data == false) {
			pthread_mutex_lock(&queue1_data_mutex);
			iTotal_num = queue1.iTotal_num;
			iCurrent_index = queue1.iRead_index;
			if (iTotal_num > 0) {
				/*需要执行这个函数*/
				iWhich_priority = PRIORITY_1;				
				iQueue1_have_data = true;
				iValid_data_num = 1;
				memcpy(&st_display_info[0], &queue1.sz_display_info[iCurrent_index], sizeof(display_info_t));
				
				queue1.iRead_index++;
				if (queue1.iRead_index >= QUEUE_LENGTH) {
					queue1.iRead_index = 0;
				}
				
				if (queue1.iTotal_num > 0) {
					queue1.iTotal_num--;
				}
			} else {
				iWhich_priority = 0;
				iQueue1_have_data = false;
			}
			/*
			if (true == iQueue1_have_data)
				printf("queue1RRRRRRRRRRRRRRiRead_index:%d, iTotal_num:%d, iCurrent_index:%d\n", queue1.iRead_index, queue1.iTotal_num, iCurrent_index);
			*/
			pthread_mutex_unlock(&queue1_data_mutex);
		}

		/*display 函数*/
		if (true == iQueue4_have_data) {
			ink_display_interface(iWhich_priority, st_display_info, iValid_data_num);
		} else if (iQueue3_have_data == true) {
			ink_display_interface(iWhich_priority, st_display_info, iValid_data_num);
		} else if (iQueue2_have_data == true) {
			ink_display_interface(iWhich_priority, st_display_info, iValid_data_num);
		} else if (iQueue1_have_data == true) {
			ink_display_interface(iWhich_priority, st_display_info, iValid_data_num);
		}
		usleep(2000*1000);
	}

	return (void *)0;
}

int display_module_init(void)
{

    DEV_ModuleInit();
	
    // Exception handling:ctrl + c
    //signal(SIGINT, Handler);
    
    if(EPD_Init(lut_full_update) != 0) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);


	pthread_t tid;
	int iRet = 0;
    iRet = pthread_create(&tid,NULL, handle_queue_thread, NULL);
    if (iRet != 0) {
        printf("create handle_queue_thread failed!\n");
    }

	return 0;
}



int enqueue_display(int iX, int iY, int iRefresh_mode, char *path_or_text,  int iPriority, int iDisplay_type)
{
	int iRet = 0;
	
	display_info_t st_display_info;
	memset(&st_display_info, 0, sizeof(st_display_info));
	display_info_t *p_display_info = &st_display_info;

	st_display_info.iDisplay_type = iDisplay_type;
	st_display_info.iRefresh_Mode = iRefresh_mode;
	st_display_info.i_x_coordinate = iX;
	st_display_info.i_y_coordinate = iY;
	sprintf(st_display_info.szPath_text, "%s", path_or_text);
	st_display_info.szPath_text[strlen(path_or_text) + 1] = '\0';
	if (p_display_info == NULL) {
		printf("errno p_display_info, you should pass a valid p_display_info\n");
		return -1;
	} 
	/*要往里面放入数据了
		1. 往哪个队列放呢?
		2. 队列的第几项

		队列需要记录当前放到第几个了
	*/
	int i = 0;
	int iTotal_num = 0;
	switch (iPriority) {
		case PRIORITY_1:
			
            pthread_mutex_lock(&queue1_data_mutex);
			i = queue1.iWrite_index;
			iTotal_num = queue1.iTotal_num;
			if (iTotal_num < QUEUE_LENGTH) {
				memcpy(&queue1.sz_display_info[i], p_display_info, sizeof(display_info_t));
				/*一直写，写到底部, 下一次写的位置*/
				queue1.iWrite_index++;
				queue1.iTotal_num++;
				if (queue1.iWrite_index == QUEUE_LENGTH) {
					queue1.iWrite_index = 0;
				}
			} else if (iTotal_num >= QUEUE_LENGTH) {				
				iRet = -1;
				printf("iPriority:%d failed\n", iPriority);
			}
			
			//printf("queue1WWWWWWWWWWWWWWWiWrite_index:%d, iTotal_num:%d, i:%d\n", queue1.iWrite_index, queue1.iTotal_num, i);
            pthread_mutex_unlock(&queue1_data_mutex);
			break;
		case PRIORITY_2:
            pthread_mutex_lock(&queue2_data_mutex);
			i = queue2.iWrite_index;
			iTotal_num = queue2.iTotal_num;
			if (iTotal_num < QUEUE_LENGTH) {
				memcpy(&queue2.sz_display_info[i], p_display_info, sizeof(display_info_t));
				/*一直写，写到底部, 下一次写的位置*/
				queue2.iWrite_index++;
				queue2.iTotal_num++;
				if (queue2.iWrite_index == QUEUE_LENGTH) {
					queue2.iWrite_index = 0;
				}
			} else if (iTotal_num >= QUEUE_LENGTH) {				
				iRet = -1;
				printf("iPriority:%d failed\n", iPriority);
			}
			
			//printf("queue2WWWWWWWWWWWWWWWiWrite_index:%d, iTotal_num:%d, i:%d\n", queue2.iWrite_index, queue2.iTotal_num, i);
            pthread_mutex_unlock(&queue2_data_mutex);

			break;
		case PRIORITY_3:
            pthread_mutex_lock(&queue3_data_mutex);
			i = queue3.iWrite_index;
			iTotal_num = queue3.iTotal_num;
			if (iTotal_num < QUEUE_LENGTH) {
				memcpy(&queue3.sz_display_info[i], p_display_info, sizeof(display_info_t));
				/*一直写，写到底部, 下一次写的位置*/
				queue3.iWrite_index++;
				queue3.iTotal_num++;

				//这个表示是写到底部了，应该往回写了
				if (queue3.iWrite_index == QUEUE_LENGTH) {
					queue3.iWrite_index = 0;
				}
			} else if (iTotal_num >= QUEUE_LENGTH) {				
				iRet = -1;
				printf("iPriority:%d failed\n", iPriority);
			}
			//printf("queue3WWWWWWWWWWWWWWWiWrite_index:%d, iTotal_num:%d, i:%d\n", queue3.iWrite_index, queue3.iTotal_num, i);
            pthread_mutex_unlock(&queue3_data_mutex);
			break;
			
		case PRIORITY_4:
			pthread_mutex_lock(&queue4_data_mutex);
			i = queue4.iWrite_index;
			iTotal_num = queue4.iTotal_num;
			if (iTotal_num < QUEUE_LENGTH) {
				memcpy(&queue4.sz_display_info[i], p_display_info, sizeof(display_info_t));
				/*一直写，写到底部, 下一次写的位置*/
				queue4.iWrite_index++;
				queue4.iTotal_num++;
		
				//这个表示是写到底部了，应该往回写了
				if (queue4.iWrite_index == QUEUE_LENGTH) {
					queue4.iWrite_index = 0;
				}
			} else if (iTotal_num >= QUEUE_LENGTH) {				
				iRet = -1;
				printf("iPriority:%d failed\n", iPriority);
			}
			//printf("queue4WWWWWWWWWWWWWWWiWrite_index:%d, iTotal_num:%d, i:%d\n", queue4.iWrite_index, queue4.iTotal_num, i);
			pthread_mutex_unlock(&queue4_data_mutex);
			break;
			
		default:
			break;
	}
	
	return iRet;
}


static int ink_display_interface(int iWhich_priority, display_info_t *p_display_info, int iValid_data_num)
{


	static int iPre_Refresh_mode = FULL_REFRESH;
	int iCur_Refresh_mode = p_display_info[0].iRefresh_Mode;

printf("iRefresh_Mode:%d, p_display_info[0].szPath_text:%s\n", iCur_Refresh_mode, p_display_info[0].szPath_text);
	//一次都没有执行刷新的话，直接刷新不需要判断是不是和前面一次的刷新有什么不一样
	if (true) {//说明不是第一次做这个刷新的事情了
		if (iPre_Refresh_mode != iCur_Refresh_mode) { //说明两次的刷新模式不一样了才需要进行更换刷新模式
			if (FULL_REFRESH == iCur_Refresh_mode) {
				/*全局刷新的接口*/
				iPre_Refresh_mode = FULL_REFRESH;
				printf("33333333333333333\n");
				//Partial refresh, example shows time,刷新的接口理论上只需要调用一次就可以了
				 if(EPD_Init(lut_full_update) != 0) {
					 printf("e-Paper init failed\r\n");
				 }
				 
				 EPD_Clear();
			} else if (PART_REFRESH == iCur_Refresh_mode) {
				iPre_Refresh_mode = PART_REFRESH;
				//局部刷新的接口
				printf("4444444444444444\n");
				//Partial refresh, example shows time,刷新的接口理论上只需要调用一次就可以了
				 if(EPD_Init(lut_partial_update) != 0) {
					 printf("e-Paper init failed\r\n");
				 }
			}
		}//
	}

	
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	switch (iWhich_priority) {
		case PRIORITY_1:
		case PRIORITY_2:
		case PRIORITY_4: {
			int iContent = p_display_info[0].iDisplay_type;
			int iX = p_display_info[0].i_x_coordinate;
			int iY = p_display_info[0].i_y_coordinate;
			char *p_text_path = p_display_info[0].szPath_text;
			printf("iContent:%d, p_text_path:%s\n", iContent, p_text_path);
			if (TEXT_ENGLISH == iContent) {
				Paint_DrawString_EN(iX , iY, p_text_path, &Font16, WHITE, BLACK);
			} else if (PICTURE == iContent) {
				GUI_ReadBmp(p_text_path, iX, iY);
			} else if (TEXT_CHINESE == iContent) {
				Paint_DrawString_CN(iX , iY, p_text_path, &Font12CN, WHITE, BLACK);
			}
			
		}
			break;
		case PRIORITY_3: {
			int i = 0;
			while (i < iValid_data_num) {
				int iContent = p_display_info[i].iDisplay_type;
				int iX = p_display_info[i].i_x_coordinate;
				int iY = p_display_info[i].i_y_coordinate;
				char *p_text_path = p_display_info[i].szPath_text;
				if (TEXT_ENGLISH == iContent) {
					Paint_DrawString_EN(iX , iY, p_text_path, &Font16, WHITE, BLACK);
				} else if (PICTURE == iContent) {
					GUI_ReadBmp(p_text_path, iX, iY);
				} else if (TEXT_CHINESE == iContent) {
					Paint_DrawString_CN(iX , iY, p_text_path, &Font12CN, WHITE, BLACK);
				}
				i++;
			}
		}
			break;
		
		default:
			break;
	}
	
    EPD_Display(BlackImage);
	return 0;
}

static int test(void)
{
		printf("2.13inch e-Paper demo\r\n");
		DEV_ModuleInit();
	
		// Exception handling:ctrl + c
		//signal(SIGINT, Handler);
	
		if(EPD_Init(lut_full_update) != 0) {
			printf("e-Paper init failed\r\n");
		}
		EPD_Clear();
		DEV_Delay_ms(500);
	
		//Create a new image cache
		UBYTE *BlackImage;
		UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
		if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for memory...\r\n");
			return -1;
		}
		printf("Paint_NewImage\r\n");
		Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 90, WHITE);
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
	
#if 1   //show image for array    
		printf("show image for array\r\n");
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
		Paint_DrawBitMap(gImage_2in13);
	
		EPD_Display(BlackImage);
		DEV_Delay_ms(2000);
#endif
	
#if 1   // show bmp    
		printf("show bmp\r\n");
		Paint_SelectImage(BlackImage);
		
		Paint_Clear(WHITE);
		GUI_ReadBmp("./pic/home.bmp", 0, 0);
		EPD_Display(BlackImage);
		DEV_Delay_ms(500);
	
		Paint_Clear(WHITE);    
		GUI_ReadBmp("./pic/2in13.bmp", 0, 0);
		EPD_Display(BlackImage);
		DEV_Delay_ms(2000);
#endif
	
#if 1   // Drawing on the image
		//1.Select Image
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
	
		// 2.Drawing on the image
		Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
		Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
		Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
		Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
	
		Paint_DrawLine(20, 10, 70, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
		Paint_DrawLine(70, 10, 20, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
		Paint_DrawRectangle(20, 10, 70, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
		Paint_DrawRectangle(85, 10, 135, 60, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
	
		Paint_DrawLine(45, 15, 45, 55, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
		Paint_DrawLine(25, 35, 70, 35, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
		Paint_DrawCircle(45, 35, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
		Paint_DrawCircle(110, 35, 20, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
	
		Paint_DrawString_EN(140, 15, "waveshare", &Font16, BLACK, WHITE);
		Paint_DrawNum(140, 40, 123456789, &Font16, BLACK, WHITE);
	
		Paint_DrawString_CN(140, 60, "你好abc", &Font12CN, BLACK, WHITE);
		Paint_DrawString_CN(5, 65, "微雪电子", &Font24CN, WHITE, BLACK);
	
		EPD_Display(BlackImage);
#endif
	
#if 1   //Partial refresh, example shows time    
		if(EPD_Init(lut_partial_update) != 0) {
			printf("e-Paper init failed\r\n");
		}
		Paint_SelectImage(BlackImage);
		PAINT_TIME sPaint_time;
		sPaint_time.Hour = 12;
		sPaint_time.Min = 34;
		sPaint_time.Sec = 56;
		for (;;) {
			sPaint_time.Sec = sPaint_time.Sec + 1;
			if (sPaint_time.Sec == 60) {
				sPaint_time.Min = sPaint_time.Min + 1;
				sPaint_time.Sec = 0;
				if (sPaint_time.Min == 60) {
					sPaint_time.Hour =	sPaint_time.Hour + 1;
					sPaint_time.Min = 0;
					if (sPaint_time.Hour == 24) {
						sPaint_time.Hour = 0;
						sPaint_time.Min = 0;
						sPaint_time.Sec = 0;
					}
				}
			}
			Paint_ClearWindows(140, 90, 140 + Font20.Width * 7, 90 + Font20.Height, WHITE);
			Paint_DrawTime(140, 90, &sPaint_time, &Font20, WHITE, BLACK);
	
			EPD_Display(BlackImage);
			DEV_Delay_ms(500);//Analog clock 1s
		}
	
#endif
	
		printf("Goto Sleep mode...\r\n");
		EPD_Sleep();
	
		if(NULL != BlackImage) {
			free(BlackImage);
			BlackImage = NULL;
		}
	
		return 0;

}



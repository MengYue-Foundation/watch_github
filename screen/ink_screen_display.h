#ifndef __INK_SCREEN_DISPLAY__H

#define __INK_SCREEN_DISPLAY__H

#define QUEUE_LENGTH	(5)
#define QUEUE_PRIORITY_NUM	(3)
#define PARA_LENTH	(64)

#define 	PICTURE		(1)
#define 	TEXT_ENGLISH		(2)
#define 	TEXT_CHINESE		(3)

#define FULL_REFRESH	(1)
#define PART_REFRESH	(2)


/*数字越大优先级越高*/
enum QUEUE_PRIORITY {
	PRIORITY_1 = 1,
	PRIORITY_2,
	PRIORITY_3,//队列3主要用来存放多个图片一次性刷新	
	PRIORITY_4,	
};


typedef struct display_info {
	int iRefresh_Mode;
	int i_x_coordinate;
	int i_y_coordinate;
	int iDisplay_type;
	char szPath_text[PARA_LENTH];//图片路径或者需要显示的字符串
}display_info_t;

typedef struct queue_info {
	int iRead_index;
	int iWrite_index;
	int iTotal_num;
	display_info_t sz_display_info[QUEUE_LENGTH];
	char szPath_text[PARA_LENTH];	//图片路径或者需要显示的字符串
}queue_info_t;


extern int enqueue_display(display_info_t *p_display_info, int iPriority);
extern int display_module_init(void);
extern UBYTE *BlackImage;

#endif

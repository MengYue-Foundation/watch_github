#include "screen.h"
#if 0

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    EPD_Sleep();
    DEV_ModuleExit();

    exit(0);
}

int watch_screen(void)
{
#if 1
    int iFd = 0;
    int iRet = 0;
    int iData = 0;
    char buf[2] = {0};
	pthread_t stTid;


    printf("2.13inch e-Paper demo\r\n");
    DEV_ModuleInit();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(EPD_Init(lut_full_update) != 0) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(500);

	
	iFd = open(TTP229_I2C_BUS_NAME, O_RDWR);

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
#if 1
/*********/
	

	Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

	Paint_DrawCircle(110, 35, 20, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
	Paint_DrawString_EN(0, 0, "waveshare", &Font16, WHITE, BLACK);
	Paint_DrawNum(0, 0, 123456789, &Font16, WHITE, BLACK);
	Paint_DrawString_CN(140, 60, "ÄãºÃabc", &Font12CN, BLACK, WHITE);

/***************/
#endif
	if(EPD_Init(lut_partial_update) != 0) {
        printf("e-Paper init failed\r\n");
    }
    Paint_SelectImage(BlackImage);

	Paint_Clear(WHITE);    
    GUI_ReadBmp("./pic/standby.bmp", 0, 0);
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);

	GUI_ReadBmp("./pic/standby_down.bmp", 0, 64);
	EPD_Display(BlackImage);

	if (iRet < 0) {
		printf("can't open '%s'", TTP229_I2C_BUS_NAME);
        return -1;
	}

    iRet = ioctl(iFd, I2C_SLAVE, TTP229_ADDR);
	if (iRet < 0) {
		printf("can't open '%s'", TTP229_I2C_BUS_NAME);
        return -1;
	}

	pthread_create(&stTid,NULL,watch_time,BlackImage);


    while (1) {
        i2c_smbus_write_byte(iFd, 0x57);
        buf[0] = 0;
        buf[1] = 0;
        read(iFd, buf, 2);
        iData = (buf[1] << 8) | (buf[0]);
        //printf("buf[0]:0x%x, buf[1]:0x%x, iData:0x%x\n", buf[0], buf[1], iData);
        key_separation(iData,BlackImage,Imagesize); 
        //sleep(1);
		DEV_Delay_ms(50);
    }
#endif

    return 0;
}
#endif

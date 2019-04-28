#include "watch_serial.h"

int OpenDev(char *Dev)
{
    int fd = open( Dev, O_RDWR| O_NOCTTY);         //| O_NOCTTY | O_NDELAY
    if (-1 == fd)
    {
        perror("Can't Open Serial Port");
        return -1;
    }
    else
        return fd;
}


int watch_serial_init(char *pcSerial){
    int nread;
    char buff[50]={0};
    int i = 0;
    struct timeval tv;
    fd_set fds;
    int j = 0;
    int ret = 0;
    int temp;
    int iFd=0;
    iFd = OpenDev(pcSerial);
    printf("fd:%d\n", iFd);

    struct termios newttys1,oldttys1;

    if(tcgetattr(iFd,&oldttys1)!=0);
    printf("oldttys1.c_iflag:0x%x\n", oldttys1.c_iflag);
    printf("oldttys1.c_oflag:0x%x\n", oldttys1.c_oflag);
    printf("oldttys1.c_cflag:0x%x\n", oldttys1.c_cflag);
    printf("oldttys1.c_lflag:0x%x\n", oldttys1.c_lflag);


    printf("NCCS:%d\n", NCCS);
    for (i = 0; i < NCCS; i++) {
        printf("c_cc[%d]:0x%x\n", i, oldttys1.c_cc[i]);
    }
    printf("\n");
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
    if (tcsetattr(iFd,TCSANOW,&oldttys1) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }

    return iFd;
}


int watch_car_serial_init(char *pcSerial){
    int fdd, len;
    struct termios opt;
    if ((fdd = open(pcSerial, O_RDWR | O_NOCTTY | O_NDELAY))== -1) {
        printf("ERROR: Cannot open the desired port \n");
        return -1;
    }

    //tcgetattr函数用于获取与终端相关的参数
    tcgetattr(fdd,&opt);    // get this port termios struct
    //cfgetispeed函数用于获得结构体termios_p中的输入波特率信息，而cfgetospeed函数用于获得结构体termios_p中的输出波特率信息
    cfsetispeed(&opt, B115200); //input baudrate
    cfsetospeed(&opt, B115200); //output baudrate
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;         // 8 bits length
    opt.c_cflag &= ~(CSTOPB | PARENB | CRTSCTS);    // 1 bit stop, no parity no flow ctrl
    opt.c_cflag |= CLOCAL | CREAD;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
    /* 输入字符保持原样 */
    opt.c_iflag &= ~(IGNBRK | BRKINT |INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IUCLC);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY); // no software flow control
    opt.c_oflag &= ~OPOST;  // raw output
    opt.c_cc[VMIN]=0;
    opt.c_cc[VTIME]=10;
    //tcsetattr函数用于设置终端参数
    tcsetattr(fdd,TCSANOW,&opt);
    return fdd;
}

int watch_serial_write(int iFd,char *pcATcmd,int iSize){
    return write(iFd,pcATcmd,iSize);
}

int watch_serial_read(int iFd,char *pcATans,int iSize){
    return read(iFd,pcATans,iSize);
}










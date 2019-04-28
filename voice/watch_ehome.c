#include "watch_ehome.h"

int watch_ehome_fan_open(void){
    int iIndex=0;
    int iFd=open("/dev/myled",O_RDWR);
    iIndex=3;
    ioctl(iFd,1,&iIndex);
    close(iFd);

}
int watch_ehome_fan_close(void){
    int iIndex=0;
    int iFd=open("/dev/myled",O_RDWR);
    iIndex=3;
    ioctl(iFd,0,&iIndex);
    close(iFd);
}
int watch_ehome_fan_larger(void){
    int iIndex=0;
    int iFd=open("/dev/myled",O_RDWR);
    iIndex=3;
    ioctl(iFd,1,&iIndex);
    iIndex=4;
    ioctl(iFd,1,&iIndex);
    close(iFd);
}
int watch_ehome_fan_smaller(void){
    int iIndex=0;
    int iFd=open("/dev/myled",O_RDWR);
    iIndex=3;
    ioctl(iFd,1,&iIndex);
    iIndex=4;
    ioctl(iFd,0,&iIndex);
    close(iFd);
}
int watch_ehome_fan_set(char *pcLevel){
    int iIndex=0;
    int iFd=open("/dev/myled",O_RDWR);
    if(!(strcmp("1",pcLevel)&&strcmp("一",pcLevel)&&strcmp("一档",pcLevel))){
        iIndex=3;
        ioctl(iFd,1,&iIndex);
        iIndex=4;
        ioctl(iFd,0,&iIndex);
    }else if(!(strcmp("2",pcLevel)&&strcmp("二",pcLevel)&&strcmp("二档",pcLevel))){
        iIndex=3;
        ioctl(iFd,1,&iIndex);
        iIndex=4;
        ioctl(iFd,1,&iIndex);
    }
    close(iFd);
}

#include "watch_mesg.h"

int Watch_mesg_charset_convert(const char *from_charset, const char *to_charset,char *in_buf, size_t in_left, char *out_buf, size_t out_left) {
    iconv_t icd;
    char *pin = in_buf;
    char *pout = out_buf;
    size_t out_len = out_left;
        printf("%s\n%s\n",from_charset,to_charset);
    if ((iconv_t)-1 == (icd = iconv_open(to_charset,from_charset))) {
        perror("iconv_open");
        return -1;
    }
    if ((size_t)-1 == iconv(icd, &pin, &in_left, &pout, &out_left)) {
        iconv_close(icd);
        return -1;
    }
    out_buf[out_len - out_left] = 0;
    iconv_close(icd);
    return (int)out_len - out_left;
}


int ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
        short i;
        unsigned char highByte, lowByte;

        for (i = 0; i < sourceLen; i++)
        {
                highByte = source[i] >> 4;
                lowByte = source[i] & 0x0f ;

                highByte += 0x30;

                if (highByte > 0x39)
                        dest[i * 2] = highByte + 0x07;
                else
                        dest[i * 2] = highByte;

                lowByte += 0x30;
                if (lowByte > 0x39)
                        dest[i * 2 + 1] = lowByte + 0x07;
                else
                        dest[i * 2 + 1] = lowByte;
        }
        return 0;
}


int Change_Num(char *pcInput,char *pcOutput){

    int iWhile=0;
    int iI=0;
    char cTmp='F';
    char cOver='\0';

    iWhile=strlen(pcInput)/2+strlen(pcInput)%2;
    printf("%d\n",iWhile);
    for(iI=0;iI<iWhile;iI++){
        if(iI==iWhile-1){
            if(!(strlen(pcInput)%2)){
                strncpy(pcOutput+iI*2,pcInput+iI*2+1,1);
                strcat(pcOutput+iI*2+1,&cOver);
            }else{
                strcat(pcOutput+iI*2,&cTmp);
                strcat(pcOutput+iI*2+1,&cOver);
            }
        }else{
            strncpy(pcOutput+iI*2,pcInput+iI*2+1,1);
        }
        strncpy(pcOutput+iI*2+1,pcInput+iI*2,1);
    }
    return 0;

}

int watch_mesg_SendMesg(char *pcTelNumber,char *pcText,char *pcOutText,char *pcTextLen){
    char cszTelNumber[64]={0};
    char cszText[512]={0};
    char cszText16[1024]={0};
    char cszLen[5]={0};

    bzero(cszTelNumber,64);
    Change_Num(pcTelNumber,cszTelNumber);
    printf("cszTelNumber:%s\n",cszTelNumber);

    Watch_mesg_charset_convert("UTF-8","UNICODEBIG",pcText,strlen(pcText),cszText,512);
    printf("cszText:%s\n",cszText);

    ByteToHexStr(cszText,cszText16,strlen(cszText));

    sprintf(cszLen,"%02X",strlen(cszText16)/2);
    printf("cszLen:%s\n",cszLen);

    sprintf(pcOutText,"0001000B81%s0008%s%s",cszTelNumber,cszLen,cszText16);
    printf("pcOutText:%s\n",pcOutText);

    sprintf(pcTextLen,"%d",(strlen(pcOutText)-2)/2);
    printf("pcTextLen:%s\n",pcTextLen);

    return 0;

}



#include "watch_note_client.h"

int watch_note_client(char *pcNote){
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
        stServ.sin_port=htons(6665);
        //服务器的ip地址就是本机的ip地址。127.0.0.1
        inet_pton(AF_INET,"47.94.222.21",&stServ.sin_addr);
        //将iSfd连接到服务器
		
		printf("******%s,%s,iPort:%d,Ip:%s\n",__FILE__,__FUNCTION__,SERVER_NOTE_PORT,SERVER_IP);
		printf("111111\n");
        int iC=connect(iSfd,(SA *)&stServ,sizeof(stServ));
        if(iC==-1){
            perror("connect");
            return -1;;
        }
        //已经连接到服务器了

		printf("******%s,%s,pcNote:%s\n",__FILE__,__FUNCTION__,pcNote);
		write(iSfd,pcNote,strlen(pcNote));

        close(iSfd);


    return 0;
}
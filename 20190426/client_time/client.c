#include "function.h"
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socket_fd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"connect");
	printf("connect success\n");
	int dataLen;
	char buf[1000]={0};
	
    //接收文件名
    recvCycle(socket_fd,&dataLen,4);
	recvCycle(socket_fd,buf,dataLen);
	int fd;
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	ERROR_CHECK(fd,-1,"open");

    //接收文件大小
    time_t start,curr;
    off_t fileSize=0,downLoadSize=0;
    recvCycle(socket_fd,&dataLen,4);
    recvCycle(socket_fd,&fileSize,dataLen);

    //接收文件内容
    start=curr=time(NULL);
	while(1)
	{
		ret=recvCycle(socket_fd,&dataLen,4);
        if(-1==ret)
        {
            printf("server is updating\n");
            break;
        }
		if(dataLen>0)
		{
			ret=recvCycle(socket_fd,buf,dataLen);
            if(-1==ret)
            {
                break;
            }
			write(fd,buf,dataLen);
            downLoadSize += dataLen;
            time(&curr);
            if(curr-start>=1)   //每隔1秒打印一次进度
            {
                printf("\r%5.2f%%",(float)downLoadSize/fileSize*100);
                fflush(stdout);
                start=curr;
            }
		}else{
            printf("\r100.00%%\n");
			break;
		}
	}
	close(fd);
	close(socket_fd);
	return 0;
}

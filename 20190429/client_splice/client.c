#include "function.h"
#define SPLICE_BLOCK 4096
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
	fd=open(buf,O_CREAT|O_RDWR,0666);
	ERROR_CHECK(fd,-1,"open");

    //接收文件大小
    off_t fileSize=0,oldSize=0,downLoadSize=0,sliceSize;
    recvCycle(socket_fd,&dataLen,4);
    recvCycle(socket_fd,&fileSize,dataLen);
    
    struct timeval start,end;
    int fds[2];
    pipe(fds);
    sliceSize=fileSize/10000;
    //splice接收文件内容
    gettimeofday(&start,NULL);

    while(downLoadSize+SPLICE_BLOCK<fileSize)
    {
        ret=splice(socket_fd,NULL,fds[1],NULL,SPLICE_BLOCK,SPLICE_F_MORE|SPLICE_F_MOVE);
        ERROR_CHECK(ret,-1,"splice1");
        ret=splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE|SPLICE_F_MOVE);
        ERROR_CHECK(ret,-1,"splice2");
        downLoadSize += ret;

        if(downLoadSize-oldSize>sliceSize)
        {
            printf("\r%5.2f%%",(float)downLoadSize/fileSize*100);
            fflush(stdout);
            oldSize=downLoadSize;
        }
    }
    //接最后小于SPLICE_BLOCK的部分
    ret=splice(socket_fd,NULL,fds[1],NULL,fileSize-downLoadSize,SPLICE_F_MORE|SPLICE_F_MOVE);
    ERROR_CHECK(ret,-1,"splice1");
    ret=splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MORE|SPLICE_F_MOVE);
    ERROR_CHECK(ret,-1,"splice2");
    
    gettimeofday(&end,NULL);
    printf("\r100.00%%\n");
    printf("cost time: %ld us\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    
    close(fd);
	close(socket_fd);
	return 0;
}

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
	fd=open(buf,O_CREAT|O_RDWR,0666);
	ERROR_CHECK(fd,-1,"open");

    //接收文件大小
    off_t fileSize=0,oldSize=0,downLoadSize=0,sliceSize;
    recvCycle(socket_fd,&dataLen,4);
    recvCycle(socket_fd,&fileSize,dataLen);
    ftruncate(fd,fileSize); //创建一个大小相同的文件
    char *pMap=mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    
    struct timeval start,end;
    //接收文件内容
    //sliceSize=fileSize/10000;//每接收0.01%的文件打印一次百分比
    gettimeofday(&start,NULL);
    ret=recvCycle(socket_fd,pMap,fileSize);
    if(-1==ret) return -1;
    munmap(pMap,fileSize);
    gettimeofday(&end,NULL);
    printf("\r100.00%%\n");
    printf("cost time: %ld us\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    
    close(fd);
	close(socket_fd);
	return 0;
}

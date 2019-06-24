#include "function.h"

int tranFile(int new_fd)
{
	Train train;
	int ret;

    //发送文件名
	train.dataLen=strlen(FILENAME);
	strcpy(train.buf,FILENAME);
	ret=send(new_fd,&train,4+train.dataLen,0);
	ERROR_CHECK(ret,-1,"send");
    int fd=open(FILENAME,O_RDONLY);
	ERROR_CHECK(fd,-1,"open");

    //发送文件大小
    struct stat buf;
    fstat(fd,&buf);
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    ret=send(new_fd,&train,4+train.dataLen,0);
    ERROR_CHECK(ret,-1,"send");

    //mmap发送文件内容
    off_t ptrPos=0;
    char *pMap=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    ret=send(new_fd,pMap,buf.st_size,0);
    if(-1==ret) return -1;

    //发送结束标志
	//ret=send(new_fd,&train,4,0);
   // ERROR_CHECK(ret,-1,"send");
	return 0;
}

#include "tranfile.h"

int tranFile(int new_fd)
{
    Train train;
    int ret;
    train.dataLen=strlen(FILENAME);
    strcpy(train.buf,FILENAME);

    //发送文件名
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

    //发送文件内容
    off_t ptrPos=0;//记录目前mmap指针的偏移
    char *pMap=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    //ret=send(new_fd,pMap,buf.st_size,0);
    
    if(buf.st_size>1000)
    {
        while(ptrPos+1000<buf.st_size)
        {
            memcpy(train.buf,pMap+ptrPos,sizeof(train.buf));
            train.dataLen=1000;
            ptrPos += 1000;
            ret=send(new_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");
        }
        train.dataLen=buf.st_size-ptrPos;
    }else{
        train.dataLen=buf.st_size;
    }

    //发送最后小于1000的部分
    memcpy(train.buf,pMap+ptrPos,train.dataLen);
    ret=send(new_fd,&train,4+train.dataLen,0);
    ERROR_CHECK(ret,-1,"send");

    //发送结束标志
    train.dataLen=0;
    ret=send(new_fd,&train,4,0);
    ERROR_CHECK(ret,-1,"send");

    return 0;
}

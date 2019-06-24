#include "tranfile.h"
//循环发送,退出机制
int sendCycle(int fd,void* p,int sendLen)
{
    int total=0;
    int ret;
    char *pStart=(char*)p;
    while(total<sendLen)
    {
        ret=send(fd,pStart+total,sendLen-total,0);
        if(-1==ret)
        {
            return -1;
        }
        total=total+ret;
    }
    return 0;
}
//循环接收
int recvCycle(int fd,void* p,int recvLen)
{
    int total=0;
    int ret;
    char *pStart=(char*)p;
    while(total<recvLen)
    {
        ret=recv(fd,pStart+total,recvLen-total,0);
        if(!ret) return -1;
        total=total+ret;
    }
    return 0;
}

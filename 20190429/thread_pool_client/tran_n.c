#include "function.h"

int recvCycle(int new_fd,void *p,int len)
{
    int total=0;
    int ret;
    char *pStart=(char*)p;
    while(total<len)
    {
        ret=recv(new_fd,pStart+total,len-total,0);
        if(0==ret){
            return -1;
        }
        total=total+ret;
    }
    return 0;
}


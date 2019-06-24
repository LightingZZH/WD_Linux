#include "function.h"

int recvCycle(int new_fd,void *p,int len)
{
    int total=0;
    int ret;
    char *pStart=(char*)p;
   // printf("len=%d",len);
    while(total<len)
    {
        ret=recv(new_fd,pStart+total,len-total,0);
       // printf("ret=%d",ret);
        if(0==ret){
            return -1;
        }
        total=total+ret;
        //进度条
        printf("\r%5.2f%%",(float)total/len*100);
        fflush(stdout);
    }
    return 0;
}


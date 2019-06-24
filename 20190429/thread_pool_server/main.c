#include "factory.h"

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,5);
    Factory Fac;
    int threadNum=atoi(argv[3]);
    int capacity=atoi(argv[4]);
    factoryInit(&Fac,threadNum,capacity);
    factoryStart(&Fac);

    int socket_fd;
    tcpInit(&socket_fd,argv[1],argv[2]);
    int new_fd;
    pQue que=&Fac.que;

    while(1)
    {
        new_fd=accept(socket_fd,NULL,NULL);
        pNode pNew=(pNode)calloc(1,sizeof(Node));
        pNew->new_fd=new_fd;
        pthread_mutex_lock(&que->mutex);
        queInsert(que,pNew);
        pthread_mutex_unlock(&que->mutex);
        pthread_cond_signal(&Fac.cond);
    }
}

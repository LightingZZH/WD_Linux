#include "factory.h"

#define THREAD_NUM 10
#define CAPACITY 10
#define IP "192.168.3.155"
#define PORT "2000"

int main()
{
    //ARGS_CHECK(argc,5);
    Factory Fac;
    int threadNum=THREAD_NUM; //atoi(argv[3]);
    int capacity=CAPACITY; //atoi(argv[4]);
    factoryInit(&Fac,threadNum,capacity);
    factoryStart(&Fac);

    int socket_fd;
    tcpInit(&socket_fd,IP,PORT);
   // tcpInit(&socket_fd,argv[1],argv[2]);
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

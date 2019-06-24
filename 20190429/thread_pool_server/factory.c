#include "factory.h"

//线程函数
void* downLoadFile(void *p)
{
    pFactory pFac=(pFactory)p;
    pQue que=&pFac->que;
    pNode pNew;
    int ret;
    while(1)
    {
        pthread_mutex_lock(&que->mutex);
        if(!que->queSize)//如果队列为空，就睡觉等信号
        {
            pthread_cond_wait(&pFac->cond,&que->mutex);
        }
        ret=queGet(que,&pNew);
        pthread_mutex_unlock(&que->mutex);
        if(!ret)    //取得队列中的节点成功(即可以取得new_fd)
        {
            tranFile(pNew->new_fd);
            free(pNew);
        }
    }
}

void factoryInit(pFactory pFac,int threadNum,int capacity)
{
    memset(pFac,0,sizeof(Factory));
    pFac->pthId=(pthread_t*)calloc(threadNum,sizeof(pthread_t));
    pFac->threadNum=threadNum;
    pthread_cond_init(&pFac->cond,NULL);
    queInit(&pFac->que,capacity);
}

void factoryStart(pFactory pFac)
{
    int i;
    if(0==pFac->startFlag)
    {
        for(i=0;i<pFac->threadNum;i++)
        {
            pthread_create(pFac->pthId+i,NULL,downLoadFile,pFac);
        }
        pFac->startFlag=1;
    }
}

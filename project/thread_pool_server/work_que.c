#include "work_que.h"

void queInit(pQue que,int capacity)
{
    memset(que,0,sizeof(Que));  //方便移植
    que->queCapacity=capacity;
    pthread_mutex_init(&que->mutex,NULL);
}

void queInsert(pQue que,pNode pNew)
{
    if(!que->queHead)
    {
        que->queHead=pNew;
        que->queTail=pNew;
    }else{
        que->queTail->pNext=pNew;
        que->queTail=pNew;
    }
    ++que->queSize;
}

int queGet(pQue que,pNode* p)
{
    if(!que->queSize)
    {
        return -1;
    }else{
        *p=que->queHead;
        que->queHead=que->queHead->pNext;
        if(!que->queHead)
        {
            que->queTail=NULL;
        }
        --que->queSize;
        return 0;
    }
}

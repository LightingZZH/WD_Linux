#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"

typedef struct tag_node{
    int new_fd;
    struct tag_node *pNext;
}Node,*pNode;

typedef struct{
    pNode queHead,queTail;
    int queCapacity;
    int queSize;
    pthread_mutex_t mutex;
}Que,*pQue;

void queInit(pQue,int);
void queInsert(pQue,pNode);
int queGet(pQue,pNode*);
#endif

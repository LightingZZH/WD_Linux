#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "tranfile.h"

typedef struct{
    pthread_t *pthId;
    int threadNum;
    pthread_cond_t cond;
    Que que;
    short startFlag;
}Factory,*pFactory;

void factoryInit(pFactory,int,int);
void factoryStart(pFactory);
int tcpInit(int *,char *,char *);
#endif

#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"

typedef struct{
    int dataLen;
    char buf[1000];
}Train;

int tranFile(int);
int recvCycle(int,void*,int);
int sendCycle(int,void*,int);
#define FILENAME "file"
#endif

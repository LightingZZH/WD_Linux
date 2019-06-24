#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"

typedef struct{
    int dataLen;
    char buf[1000];
}Train;

int tranFile(int);
int recvCycle(int,char*,int);
int sendCycle(int,char*,int);
#define FILENAME "file"
#endif

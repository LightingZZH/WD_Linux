#include <func.h>

int semid;

void init()
{
	semid=semget(1000,2,IPC_CREAT|0600);
}

int main()
{
	printf("productor: %d, consumer: %d\n",semctl(semid,1,GETVAL),semctl(semid,0,GETVAL));
	while(1);
}

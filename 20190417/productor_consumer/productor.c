#include <func.h>

int semid;

void init()
{
	semid=semget(1000,2,IPC_CREAT|0600);
	unsigned short arr[2]={10,0};
	semctl(semid,0,SETALL,arr);
}

int main()
{
	init();
	printf("productor: %d ,space: %d\n",semctl(semid,1,GETVAL),semctl(semid,0,GETVAL));
	while(1);
}

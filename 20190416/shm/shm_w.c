#include <func.h>
int main()
{
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	char *p;
	p=(char*)shmat(shmid,NULL,0);
	strcpy(p,"How are you");
	shmdt(p);
	return 0;
}

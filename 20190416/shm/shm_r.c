#include <func.h>
int main()
{
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	char *p;
	p=(char*)shmat(shmid,NULL,0);
	printf("%s\n",p);
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}

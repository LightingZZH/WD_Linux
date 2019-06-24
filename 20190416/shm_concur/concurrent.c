#include <func.h>
#define N 10000000
int main()
{
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	int *p=(int*)shmat(shmid,NULL,0);
	*p=0;
	if(!fork()){
		for(int i=0;i<N;i++)
			(*p)++;
		return 0;
	}else{
		for(int i=0;i<N;i++)
			(*p)++;
		wait(NULL);
		printf("%d\n",*p);
		return 0;
	}
}

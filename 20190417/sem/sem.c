#include <func.h>
#define N 10000000
int main()
{
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	int *p=(int*)shmat(shmid,NULL,0);
	*p=0;

	int semid=semget(1000,1,IPC_CREAT|0600);
	ERROR_CHECK(semid,-1,"semget");
    int ret=semctl(semid,0,SETVAL,1);
	ERROR_CHECK(ret,-1,"semctl");
	int semval=semctl(semid,0,GETVAL);
	ERROR_CHECK(semval,-1,"semctl");
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;

	struct timeval start,end;
	gettimeofday(&start,NULL);
	if(!fork()){
		for(int i=0;i<N;i++){
			semop(semid,&sopp,1);
			(*p)++;
			semop(semid,&sopv,1);
		}
		return 0;
	}else{
		for(int i=0;i<N;i++){
			semop(semid,&sopp,1);
			(*p)++;
			semop(semid,&sopv,1);
		}
		wait(NULL);
		gettimeofday(&end,NULL);
		printf("%d time: %ld\n",*p,(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
		shmctl(shmid,IPC_RMID,0);	//删除共享内存
		semctl(semid,IPC_RMID,0);	//删除信号量
		return 0;
	}
}

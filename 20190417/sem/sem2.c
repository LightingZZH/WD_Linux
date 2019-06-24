#include <func.h>
#define N 10000
//计数信号量卖票
int main()
{
	int semid=semget(1000,3,IPC_CREAT|0600);
	ERROR_CHECK(semid,-1,"semget");
	unsigned int arr[3]={N,0,0};
	int ret=semctl(semid,0,SETALL,arr);
	ERROR_CHECK(ret,-1,"semctl");
	struct sembuf sopp,sopv;
	if(!fork()){
		sopp.sem_num=0;
		sopp.sem_op=-1;
		sopp.sem_flg=0;//SEM_UNDO;
		sopv.sem_num=1;
		sopv.sem_op=1;
		sopv.sem_flg=0;//SEM_UNDO;
		while(1){
			if(semctl(semid,0,GETVAL)<=0) break;
			semop(semid,&sopp,1);
			semop(semid,&sopv,1);
		}
		return 0;
	}else{

		sopp.sem_num=0;
		sopp.sem_op=-1;
		sopp.sem_flg=0;//SEM_UNDO;
		sopv.sem_num=2;
		sopv.sem_op=1;
		sopv.sem_flg=0;//SEM_UNDO;
		while(1){
			if(semctl(semid,0,GETVAL)<=0) break;
			semop(semid,&sopp,1);
			semop(semid,&sopv,1);
		}
		wait(NULL);
		printf("child: %d  parent: %d\n",semctl(semid,1,GETVAL),semctl(semid,2,GETVAL));
		return 0;
	}
}

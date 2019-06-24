#include <func.h>
#define N 20000000
int main()
{
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	int *p=(int*)shmat(shmid,NULL,0);
	p[0]=N;
	p[1]=0;
	p[2]=0;

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

	if(!fork()){
		while(1){
			semop(semid,&sopp,1);
			if(p[0]==0){
				semop(semid,&sopv,1);
			   	break;
			}
			else{
				p[0]--;
				p[1]++;
			}
			semop(semid,&sopv,1);
		}
		printf("child is ok\n");
		return 0;
	}else{
		while(1){
			semop(semid,&sopp,1);
			if(p[0]==0){
			   	semop(semid,&sopv,1);	
				break;
			}
			else{
				p[0]--;
				p[2]++;
			}
			semop(semid,&sopv,1);
		}
		printf("parent is ok\n");
		wait(NULL);
		printf("child: %d  parent: %d\n",p[1],p[2]);
		shmctl(shmid,IPC_RMID,0);	//删除共享内存
		semctl(semid,IPC_RMID,0);	//删除信号量
		return 0;
	}
}

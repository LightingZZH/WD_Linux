#include <func.h>
#define N 1024

typedef struct{
	int useful;
	char buf[N];
}Text,*pText;

int main()
{
	//初始化信号量
	int semid=semget(1000,1,IPC_CREAT|0600);
	ERROR_CHECK(semid,-1,"semget");
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;	

	//初始化共享内存	
	int shmid=shmget(1000,sizeof(Text),0600|IPC_CREAT);
	ERROR_CHECK(shmid,-1,"shmget");
	printf("shmid=%d\n",shmid);
	pText p=(pText)shmat(shmid,NULL,0);
	while(1){
		semop(semid,&sopp,1);
		if(p->useful==1){
			printf("%50s",p->buf);
			p->useful=0;
		}
		if(p->useful==2){
			printf("%s",p->buf);
			p->useful=0;
		}
		semop(semid,&sopv,1);
	}
}

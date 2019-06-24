#include <func.h>
int main()
{
	int semid;
	semid=semget(1000,2,IPC_CREAT|0600);//一个信号量集有两个信号量
	ERROR_CHECK(semid,-1,"semget");
	unsigned short arr[2]={10,0};//arr[0]表示库空闲个数 arr[1]表示生产个数
	int ret=semctl(semid,0,SETALL,arr);//因为是设置所有信号量，所以第二个参数没有用，但是得写
	ERROR_CHECK(ret,-1,"semctl");
	struct sembuf sopp,sopv;
	if(!fork()){
		//子进程生产者
		sopp.sem_num=0;
		sopp.sem_op=-1;
		sopp.sem_flg=0;//SEM_UNDO;
		sopv.sem_num=1;
		sopv.sem_op=1;
		sopv.sem_flg=0;//SEM_UNDO;
		while(1){
			printf("I am producer, productor = %d,space = %d\n",semctl(semid,1,GETVAL),semctl(semid,0,GETVAL));
			semop(semid,&sopp,1);
			printf("I will produce\n");
			semop(semid,&sopv,1);
			printf("I am producer, productor = %d,space = %d\n",semctl(semid,    1,GETVAL),semctl(semid,0,GETVAL));
			sleep(1);
		}
	}else{
		//父进程消费者
		sopp.sem_num=1;
		sopp.sem_op=-1;
		sopp.sem_flg=0;//SEM_UNDO;
		sopv.sem_num=0;
		sopv.sem_op=1;
		sopv.sem_flg=0;//SEM_UNDO;
		while(1){
			printf("I am consumer, productor = %d,space = %d\n",semctl(semid,    1,GETVAL),semctl(semid,0,GETVAL));
			semop(semid,&sopp,1);
			printf("I will consume\n");
			semop(semid,&sopv,1);
			printf("I am consumer, productor = %d,space = %d\n",semctl(semid,        1,GETVAL),semctl(semid,0,GETVAL));
			sleep(2);
		}
	}
	return 0;
}

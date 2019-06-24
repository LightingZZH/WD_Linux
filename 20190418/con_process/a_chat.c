#include <func.h>
#define N 1024
typedef struct{
	int useful;
	char buf[N];
}Text,*pText;

pid_t a_pid;
pid_t b_pid;
int shmid;
int semid;
pText p;

void sigFunc(int signum)
{
	kill(b_pid,2);
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);//删除共享内存
	semctl(semid,IPC_RMID,0);//删除信号量	 
	kill(a_pid,9);
}

int main(int argc,char *argv[2])
{
	ARGS_CHECK(argc,3);
	int ret;

	//初始化信号量
	semid=semget(1000,1,IPC_CREAT|0600);
	ERROR_CHECK(semid,-1,"semget");
	ret=semctl(semid,0,SETVAL,1);
	ERROR_CHECK(ret,-1,"semctl");
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;

	//初始化共享内存
	shmid=shmget(1000,sizeof(Text),0600|IPC_CREAT);
	ERROR_CHECK(shmid,-1,"shmget");
	printf("shmid=%d\n",shmid);
    pText p=(pText)shmat(shmid,NULL,0);

	//初始化管道
	int fdr,fdw;
	fdr=open(argv[1],O_RDONLY);
	ERROR_CHECK(fdr,-1,"open");
	fdw=open(argv[2],O_WRONLY);
	ERROR_CHECK(fdw,-1,"open");
	printf("chat1 fdr=%d,fdw=%d\n",fdr,fdw);
	a_pid=getpid();
	printf("a_pid=%d\n",a_pid);

	fd_set rdset;

	write(fdw,&a_pid,sizeof(pid_t));
	read(fdr,&b_pid,sizeof(pid_t));
	printf("b_pid=%d\n",b_pid);

	signal(SIGINT,sigFunc);
	while(1){
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0){
			if(FD_ISSET(fdr,&rdset)){	//读管道
				semop(semid,&sopp,1);
				memset(p,0,sizeof(Text));
				p->useful=2;			//信息是他人的
				ret=read(fdr,p->buf,N);
				ERROR_CHECK(ret,-1,"read");
				semop(semid,&sopv,1);
			}
			if(FD_ISSET(STDIN_FILENO,&rdset)){ //读标准输入，写管道
				semop(semid,&sopp,1);
				memset(p,0,sizeof(Text));
				p->useful=1;			//信息是自己的
				ret=read(STDIN_FILENO,p->buf,N);
				write(fdw,p->buf,strlen(p->buf));
				semop(semid,&sopv,1);
			}
		}
	}
	return 0;
}

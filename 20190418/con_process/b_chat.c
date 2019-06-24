#include <func.h>
#define N 1024

typedef struct{
	long type;
	char text[N];
}MQ;

pid_t a_pid;
pid_t b_pid;
int msgid;
int semid;
MQ sndmsg;

void sigFunc(int signum)
{
	kill(a_pid,2);
	sndmsg.type=3;
	msgsnd(msgid,&sndmsg,N,0);
//	semctl(semid,IPC_CREAT,0);
	kill(b_pid,9);
}

int main(int argc,char *argv[2])
{
	ARGS_CHECK(argc,3);
	int ret;
	char buf[N];

	//初始化信号量
//	semid=semget(2000,1,IPC_CREAT|0600);
//	ERROR_CHECK(semid,-1,"semget");
//	ret=semctl(semid,0,SETVAL,1);
//	ERROR_CHECK(ret,-1,"semctl");
//	struct sembuf sopp,sopv;
//	sopp.sem_num=0;
//	sopp.sem_op=-1;
//	sopp.sem_flg=SEM_UNDO;
//	sopv.sem_num=0;
//	sopv.sem_op=1;
//	sopv.sem_flg=SEM_UNDO;

	//初始化消息队列
	msgid=msgget(1000,IPC_CREAT|0600);
	ERROR_CHECK(msgid,-1,"msgget");
	printf("msgid=%d\n",msgid);

	int fdr,fdw;
	fdw=open(argv[1],O_WRONLY);
	ERROR_CHECK(fdw,-1,"open");
	fdr=open(argv[2],O_RDONLY);
	ERROR_CHECK(fdr,-1,"open");
	printf("chat2 fdr=%d,fdw=%d\n",fdr,fdw);
	b_pid=getpid();
	printf("b_pid=%d\n",b_pid);

	fd_set rdset;

	write(fdw,&b_pid,sizeof(pid_t));
	read(fdr,&a_pid,sizeof(pid_t));
	printf("a_pid=%d\n",a_pid);

	signal(SIGINT,sigFunc);
	while(1){
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0){
			if(FD_ISSET(fdr,&rdset)){		//读管道
//				semop(semid,&sopp,1);
				memset(&sndmsg,0,sizeof(MQ));
				memset(buf,0,sizeof(buf));
				sndmsg.type=2;
				ret=read(fdr,buf,N);
				ERROR_CHECK(ret,-1,"read");
				strcpy(sndmsg.text,buf);
				ret=msgsnd(msgid,&sndmsg,strlen(sndmsg.text),0);
				ERROR_CHECK(ret,-1,"msgsnd");
//				semop(semid,&sopv,1);
			}
			if(FD_ISSET(STDIN_FILENO,&rdset)){	//读标准输入，写管道
//				semop(semid,&sopp,1);
				memset(&sndmsg,0,sizeof(MQ));
				memset(buf,0,N);
				sndmsg.type=1;
				ret=read(STDIN_FILENO,buf,N);
				ERROR_CHECK(ret,-1,"read");
				ret=write(fdw,buf,strlen(buf));
				ERROR_CHECK(ret,-1,"write");
				strcpy(sndmsg.text,buf);
				ret=msgsnd(msgid,&sndmsg,strlen(sndmsg.text),0);
				ERROR_CHECK(ret,-1,"msgsnd");
//				semop(semid,&sopv,1);
			}
		}
	}
	return 0;
}

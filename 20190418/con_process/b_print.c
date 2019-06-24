#include <func.h>
#define N 1024

typedef struct{
	long type;
	char text[N];
}MQ;

int msgid;
int semid;

int main()
{
	int ret;

	//初始化信号量
//	semid=semget(2000,1,IPC_CREAT|0600);
//	ERROR_CHECK(semid,-1,"semget");
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
	MQ rcvmsg;

	while(1){
	//	semop(semid,&sopp,1);
		memset(&rcvmsg,0,sizeof(MQ));
		ret=msgrcv(msgid,&rcvmsg,sizeof(rcvmsg.text),0,0);
		ERROR_CHECK(ret,-1,"msgrcv");
		if(rcvmsg.type==1){
			printf("%50s",rcvmsg.text);
		}
		if(rcvmsg.type==2){
			printf("%s",rcvmsg.text);
		}
		if(rcvmsg.type==3){
		   	msgctl(msgid,IPC_RMID,NULL);
//			semop(semid,&sopv,1);
			exit(0);
		}
	//	semop(semid,&sopv,1);
	}
	return 0;
}

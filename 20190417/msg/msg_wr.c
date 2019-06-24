#include <func.h>

typedef struct mqbuf{
	long type;
	char text[64];
}Msg;

int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int msgid=msgget(1000,IPC_CREAT|0600);
	ERROR_CHECK(msgid,-1,"msgget");
	Msg wrmsg;
	wrmsg.type=atoi(argv[1]);
	strcpy(wrmsg.text,argv[2]);
	int ret=msgsnd(msgid,&wrmsg,strlen(wrmsg.text),0);
	ERROR_CHECK(ret,-1,"msgsnd");
	return 0;
}

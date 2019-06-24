#include <func.h>

int main()
{
	sigset_t procMask;
	sigemptyset(&procMask);
	sigaddset(&procMask,SIGINT);
	int ret;
	ret=sigprocmask(SIG_BLOCK,&procMask,NULL);
	ERROR_CHECK(ret,-1,"sigprocmask");
	sleep(5);
	sigset_t pending;
	sigemptyset(&pending);
	sigpending(&pending);
	if(sigismember(&pending,SIGINT)){
		printf("\nSIGINT is pending\n");
	}else{
		printf("\nSIGINT isn't pending\n");
	}
	sigprocmask(SIG_UNBLOCK,&procMask,NULL);
	while(1);
}

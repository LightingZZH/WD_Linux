#include <func.h>
pid_t pid;
void sigFunc(int signum)
{
//	printf("pid=%d\n",pid);
	kill(pid,9);
}
int main()
{	
	pid=getpid();
	printf("pid=%d\n",pid);
	signal(SIGINT,sigFunc);
	while(1);
}

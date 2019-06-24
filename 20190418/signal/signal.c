#include <func.h>

void sigFunc(int signum)
{
	printf(" before sleep: %d is coming\n",signum);
	sleep(3);
	printf(" after sleep: %d is coming\n",signum);
}

int main()
{
	if(signal(SIGINT,sigFunc)==SIG_ERR){
		perror("signal");
	}
	signal(SIGQUIT,sigFunc);
	while(1);
}

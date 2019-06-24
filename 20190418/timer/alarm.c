#include <func.h>
void sigFunc(int signum)
{
	printf(" %d is coming\n",signum);
}
int main()
{
	signal(SIGALRM,sigFunc);
	alarm(5);
	printf("before pause\n");
	pause();
	printf("after pause\n");
	return 0;
}

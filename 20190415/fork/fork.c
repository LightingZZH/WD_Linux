#include <func.h>
int main()
{
	pid_t ret=fork();
	if(ret==0){
		printf("child pid=%d\n",getpid());
		printf("this is child process\n");
		while(1);
	}else{
		printf("parent pid=%d\n",getpid());
		printf("this is parent process\n");
		while(1);
	}
	return 0;
}

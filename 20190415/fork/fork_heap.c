#include <func.h>
int main()
{
	pid_t pid;
	int num=10;
	char *p=(char*)malloc(20);
	strcpy(p,"hello");
	pid=fork();
	if(0==pid){
		printf("this is child process\n");
		printf("num=%d,%s\n",num,p);
	}else{
		printf("this is parent process\n");
		printf("num=%d,%s\n",num,p);
		strcpy(p,"world");
		num=5;
		printf("num=%d,%s\n",num,p);
		sleep(1);
	}
	return 0;
}

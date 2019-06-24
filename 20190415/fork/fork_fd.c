#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,2);
	pid_t pid;
	int fd;
	fd=open(argv[1],O_RDWR);
	ERROR_CHECK(fd,-1,"open");
	pid=fork();
	if(0==pid){
		printf("this is child process\n");
		write(fd,"world",5);
	}else{
		printf("this is parent process\n");
		write(fd,"hello",5);
		sleep(1);
	}
	return 0;
}

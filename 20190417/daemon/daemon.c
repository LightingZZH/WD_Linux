#include <func.h>
int main()
{
	pid_t pid;
	pid=fork();
	if(pid) exit(0);
	else{
		setsid();
		chdir("/");
		umask(0);
		for(int i=0;i<3;i++) close(i);
		while(1){
			int fd=open("/tmp/log",O_WRONLY|O_CREAT);
			lseek(fd,0,SEEK_END);
			write(fd,"I have run 5s",13);
			close(fd);
			sleep(5);
		}
		return 0;
	}
}

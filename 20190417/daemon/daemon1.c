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
			int fd=open("/tmp/log",O_WRONLY|O_CREAT,0600);//也可使用O_APPEND追加的方式打开
			lseek(fd,0,SEEK_END);
			write(fd,"I have run 5s\n",14);
			close(fd);
			sleep(5);
		}
		return 0;
	}
}

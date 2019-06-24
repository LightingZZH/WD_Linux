#include <func.h>
int main()
{
	int fds[2];
	pipe(fds);
	if(!fork()){
		printf("this is child process\n");
		close(fds[0]);
		write(fds[1],"Hello",5);
	}else{
		close(fds[1]);
		wait(NULL);
		char buf[128]={0};
		read(fds[0],buf,sizeof(buf));
		printf("this is parent process\n");
		printf("%s\n",buf);
	}
}

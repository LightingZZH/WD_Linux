#include <func.h>

int main()
{
	int fds[2];
	pipe(fds);
	if(!fork())
	{
		close(fds[1]);
		int fd;
		read(fds[0],&fd,sizeof(fd));
		printf("child fd=%d\n",fd);
		char buf[128]={0};
		read(fd,buf,sizeof(buf));
		printf("buf=%s\n",buf);
		return 0;
	}else{
		close(fds[0]);
		int fd;
		fd=open("file",O_RDWR);
		printf("parent fd=%d\n",fd);
		write(fds[1],&fd,sizeof(fd));
		wait(NULL);
		return 0;
	}
}

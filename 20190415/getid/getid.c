#include <func.h>
int main()
{
	printf("pid=%d ppid=%d\n",getpid(),getppid());
	printf("uid=%d gid=%d\n",getuid(),getgid());
	printf("euid=%d egid=%d\n",geteuid(),getegid());
	int fd;
	fd=open("file",O_RDWR);
	write(fd,"hello",5);
	return 0;
}

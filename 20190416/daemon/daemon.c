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
		while(1);
		return 0;
	}
}

#include "function.h"

int makeChild(process_data *pChild,int childNum)
{
	int i;
	int ret;
	int fds[2];
	pid_t pid;
	for(i=0;i<childNum;i++)
	{
		ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//创建一对无名的套接字描述符(全双工)
		//第一个参数(domain)表示协议族(域)
		//第二个参数(type)表示协议(TCP/UDP)
		//第三参数(protocol)表示类型,只能为0
		//第四个参数sv[2]表示套接字柄对
		ERROR_CHECK(ret,-1,"socketpair");
		pid=fork();
		if(0==pid){
			close(fds[1]);
			childHandle(fds[0]);
		}
		close(fds[0]);
		pChild[i].pid=pid;
		pChild[i].fd=fds[1];
		pChild[i].busy=0;
	}
	return 0;
}

int childHandle(int fd)
{
	int new_fd;
	while(1)
	{
		recvFd(fd,&new_fd);//接收到任务
		tranFile(new_fd);
		close(new_fd);
		write(fd,&new_fd,1);//通知父进程，该子进程非忙碌了
	}
}

#include "function.h"

int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,4);
	int childNum=atoi(argv[3]);
	process_data *pChild=(process_data *)calloc(childNum,sizeof(process_data));
	makeChild(pChild,childNum);//生成子进程
	int socket_fd;
	tcpInit(&socket_fd,argv[1],argv[2]);

	int epfd;
	int ret;
	epfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(childNum+1,sizeof(struct epoll_event));
	event.events=EPOLLIN;
	event.data.fd=socket_fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,socket_fd,&event);
	ERROR_CHECK(ret,-1,"epoll_ctl");

	int i;
	int j;
	int new_fd;
	for(i=0;i<childNum;i++)
	{
		event.data.fd=pChild[i].fd;
		ret=epoll_ctl(epfd,EPOLL_CTL_ADD,pChild[i].fd,&event);
		ERROR_CHECK(ret,-1,"epoll_ctl");
	}

#ifdef DEBUG
	printf("epfd=%d,socket_fd=%d\n",epfd,socket_fd);
#endif

	int readyFdNum;
	while(1)
	{
		readyFdNum=epoll_wait(epfd,evs,childNum+1,-1);
		for(i=0;i<readyFdNum;i++)
		{
			if(evs[i].events==EPOLLIN && evs[i].data.fd==socket_fd)
			{
				new_fd=accept(socket_fd,NULL,NULL);
                printf("main new_fd=%d\n",new_fd);
				for(j=0;j<childNum;j++)
				{
					if(!pChild[j].busy){
						sendFd(pChild[j].fd,new_fd);
						pChild[j].busy=1;
						printf("%d child is busy\n",pChild[j].pid);
						break;
					}
				}
				close(new_fd);
			}
			for(j=0;j<childNum;j++)
			{
				if(evs[i].data.fd==pChild[j].fd)
				{
					ret=read(pChild[j].fd,&ret,1);
                   // printf("ret=%d\n",ret);
					pChild[j].busy=0;
					printf("%d child is not busy\n",pChild[j].pid);
                }
			}
		}
	}
	return 0;
}

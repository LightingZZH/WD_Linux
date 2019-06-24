#include <func.h>

void changeNoblock(int fd)
{
	int status=fcntl(fd,F_GETFL);
	status=status|O_NONBLOCK;
	fcntl(fd,F_SETFL,status);
}

int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);//ipv4,tcp
	ERROR_CHECK(socket_fd,-1,"socket");

	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);

	int ret;
	ret=bind(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"bind");
	listen(socket_fd,10);

	int new_fd;
	struct sockaddr_in client;
	int addrlen=sizeof(client);
	bzero(&client,sizeof(client));

	new_fd=accept(socket_fd,(struct sockaddr*)&client,&addrlen);
	ERROR_CHECK(new_fd,-1,"accept");
	printf("client ip=%s,port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

	char buf[6]={0};

	int epfd=epoll_create(1);	//创建一个监听数目为1的epoll句柄
	ERROR_CHECK(epfd,-1,"epoll_creat");
	struct epoll_event event,evs[2];
	event.events=EPOLLIN;	//表示文件描述符可读
	event.data.fd=STDIN_FILENO;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);//内核监听标准输入是否可读
	ERROR_CHECK(ret,-1,"epoll_ctl");

	event.events=EPOLLIN|EPOLLET;
	changeNoblock(new_fd);
	event.data.fd=new_fd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);

	int i;
	int ready_fd_num;
	while(1)
	{
		bzero(evs,sizeof(evs));
		ready_fd_num=epoll_wait(epfd,evs,2,-1);
		for(i=0;i<ready_fd_num;i++)
		{
			if(evs[i].data.fd==new_fd){
				while(1){
					bzero(buf,sizeof(buf));
					ret=recv(new_fd,buf,sizeof(buf)-1,0);
					if(0==ret){
						printf("bye bye!\n");
						goto chat_over;
					}else if(-1==ret){
						break;
					}else{
						printf("%s",buf);
					}
				}
			}
			if(0==evs[i].data.fd){
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(0==ret){
					printf("bye bye!\n");
					goto chat_over;
				}
				ret=send(new_fd,buf,strlen(buf),0);
				ERROR_CHECK(ret,-1,"send");
			}
		}
	}
chat_over:
	close(new_fd);
	close(socket_fd);
	return 0;
}

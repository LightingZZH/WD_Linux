#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	int ret;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socket_fd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);

	ret=connect(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"connect");
	printf("connect success\n");

	char buf[128]={0};
	fd_set rdset;

	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(socket_fd,&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		ret=select(socket_fd+1,&rdset,NULL,NULL,NULL);

		if(ret>0){
			if(FD_ISSET(socket_fd,&rdset)){
				bzero(&buf,sizeof(buf));
				ret=recv(socket_fd,buf,sizeof(buf),0);
				if(0==ret){
					printf("bye bye!\n");
					break;
				}
				printf("%s",buf);
			}
			if(FD_ISSET(STDIN_FILENO,&rdset)){
				bzero(&buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(0==ret){
					printf("bye bye!\n");
					break;
				}
				send(socket_fd,buf,sizeof(buf),0);
			}
		}else{
			printf("time out\n");
		}
	}
	close(socket_fd);
	return 0;
}

#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_DGRAM,0);
	ERROR_CHECK(socket_fd,-1,"socket_fd");
	
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);

	int ret;
	ret=sendto(socket_fd,"h",1,0,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"sendto");

	char buf[128]={0};
	fd_set rdset;

	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(socket_fd,&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		select(socket_fd+1,&rdset,NULL,NULL,NULL);
		if(FD_ISSET(socket_fd,&rdset)){
			bzero(buf,sizeof(buf));
			ret=recvfrom(socket_fd,buf,sizeof(buf),0,NULL,NULL);	//因为只有一个服务器,所以最后两个参数为NULL也可以
			ERROR_CHECK(ret,-1,"recvfrom");
			if(0==ret){
				printf("bye bye!\n");
				break;
			}
			printf("%s",buf);
		}
		if(FD_ISSET(STDIN_FILENO,&rdset)){
			bzero(buf,sizeof(buf));
			ret=read(STDIN_FILENO,buf,sizeof(buf));
			ERROR_CHECK(ret,-1,"read");
			if(0==ret){
				printf("bye bye!\n");
				break;
			}
			ret=sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr*)&ser,sizeof(ser));
			ERROR_CHECK(ret,-1,"sendto");
		}
	}
	close(socket_fd);
	return 0;
}

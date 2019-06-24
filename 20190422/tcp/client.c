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
	ret=send(socket_fd,"hello",5,0);
	ERROR_CHECK(ret,-1,"send");
	ret=recv(socket_fd,buf,sizeof(buf),0);
	ERROR_CHECK(ret,-1,"send");
	printf("I am client, gets: %s\n",buf);

	close(socket_fd);
	return 0;
}

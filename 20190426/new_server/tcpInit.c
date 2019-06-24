#include "function.h"

int tcpInit(int *pSocketFd,char *ip,char *port)
{
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socket_fd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(port));
	ser.sin_addr.s_addr=inet_addr(ip);
	int ret;
	ret=bind(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"bind");
	listen(socket_fd,10);
	*pSocketFd=socket_fd;
	return 0;
}

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
	ret=bind(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"bind");
	listen(socket_fd,10);

	struct sockaddr_in client;
	bzero(&client,sizeof(client));
	int addrlen=sizeof(client);
	int new_fd;
	
	new_fd=accept(socket_fd,(struct sockaddr*)&client,&addrlen);
	ERROR_CHECK(new_fd,-1,"accept");
	printf("client ip=%s ,port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	
	char buf[128]={0};
	ret=recv(new_fd,buf,sizeof(buf),0);
	ERROR_CHECK(ret,-1,"recv");
	printf("I am server, gets: %s\n",buf);
	ret=send(new_fd,"world",5,0);
	ERROR_CHECK(ret,-1,"send");

	close(new_fd);
	close(socket_fd);
	return 0;
}

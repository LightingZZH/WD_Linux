#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	int ret;

	socket_fd=socket(AF_INET,SOCK_DGRAM,0);//SOCK_DGRAM表示使用UDP协议
	ERROR_CHECK(socket_fd,-1,"socket");

	struct sockaddr_in ser;
	ser.sin_family=AF_INET;	//表示ipv4
	ser.sin_port=htons(atoi(argv[2]));	//主机字节序->网络字节序(端口号)
	ser.sin_addr.s_addr=inet_addr(argv[1]);	//点分十进制->网络字节序(IP)
	
	ret=bind(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	//UDP,bind后,socket_fd已经激活
	ERROR_CHECK(ret,-1,"bind");

	char buf[128]={0};	//UDP报文最大65535B,首部固定为8B
	struct sockaddr_in client;
	int addrlen=sizeof(client);
	ret=recvfrom(socket_fd,buf,1,0,(struct sockaddr*)&client,&addrlen);
	//UDP,客户端必须先发点东西,服务器才能知道它的端口号和ip
	ERROR_CHECK(ret,-1,"recvfrom");
	printf("client ip=%s, port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(socket_fd,&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		ret=select(socket_fd+1,&rdset,NULL,NULL,NULL);
		if(FD_ISSET(socket_fd,&rdset)){
			bzero(buf,sizeof(buf));
			ret=recvfrom(socket_fd,buf,sizeof(buf),0,(struct sockaddr*)&client,&addrlen);
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
			ret=sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(client));
			ERROR_CHECK(ret,-1,"sendto");
		}
	}
	close(socket_fd);
	return 0;
}

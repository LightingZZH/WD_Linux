#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socket_fd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"connect");
	printf("connect success\n");
	int dataLen;
	char buf[1000]={0};
	recv(socket_fd,&dataLen,4,0);
	recv(socket_fd,buf,dataLen,0);
	int fd;
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	ERROR_CHECK(fd,-1,"open");
	while(1)
	{
		recv(socket_fd,&dataLen,4,0);
		if(dataLen>0)
		{
			recv(socket_fd,buf,dataLen,0);
			write(fd,buf,dataLen);
		}else{
			break;
		}
	}
	close(fd);
	close(socket_fd);
	return 0;
}

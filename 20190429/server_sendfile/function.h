#include <sys/sendfile.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sys/msg.h>
#include <strings.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#define ARGS_CHECK(argc,val) {if(argc!=val) \
	{printf("error args\n"); return -1;}}
#define ERROR_CHECK(ret,retval,funcName) {if(ret==retval) \
	{printf("%d: ",__LINE__); fflush(stdout); perror(funcName); return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) \
	{printf("%s: %s\n",funcName,strerror(ret)); return -1;}

typedef struct{
	pid_t pid;
	int fd;
	short busy;
}process_data;

typedef struct{
	int dataLen;
	char buf[1000];
}Train;

#define FILENAME "file"

int makeChild(process_data*,int);//生成子进程
int childHandle(int);//子进程要干的事
int tcpInit(int*,char*,char*);//初始化TCP连接
int sendFd(int,int);//发送描述符
int recvFd(int,int*);//接收描述符
int tranFile(int);//传输文件

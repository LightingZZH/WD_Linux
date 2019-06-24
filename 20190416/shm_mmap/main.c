#include <func.h>
int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,2);
	int fd;
	fd=open(argv[1],O_RDWR);
	ERROR_CHECK(fd,-1,"open");
	char *p;
	struct stat buf;
	fstat(fd,&buf);
	p=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	ERROR_CHECK(p,(char*)-1,"mmap");
	if(!fork()){
		printf("i am child ,gets %s\n",p);
		return 0;
	}else{
		strcpy(p,"i am parent");
		wait(NULL);
		return 0;
	}
}

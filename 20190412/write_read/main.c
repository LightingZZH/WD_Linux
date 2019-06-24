#include <func.h>

struct student{
	int num;
	char name[20];
	float score;
}Stu,*pStu;

int main(int argc,char *argv[])
{
	int i;
	int ret;
	struct student tmp;
	struct student s[3]={{1001,"zhangsan",98.5},{1004,"lisi",85.5},{1002,"wangwu",88.5}};
	ARGS_CHECK(argc,2);
	int fd;
	fd=open(argv[1],O_RDWR|O_CREAT);
	ERROR_CHECK(fd,-1,"open");
	for(i=0;i<3;i++)
	{
		ret=write(fd,&s[i],sizeof(s[i]));
		ERROR_CHECK(ret,-1,"write");
	}
	lseek(fd,0,SEEK_SET);
	for(i=0;i<3;i++)
	{
		memset(&tmp,0,sizeof(tmp));
		ret=read(fd,&tmp,sizeof(tmp));
		ERROR_CHECK(ret,-1,"read");
		printf("%d %10s %5.2f\n",tmp.num,tmp.name,tmp.score);
	}
	return 0;
}

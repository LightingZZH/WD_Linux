#include <func.h>

int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int fdr,fdw;
	fdw=open(argv[1],O_WRONLY);
	ERROR_CHECK(fdw,-1,"open");
	fdr=open(argv[2],O_RDONLY);
	ERROR_CHECK(fdr,-1,"open");
	printf("char1 fdr=%d fdw=%d\n",fdr,fdw);
	char buf[128]={0};
	int ret;
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(fdr,&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(fdr,&rdset))
			{
				memset(buf,0,sizeof(buf));
				ret=read(fdr,buf,sizeof(buf));
				if(0==ret)
				{
					printf("bye bye!\n");
					break;
				}
				printf("%s\n",buf);
			}
			if(FD_ISSET(STDIN_FILENO,&rdset))
			{
				memset(buf,0,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(0==ret)
				{
					printf("bye bye!\n");
					break;
				}
				write(fdw,buf,strlen(buf)-1);
			}
		}
	}
	close(fdr);
	close(fdw);
	return 0;
}

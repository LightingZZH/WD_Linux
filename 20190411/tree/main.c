#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#define ARGS_CHECK(argc,val) {if(argc!=val) {printf("error args\n"); return -1;}}
#define ERROR_CHECK(ret,retval,funcName) {if(ret==retval) \
   	{perror(funcName); return -1;}}

int printDir(char *pathName,char *fileName)
{
	char path[512]={0};
	DIR *dir;
	chdir(pathName);
	dir = opendir(pathName);
	ERROR_CHECK(dir,NULL,"opendir");
	struct dirent *p;
	while((p=readdir(dir)))
	{
		if(!strcmp(p->d_name,fileName))
		{
			printf("%ld %d %d %s\n",p->d_ino ,p->d_reclen ,p->d_type ,p->d_name);
			sprintf(path,"%s%s%s",getcwd(NULL,0),"/",fileName);
			printf("%s\n",path);
			break;
		}
	}
	if(!p)
	{
		printf("no this file in this path\n");
	}
	closedir(dir);
	return 0;
}

int main(int argc ,char *argv[])
{
	ARGS_CHECK(argc,3);
	printDir(argv[1],argv[2]);
	return 0;
}

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

int printDir(char *pathName,int width)
{
	char path[512]={0};
	DIR *dir;
	dir = opendir(pathName);
	ERROR_CHECK(dir,NULL,"opendir");
	struct dirent *p;
	while((p=readdir(dir)))
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")) continue;
		printf("%*s%s\n",width,"-",p->d_name);
		sprintf(path,"%s%s%s",pathName,"/",p->d_name);
		if(4==p->d_type) printDir(path,width+4);
	}
	closedir(dir);
	return 0;
}

int main(int argc ,char *argv[])
{
	ARGS_CHECK(argc,2);
	printDir(argv[1],4);
	return 0;
}

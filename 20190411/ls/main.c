#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#define ARGS_CHECK(argc,val) {if(argc!=val) {printf("error args\n"); return -1;}}
#define ERROR_CHECK(ret,retval,funcName) {if(ret==retval) \
   	{perror(funcName); return -1;}}

int printDir(char *pathName)
{
	char path[512]={0};
	int ret;
	DIR *dir;
	dir = opendir(pathName);
	ERROR_CHECK(dir,NULL,"opendir");
	struct dirent *p;
	while((p=readdir(dir)))
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")) continue;
		sprintf(path,"%s%s%s",pathName,"/",p->d_name);
		struct stat buf;
		ret=stat(path,&buf);
		ERROR_CHECK(ret,-1,"stat");
		char mod[10];
		char type;
		mode_t mode = buf.st_mode;
		if((mode & S_IFMT)==S_IFDIR) type='d';
		else type='-';
		if((mode & 1) == 0) mod[8]='-';
		else mod[8]='x';
		if((mode & 2) == 0) mod[7]='-';
	   	else mod[7]='w';
		if((mode & 4) == 0) mod[6]='-';
	   	else mod[6]='r';
		if((mode & 8) == 0) mod[5]='-';
	   	else mod[5]='x';
		if((mode & 16) == 0) mod[4]='-';
	   	else mod[4]='w';
		if((mode & 32) == 0) mod[3]='-';
		else mod[3]='r';
		if((mode & 64) == 0) mod[2]='-';
		else mod[2]='x';
		if((mode & 128) == 0) mod[1]='-';
		else mod[1]='w';
		if((mode & 256) == 0) mod[0]='-';
		else mod[0]='r';
		mod[9]='\0';
		//printf("%c%c%c%c%c%c%c%c%c\n",mod[0],mod[1],mod[2],mod[3],mod[4],mod[5],mod[6],mod[7],mod[8]);
		printf("%c%s %ld %s %s %6ld %.12s %s\n",type,mod,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,(4+ctime(&buf.st_mtime)),p->d_name);
	}
	closedir(dir);
	return 0;
}

int main(int argc ,char *argv[])
{
	ARGS_CHECK(argc,2);
	printDir(argv[1]);
	return 0;
}

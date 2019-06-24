#ifndef __MYSQL_H__
#define __MYSQL_H__
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

int query(char*,char*,char*,char*);
int insert(char*,char*,char*);

//typedef struct{
//    int dataLen;
//    char buf[1000];
//}File,*pFile;

int ls(char*,char*,char*);
int cd(char*,char*,char*,char*);
int puts_query(char*,char*,char*);
int puts_insert(char*,char*,char*,char*,off_t);
int gets_query(char*,char*,char*,char*);
int remove_query(char*,char*,char*,char*);
int remove_delete(char*,char*,char*);
int remove_query1(char*);
int mdr(char*,char*,char*);
int User_log(char*,char*,char*);
#endif

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="test";
    char query[300]="insert into class(T_ID,S_ID) values(2,2)";
    printf("%s\n",query);

    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database: %s\n",mysql_error(conn));
    }else{
        printf("Connect success...\n");
    }

    t=mysql_query(conn,query);  //执行SQL语句
    if(t)
    {
        printf("Error making insert: %s\n",mysql_error(conn));
    }else{
        printf("insert success\n");
    }

    mysql_close(conn);
    printf("Connect close...\n");
    return 0;
}


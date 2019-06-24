#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="test";
    char query[200]="delete from class where T_ID=2";
    printf("%s\n",query);

    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));

    }else{
        printf("Connected...\n");

    }

    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));

    }else{
        printf("delete success, delete %ld row\n",(long)mysql_affected_rows(conn));
    }

    mysql_close(conn);
    printf("Connect close...\n");
    return 0;
}


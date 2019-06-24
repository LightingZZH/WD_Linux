#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("error args\n");
        return -1;
    }
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="test";
    char query[200]="update class set T_ID='"; 
    sprintf(query,"%s%s%s",query,argv[1],"' where S_ID=1");
    puts(query);

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
        printf("update success\n");
    }

    mysql_close(conn);
    printf("Connect close...\n");
    return 0;
}

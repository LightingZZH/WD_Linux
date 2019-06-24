#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
 //   if(argc!=2){
 //       printf("error args\n");
 //       return -1;
 //   }
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="test";  //数据库名称
    
    char query[300]="select * from teacher where ID='";
   // sprintf(query,"%s%s%s",query,argv[1],"'");
    strcpy(query,"select * from class");
    puts(query);

    unsigned int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database: %s\n",mysql_error(conn));
        return -1;
    }else{
        printf("Connect success...\n");
    }

    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query: %s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL) //按行的方式遍历查询结果
            {
                //printf("col=%d\n",mysql_num_fields(res));//列数
                for(t=0;t<mysql_num_fields(res);t++)    //打印一条记录的所有信息
                {
                    printf("%8s ",row[t]);
                }
                printf("\n");
            }
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    printf("Connect close...\n");
    return 0;
}

#include "user_sql.h"

int query(char *username,char* salt,char* ciphertext,char* id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from User where name='";
    sprintf(query,"%s%s%s",query, username,"'");
    //    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else{
                strcpy(id,row[0]);
                strcpy(salt,row[2]);
                strcpy(ciphertext,row[3]);
                mysql_free_result(res);
                mysql_close(conn);
                return 1;
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int insert(char* name,char *salt,char *ciphertext)
{
    MYSQL *conn;
    //MYSQL_RES *res;
    // MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";
    char query[200]="insert into User(name,salt,ciphertext) values('";
    sprintf(query,"%s%s%s",query,name,"','");
    sprintf(query,"%s%s%s",query,salt,"','");
    sprintf(query,"%s%s%s",query,ciphertext,"')");
    //    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));

    }else{
        //    printf("Connected...\n");

    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));

    }else{
        //  printf("insert success\n");
    }
    mysql_close(conn);
    return 0;
}

int ls(char *precode,char *belongID,char *buf)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {
                sprintf(buf,"%s%s",buf,row[4]);
                sprintf(buf,"%s%15s%s",buf,row[6],"\t");
                sprintf(buf,"%s%s%s",buf,row[2],"\n");
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int cd(char *precode,char *filename,char *belongID,char *code)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else{
                if(!strcmp(row[4],"f")){
                    mysql_free_result(res);
                    mysql_close(conn);
                    return 0;
                }
                else{
                    strcpy(code,row[1]);
                    return 1;
                }
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int puts_query(char *precode,char* filename,char* id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,id);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else{
                mysql_free_result(res);
                mysql_close(conn);
                return 1;
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int puts_insert(char* precode,char *filename,char *belongID,char *md5sum,off_t filesize)
{
    MYSQL *conn;
    //MYSQL_RES *res;
    // MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";
    char query[200]="insert into File(precode,filename,belongID,fileType,md5sum,filesize) values(";
    sprintf(query,"%s%s%s",query,precode,",'");
    sprintf(query,"%s%s%s",query,filename,"',");
    sprintf(query,"%s%s%s",query,belongID,",");
    sprintf(query,"%s%s",query,"'f',");
    sprintf(query,"%s%s%s%s%s",query,"'",md5sum,"'",",");
    sprintf(query,"%s%ld%s",query,filesize,")");
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));

    }else{
        //    printf("Connected...\n");

    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));

    }else{
        //  printf("insert success\n");
    }
    mysql_close(conn);
    return 0;
}

int gets_query(char *precode,char* filename,char* belongID,char* md5sum)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }
            else{
                if(!strcmp(row[4],"d")){
                    mysql_free_result(res);
                    mysql_close(conn);
                    return -1;
                }
                strcpy(md5sum,row[5]);
                mysql_free_result(res);
                mysql_close(conn);
                return 1;
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int remove_query(char *precode,char* filename,char* belongID,char* md5sum)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    printf("t=%d\n",t);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }else{
                printf("111\n");
                if(!strcmp(row[4],"d")){
                    printf("222\n");
                    bzero(query,300);
                    sprintf(query,"%s%s%s",query,"select * from File where precode=",row[1]);
                    puts(query);
                    mysql_free_result(res);
                    t=mysql_query(conn,query);
                    printf("t=%d\n",t);
                    if(t) printf("Error making query:%s\n",mysql_error(conn));
                    else{
                        res=mysql_use_result(conn);
                        if(res){
                            row=mysql_fetch_row(res);
                            if(!row){
                                mysql_free_result(res);
                                mysql_close(conn);
                                return 1;
                            }else{
                                mysql_free_result(res);
                                mysql_close(conn);
                                return 2;
                            }
                        }
                    }
                }else{
                    strcpy(md5sum,row[5]);
                    mysql_free_result(res);
                    mysql_close(conn);
                    return 3;
                }
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int remove_delete(char*precode,char*filename,char*belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";
    char query[200]="delete from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t,r;
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
        printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));

    }
    mysql_close(conn);
    return 0;
}

int remove_query1(char *md5sum)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where md5sum='";
    sprintf(query,"%s%s%s",query,md5sum,"'");
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                mysql_free_result(res);
                mysql_close(conn);
                return 4;
            }else{
                mysql_free_result(res);
                mysql_close(conn);
                return 3;
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int mdr(char *precode,char *filename,char* belongID)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="select * from File where precode=";
    sprintf(query,"%s%s%s",query,precode," and filename='");
    sprintf(query,"%s%s%s",query,filename,"' and belongID=");
    sprintf(query,"%s%s",query,belongID);
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //  printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
            if(!row){
                bzero(query,300);
                sprintf(query,"%s%s",query,"insert into File (precode,filename,belongID,fileType) values(");
                sprintf(query,"%s%s%s",query,precode,",'");
                sprintf(query,"%s%s%s",query,filename,"',");
                sprintf(query,"%s%s%s",query,belongID,",'d')");
                puts(query);
                t=mysql_query(conn,query);
                if(t){
                    printf("Error making query:%s\n",mysql_error(conn));
                }else printf("insert success\n");
                mysql_free_result(res);
                mysql_close(conn);
                return 0;
            }else{
                mysql_free_result(res);
                mysql_close(conn);
                return 1;
            }
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
    }
}

int User_log(char *id,char *cmd,char* text)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123456";
    char* database="project";//要访问的数据库名称
    char query[300]="insert into Userlog (UserID,command,parameter) values(";
    sprintf(query,"%s%s%s",query,id,",'");
    sprintf(query,"%s%s%s",query,cmd,"','");
    sprintf(query,"%s%s%s",query,text,"')");
    puts(query);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }else{
        //      printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        printf("insert success\n");
    }
    mysql_close(conn);
    return 0;
}

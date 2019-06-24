//#include "factory.h"
#include "tranfile.h"
#include "user_sql.h"
#include <time.h>
#include <unistd.h>
#define PATH "/home/zzh/project/server_file_pool"
#define STR_LEN 10//定义随机输出的字符串长度
char str[STR_LEN+1];

char *getSalt()
{
    bzero(str,STR_LEN+1);
    int i,flag;
    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for(i = 0; i < STR_LEN; i ++)
    {
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break;
        case 2:
            str[i] = rand()%10 + '0';
            break;  
        } 
    }
    //printf("%s\n", str);//输出生成的随机数。
    return str;
}


int interactive(int new_fd)
{
    Train train;
    int choice;
    int dataLen;
    int ret;
    char id[11]={0};
    char username[20]={0};
    char salt[20]={0};
    char ciphertext[20]={0};
    recvCycle(new_fd,&choice,4);
    //printf("choice=%d\n",choice);
    if(choice==1)   //用户进行注册
    {
Label1:
        recvCycle(new_fd,&dataLen,4);
        recvCycle(new_fd,username,dataLen);

        ret=query(username,salt,ciphertext,id);
        if(!ret)  //没有同名用户
        {
            strcpy(salt,getSalt());
            train.dataLen=strlen(salt);
            //printf("salt.dataLen=%d\n",train.dataLen);
            strcpy(train.buf,salt);
            ret=send(new_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");
            
            recvCycle(new_fd,&dataLen,4);
            recvCycle(new_fd,ciphertext,dataLen);
            //printf("ciphertext=%s\n",ciphertext);
            
            insert(username,salt,ciphertext);
            query(username,salt,ciphertext,id);
            printf("userid=%s\n",id);
        }
        else{
            train.dataLen=0;
            ret=send(new_fd,&train,4,0);
            ERROR_CHECK(ret,-1,"send");
            goto Label1;
        }
        User_log(id,"regist","");
    }

    char buf[20]={0};   //用来存储用户发来的密文
    if(choice==2)   //用户登录
    {
        recvCycle(new_fd,&dataLen,4);
        recvCycle(new_fd,username,dataLen);
        //printf("username=%s\n",username);
        ret=query(username,salt,ciphertext,id);
        //printf("userid=%s\n",id);
       // printf("salt=%s\n",salt);
       // printf("ciphertext=%s\n",ciphertext);
        if(!ret){   //用户不存在
            train.dataLen=0;
            ret=send(new_fd,&train,4,0);
            ERROR_CHECK(ret,-1,"send");
        }
        else{
            train.dataLen=strlen(salt);
            //printf("salt.dataLen=%d\n",train.dataLen);
            strcpy(train.buf,salt);
            ret=send(new_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");
Label2:            
            //char buf[20]={0};
            recvCycle(new_fd,&dataLen,4);
            recvCycle(new_fd,buf,dataLen);
            //printf("buf=%s\n",buf);

            if(!strcmp(ciphertext,buf)){
                train.dataLen=1;
                ret=send(new_fd,&train,4,0);
                ERROR_CHECK(ret,-1,"send");
            }else{
                train.dataLen=0;
                ret=send(new_fd,&train,4,0);
                ERROR_CHECK(ret,-1,"send");
                goto Label2;    //密码输入错误
            }
        }
        User_log(id,"login","");
    }

    int cmd;
    char text[100]={0}; //用来接命令之后的信息
    char fbuf[1000]={0};    //用来存储ls的信息,含格式控制符
    char code[12]={0};
    char ppcode[12]={0};    //ls 使用ppcode
    ppcode[0]='0';
    char precode[12]={0};   //cd 使用precode, 用户发来的都是绝对路径
    precode[0]='0';
    while(1)
    {
        bzero(text,100);
        bzero(fbuf,1000);
        recvCycle(new_fd,&dataLen,4);
        recvCycle(new_fd,&cmd,4);
        if(cmd==0){
            User_log(id,"exit","");
            break;
        }
        if(cmd==2){ //ls
            ls(ppcode,id,fbuf);
           // printf("%s\n",fbuf);
            train.dataLen=strlen(fbuf);
            strcpy(train.buf,fbuf);
            ret=send(new_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");
            User_log(id,"ls","");
        }
        if(cmd==3){ //cd
            bzero(precode,12);
            precode[0]='0';
            recvCycle(new_fd,text,dataLen);
           // printf("text=%s\n",text);
            if(dataLen==1){ //cd /
                bzero(ppcode,12);
                ppcode[0]='0';
                ret=1;  //cd 成功
            }
            else{
                char *p=text+1;
                char *q=text+1;
                for(;*p!='\0';q++)  //循环遍历各级目录
                {
                    if(*q=='/'||*q=='\0'){
                        *q='\0';
                        //printf("p=%s\n",p);
                        ret=cd(precode,p,id,code);
                        if(!ret) break; //cd 失败
                        strcpy(precode,code);
                        //printf("code=%s\n",code);
                        q++;
                        p=q;
                    }
                }
            }
            if(ret){    //cd 成功
                strcpy(ppcode,precode);
                ret=send(new_fd,&ret,4,0);
                ERROR_CHECK(ret,-1,"send");
                User_log(id,"cd",text);
            }else{  //cd 失败
                ret=send(new_fd,&ret,4,0);
                ERROR_CHECK(ret,-1,"send");
            }
        }
        if(cmd==4){
            //接文件名
            recvCycle(new_fd,text,dataLen);
            printf("filename=%s\n",text);
            ret=puts_query(ppcode,text,id);
            if(ret){    //有同名文件
                dataLen=1;
                ret=send(new_fd,&dataLen,4,0);
                ERROR_CHECK(ret,-1,"send");
                continue;
            }else{
                dataLen=0;
                ret=send(new_fd,&dataLen,4,0);
                ERROR_CHECK(ret,-1,"send");
            }

            off_t filesize;
            char md5sum[36]={0};
            //接文件大小
            recvCycle(new_fd,&dataLen,4);
            recvCycle(new_fd,&filesize,dataLen);
            //接文件MD5
            recvCycle(new_fd,&dataLen,4);
            recvCycle(new_fd,md5sum,dataLen);

            char file_path[100]={0};
            sprintf(file_path,"%s%s%s%s",file_path,PATH,"/",md5sum);
            printf("file_path=%s\n",file_path);

            int fd=open(file_path,O_CREAT|O_RDWR|O_APPEND,0666);
            ERROR_CHECK(fd,-1,"open");
            struct stat buf;
            fstat(fd,&buf);
            train.dataLen=sizeof(buf.st_size);
            memcpy(train.buf,&buf.st_size,train.dataLen);
            ret=send(new_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");

            while(1){
                ret=recvCycle(new_fd,&dataLen,4);
                if(-1==ret){
                    printf("客户端断开\n");
                    break;
                }
                if(dataLen>0){
                    ret=recvCycle(new_fd,fbuf,dataLen);
                    if(-1==ret){
                        printf("客户端断开\n");
                        break;
                    }
                    write(fd,fbuf,dataLen);
                }else break;
            }
            if(ret!=-1) puts_insert(ppcode,text,id,md5sum,filesize);
            close(fd);
            User_log(id,"puts",text);
        }
        if(cmd==5){
            char md5sum[36]={0};
            recvCycle(new_fd,text,dataLen);
            ret=gets_query(ppcode,text,id,md5sum);
            if(ret==0){ //不存在文件
                ret=send(new_fd,&ret,4,0);
                ERROR_CHECK(ret,-1,"send");
                continue;
            }else if(ret==-1){  //目录文件
                ret=send(new_fd,&ret,4,0);
                ERROR_CHECK(ret,-1,"send");
                continue;
            }else{
                ret=send(new_fd,&ret,4,0);
                ERROR_CHECK(ret,-1,"send");

                off_t filesize;
                char file_path[100]={0};
                sprintf(file_path,"%s%s%s%s",file_path,PATH,"/",md5sum);
                int fd=open(file_path,O_RDONLY);
                ERROR_CHECK(fd,-1,"open");
                struct stat buf;
                fstat(fd,&buf);

                //发送服务器文件大小
                train.dataLen=sizeof(buf.st_size);
                memcpy(train.buf,&buf.st_size,train.dataLen);
                ret=send(new_fd,&train,4+train.dataLen,0);
                ERROR_CHECK(ret,-1,"send");
                //接客户端文件大小
                recvCycle(new_fd,&dataLen,4);
                recvCycle(new_fd,&filesize,dataLen);
                
                off_t seek;
                off_t ptrPos=0;
                char *pMap=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
                ERROR_CHECK(pMap,(char*)-1,"mmap");
                char *p=pMap;
                p += filesize;
                seek=buf.st_size-filesize;
                //发文件
                if(seek>1000){
                    while(ptrPos+1000<seek){
                        memcpy(train.buf,p+ptrPos,1000);
                        train.dataLen=1000;
                        ptrPos += 1000;
                        ret=send(new_fd,&train,4+train.dataLen,0);
                        if(-1==ret) return -1;
                    }
                    train.dataLen=seek-ptrPos;
                }else train.dataLen=seek;
                //发最后一列火车
                memcpy(train.buf,p+ptrPos,train.dataLen);
                ret=send(new_fd,&train,4+train.dataLen,0);
                if(-1==ret) return -1;
                munmap(pMap,buf.st_size);
                //发送接收标记
                if(train.dataLen){
                    train.dataLen=0;
                    send(new_fd,&train,4,0);
                }
                close(fd);
                User_log(id,"gets",text);
            }
        }
        if(cmd==6){
            char md5sum[36]={0};
            recvCycle(new_fd,text,dataLen);
            dataLen=remove_query(ppcode,text,id,md5sum);
            if(dataLen==1 || dataLen==3){
                remove_delete(ppcode,text,id);
                User_log(id,"remove",text);
            }
            if(dataLen==3){
                //remove_delete(ppcode,text,id);
                dataLen=remove_query1(md5sum);
            }
            if(dataLen==4){
                char file_path[100]={0};
                sprintf(file_path,"%s%s%s%s",file_path,PATH,"/",md5sum);
                ret=unlink(file_path);
                ERROR_CHECK(ret,-1,"unlink");
            }
            ret=send(new_fd,&dataLen,4,0);
            ERROR_CHECK(ret,-1,"send");
        }
        if(cmd==7){
            recvCycle(new_fd,text,dataLen);
            dataLen=mdr(ppcode,text,id);
            ret=send(new_fd,&dataLen,4,0);
            ERROR_CHECK(ret,-1,"send");
            if(dataLen!=1) User_log(id,"mkdir",text);
        }
    }
    return 0;
}

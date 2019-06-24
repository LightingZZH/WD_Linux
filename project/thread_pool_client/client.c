#include "function.h"
#include "file_md5.h"
#define PATH "/home/zzh/project/client_file_pool"

void cdpre(char *buf)
{
    int i=0,j;
    for(j=1;buf[j]!='\0';j++)
    {
        if(buf[j]=='/') i=j;
    }
    if(i) buf[i]='\0';
    else buf[1]='\0';
}

int main(int argc,char *argv[])
{
	ARGS_CHECK(argc,3);
	int socket_fd;
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socket_fd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"connect");
	printf("connect success\n\n");

    system("clear");
    int choice;
    printf("欢迎使用私有协议文件管理系统\n");
    printf("--->1.注册\n");
    printf("--->2.登录\n");
    scanf("%d",&choice);
    send(socket_fd,&choice,4,0);
    Train train;
    int dataLen;
    char salt[20]={0};
    char *password;
    char ciphertext[20]={0};

    if(choice==1)
    {
        printf("\n请输入用户名：");
        scanf("%s",train.buf);
        train.dataLen=strlen(train.buf);
        ret=send(socket_fd,&train,4+train.dataLen,0);
        ERROR_CHECK(ret,-1,"send");

        while(1)
        {
            recvCycle(socket_fd,&dataLen,4);
//            printf("dataLen=%d\n",dataLen);
            if(dataLen)
            {
                recvCycle(socket_fd,salt,dataLen);
//                printf("salt=%s\n",salt);
        
                password=getpass("\n请输入密码：");
                strcpy(ciphertext,crypt(password,salt));

                train.dataLen=strlen(ciphertext);
                strcpy(train.buf,ciphertext);
                ret=send(socket_fd,&train,4+train.dataLen,0);
                ERROR_CHECK(ret,-1,"send");
                
                printf("\n注册成功\n");
                break;
            }
            else{ 
                printf("\n用户名已存在，请重新输入用户名：");
                scanf("%s",train.buf);
                train.dataLen=strlen(train.buf);
                ret=send(socket_fd,&train,4+train.dataLen,0);
                ERROR_CHECK(ret,-1,"send");
            }
        }    
    }

    if(choice==2)
    {
        printf("\n请输入用户名：");
        scanf("%s",train.buf);
        train.dataLen=strlen(train.buf);
        ret=send(socket_fd,&train,4+train.dataLen,0);
        ERROR_CHECK(ret,-1,"send");

        recvCycle(socket_fd,&dataLen,4);
        if(!dataLen){
            printf("\n无此用户名\n");
            close(socket_fd);
            return -1;
        }
        else{
            recvCycle(socket_fd,salt,dataLen);
  //          printf("salt=%s\n",salt);
            
            password=getpass("\n请输入密码：");
            strcpy(ciphertext,crypt(password,salt));

            train.dataLen=strlen(ciphertext);
            strcpy(train.buf,ciphertext);
            ret=send(socket_fd,&train,4+train.dataLen,0);
            ERROR_CHECK(ret,-1,"send");

            while(1)
            {
                recvCycle(socket_fd,&dataLen,4);
                if(!dataLen){
                    password=getpass("\n密码输入错误，请重新输入密码：");
                    strcpy(ciphertext,crypt(password,salt));
                    
                    train.dataLen=strlen(ciphertext);
                    strcpy(train.buf,ciphertext);
                    ret=send(socket_fd,&train,4+train.dataLen,0);
                    ERROR_CHECK(ret,-1,"send");    
                }else{
                    printf("\n用户登录成功\n");
                    break;
                }
            }
        }
    }
    
    printf("\n请使用以下命令进行操作\n");
    printf("---> pwd\n");
    printf("---> ls\n");
    printf("---> cd\n");
    printf("---> puts\n");
    printf("---> gets\n");
    printf("---> remove\n");
    printf("---> mkdir\n");
    printf("---> exit\n\n");
    
    char ch;
    char buf[1000];
    char pwd[100]={0};
    pwd[0]='/';
    char pwdTest[100]={0};
    pwdTest[0]='/';
    char cmd[10]={0};
    char text[100]={0};
    Msg msg;
    while(1)
    {
        bzero(buf,1000);
        printf("please input: ");
        scanf("%s",cmd);
        if(!strcmp(cmd,"exit")){
            msg.len=0;
            msg.ctl_code=0;
            ret=send(socket_fd,&msg,8,0);
            ERROR_CHECK(ret,-1,"send");
            break;
        }
        else if(!strcmp(cmd,"pwd")){
            msg.ctl_code=1;
            printf("%s\n",pwd);
            while((ch=getchar())!='\n'); //刷新输入缓存区
            continue;
        }            
        else if(!strcmp(cmd,"ls")){
            msg.len=0;
            msg.ctl_code=2;
            ret=send(socket_fd,&msg,8,0);
            ERROR_CHECK(ret,-1,"send");
            recvCycle(socket_fd,&dataLen,4);
        //    printf("dataLen=%d\n",dataLen);
            recvCycle(socket_fd,buf,dataLen);
            printf("%s",buf);
            while((ch=getchar())!='\n');
            continue;
        }
        else if(!strcmp(cmd,"cd")){
            msg.ctl_code=3;
        }
        else if(!strcmp(cmd,"puts")){
            msg.ctl_code=4;
        } 
        else if(!strcmp(cmd,"gets")){
            msg.ctl_code=5;
        }
        else if(!strcmp(cmd,"remove")){
            msg.ctl_code=6;
        }
        else if(!strcmp(cmd,"mkdir")){
            msg.ctl_code=7;
        }
        else{
            printf("命令输入错误\n");
            while((ch=getchar())!='\n');
            continue;
        }

        if(msg.ctl_code>2){
            bzero(text,100);
            scanf("%s",text);

            if(msg.ctl_code==3){  
                dataLen=strlen(text);
                if(text[dataLen-1]=='/' && dataLen!=1){
                    text[dataLen-1]='\0';
                    dataLen--;
                }
                if(text[0]=='/') strcpy(pwdTest,text);
                else if(!strcmp(text,"..")) cdpre(pwdTest);
                else{
                    if(!strcmp(pwdTest,"/")) sprintf(pwdTest,"%s%s",pwdTest,text);
                    else sprintf(pwdTest,"%s%s%s",pwdTest,"/",text);
                } 
                msg.len=strlen(pwdTest);
                strcpy(msg.token,pwdTest);
                ret=send(socket_fd,&msg,8+msg.len,0);   //发送命令
                ERROR_CHECK(ret,-1,"send");
                recvCycle(socket_fd,&dataLen,4);
                if(!dataLen){
                    strcpy(pwdTest,pwd);
                    printf("cd 失败\n");
                }else{
                    strcpy(pwd,pwdTest);
                    printf("cd 成功\n");
                }
                while((ch=getchar())!='\n');
                continue;
            }
            if(msg.ctl_code==4){
                char file_path[100]={0};
                char md5_str[MD5_STR_LEN+1];
                sprintf(file_path,"%s%s%s%s",file_path,PATH,"/",text);
                printf("file_path=%s\n",file_path);
                int fd=open(file_path,O_RDONLY);
                if(fd==-1){
                    printf("文件不存在\n");
                    continue;
                }
                struct stat buf;
                fstat(fd,&buf);
                printf("filesize=%ld\n",buf.st_size);
                Compute_file_md5(file_path,md5_str);
                printf("%s\n",md5_str);
                //传文件名
                msg.len=strlen(text);
                strcpy(msg.token,text);
                ret=send(socket_fd,&msg,8+msg.len,0);
                ERROR_CHECK(ret,-1,"send");

                recvCycle(socket_fd,&dataLen,4);
                if(dataLen==1){
                    printf("当前目录下文件名重复\n");
                    continue;
                }else{
                    //传文件大小
                    train.dataLen=sizeof(buf.st_size);
                    memcpy(train.buf,&buf.st_size,sizeof(buf.st_size));
                    ret=send(socket_fd,&train,4+train.dataLen,0);
                    ERROR_CHECK(ret,-1,"send");
                    //传文件md5
                    train.dataLen=strlen(md5_str);
                    strcpy(train.buf,md5_str);
                    ret=send(socket_fd,&train,4+train.dataLen,0);
                    ERROR_CHECK(ret,-1,"send");
                    //接服务器端文件的大小
                    off_t filesize;
                    off_t sliceSize;
                    off_t seek;
                    off_t upLoadSize;
                    off_t oldSize=0;
                    off_t ptrPos=0;
                    recvCycle(socket_fd,&dataLen,4);
                    recvCycle(socket_fd,&filesize,dataLen);
                    char *pMap=mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
                    ERROR_CHECK(pMap,(char*)-1,"mmap");
                    char *p=pMap;
                    p += filesize;
                    seek=buf.st_size-filesize;
                    sliceSize=buf.st_size/10000;
                    upLoadSize=filesize;
                    if(seek>1000){
                        while(ptrPos+1000<seek){
                            memcpy(train.buf,p+ptrPos,1000);
                            train.dataLen=1000;
                            ptrPos += 1000;
                            ret=send(socket_fd,&train,4+train.dataLen,0);
                            if(-1==ret) return -1;
                            upLoadSize += 1000;
                            if(upLoadSize-oldSize>sliceSize){
                                printf("\r%5.2f%%",(float)upLoadSize/buf.st_size*100);
                                fflush(stdout);
                                oldSize=upLoadSize;
                            }
                        }
                        train.dataLen=seek-ptrPos;
                    }else train.dataLen=seek;
                    //发最后一列火车
                    memcpy(train.buf,p+ptrPos,train.dataLen);
                    ret=send(socket_fd,&train,4+train.dataLen,0);
                    if(-1==ret) return -1;
                    printf("\r100.00%%\n");
                    munmap(pMap,buf.st_size);
                    if(train.dataLen){  //发文件结束标记
                        train.dataLen=0;
                        send(socket_fd,&train,4,0);
                    }
                    close(fd);
                    printf("上传成功\n");
                }
            }//puts
            if(msg.ctl_code==5){    //gets
                msg.len=strlen(text);
                strcpy(msg.token,text);
                ret=send(socket_fd,&msg,8+msg.len,0);
                ERROR_CHECK(ret,-1,"send");
                //接收标记
                recvCycle(socket_fd,&dataLen,4);
                if(dataLen==0){
                    printf("当前目录下不存在该文件，无法下载\n");
                    continue;
                }else if(dataLen==-1){
                    printf("该文件为目录文件，无法下载\n");
                    continue;
                }else{
                    //接服务器文件大小
                    off_t filesize,sliceSize,oldSize=0,downLoadSize;
                    recvCycle(socket_fd,&dataLen,4);
                    recvCycle(socket_fd,&filesize,dataLen);
        
                    char file_path[100]={0};
                    sprintf(file_path,"%s%s%s%s",file_path,PATH,"/",text);
                    int fd=open(file_path,O_CREAT|O_RDWR|O_APPEND,0666);
                    ERROR_CHECK(fd,-1,"open");
                    struct stat sbuf;
                    fstat(fd,&sbuf);
                    //发送本地文件大小
                    train.dataLen=sizeof(sbuf.st_size);
                    memcpy(train.buf,&sbuf.st_size,train.dataLen);
                    ret=send(socket_fd,&train,4+train.dataLen,0);
                    ERROR_CHECK(ret,-1,"send");

                    sliceSize=filesize/10000;
                    downLoadSize=sbuf.st_size;
                    //接文件
                    while(1){
                        ret=recvCycle(socket_fd,&dataLen,4);
                        if(-1==ret){
                            printf("服务器断开\n");
                            break;
                        }
                        if(dataLen>0){
                            ret=recvCycle(socket_fd,buf,dataLen);
                            if(-1==ret){
                                printf("服务器断开\n");
                                break;
                            }
                            write(fd,buf,dataLen);
                            downLoadSize += dataLen;
                            if(downLoadSize-oldSize>sliceSize){
                                printf("\r%5.2f%%",(float)downLoadSize/filesize*100);
                                oldSize=downLoadSize;
                            }
                        }else{
                            printf("\r100.00%%\n");
                            break;
                        }
                    }
                    printf("下载成功\n");
                    close(fd);
                }
            }//gets
            if(msg.ctl_code==6){
                msg.len=strlen(text);
                strcpy(msg.token,text);
                ret=send(socket_fd,&msg,8+msg.len,0);
                ERROR_CHECK(ret,-1,"send");

                recvCycle(socket_fd,&dataLen,4);

                if(dataLen==0){
                    printf("当前目录下不存在该文件，无法删除\n");
                }else if(dataLen==2){
                    printf("该文件为非空目录文件，无法删除\n");
                }else{
                    printf("删除成功\n");
                }
            }//remove
            if(msg.ctl_code==7){
                msg.len=strlen(text);
                strcpy(msg.token,text);
                ret=send(socket_fd,&msg,8+msg.len,0);
                ERROR_CHECK(ret,-1,"send");

                recvCycle(socket_fd,&dataLen,4);

                if(dataLen==1){
                    printf("当前目录下存在同名文件\n");
                }else{
                    printf("新建文件夹成功\n");
                }
            }//mkdir
        }//msg.cmd>2
    }//while
    close(socket_fd);
    return 0;
}

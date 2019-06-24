#include <func.h>
#define N 10
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
    ret=bind(socket_fd,(struct sockaddr*)&ser,sizeof(ser));
    ERROR_CHECK(ret,-1,"bind");

    listen(socket_fd,N);

    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    int addrlen;
    
    char buf[128]={0};

    int epfd=epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    struct epoll_event event,evs[N];
    event.events=EPOLLIN;
    event.data.fd=socket_fd;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,socket_fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");

    int i,j,k;
    int newFd[N]={0};
    int new_fd;
    int readyFdNum;

    while(1)
    {
        memset(evs,0,sizeof(evs));
        readyFdNum=epoll_wait(epfd,evs,N+1,-1);
        for(i=0;i<readyFdNum;i++)
        {
            if(evs[i].data.fd==socket_fd)
            {
                addrlen=sizeof(client);
                new_fd=accept(socket_fd,(struct sockaddr*)&client,&addrlen);
                ERROR_CHECK(new_fd,-1,"accept");
                printf("client ip=%s, port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                for(j=0;j<N;j++)
                {
                    if(newFd[j]==0)
                    {
                        newFd[j]=new_fd;
                        break;
                    }
                }
                event.data.fd=newFd[j];
                ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);
                ERROR_CHECK(ret,-1,"epoll_ctl");
            }
            for(j=0;j<N;j++)
            {
                if(evs[i].data.fd==newFd[j])
                {
                    bzero(buf,sizeof(buf));
                    ret=recv(newFd[j],buf,sizeof(buf),0);
                    ERROR_CHECK(ret,-1,"recv");
                    if(0==ret)
                    {
                        printf("bye bye!\n");
                        event.events=EPOLLIN;
                        event.data.fd=newFd[j];
                        ret=epoll_ctl(epfd,EPOLL_CTL_DEL,newFd[j],&event);
                        ERROR_CHECK(ret,-1,"epoll_ctl");
                        newFd[j]=0;
                        break;
                    }
                    for(k=0;k<N;k++)
                    {
                        if(newFd[k]!=0 && newFd[k]!=newFd[j])
                        {
                            ret=send(newFd[k],buf,strlen(buf),0);
                            ERROR_CHECK(ret,-1,"send");
                        }
                    }
                }
            }
        }
    }
    return 0;
}


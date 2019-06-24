#include "function.h"
#define N 3
#define SWAP(x,y) x=(y+x)-(y=x)
#define RAND() (srand((unsigned int)time(NULL)),rand())

void Sort(int x[],int min,int max)
{
    for(int i=min;i<=max-1;i++)
    {
        for(int j=i+1;j<=max;j++)
        {
            if(x[j]>x[i]) SWAP(x[i],x[j]);
        }
    }
}

void Reshuffle()
{
    memset(m_iPoker,-1,54);
    for(int iCard=0;iCard<54;iCard++)
    {
        int iPos=RAND()%54;
        while(m_iPoker[iPos]!=-1)
        {
            iPos++;
            if(iPos>=54) iPos=0;
        }
        m_iPoker[iPos]=iCard;
    }

    Sort(m_iPoker,0,16);
    Sort(m_iPoker,17,33);
    Sort(m_iPoker,34,50);
    Sort(m_iPoker,51,53);
}

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,3);
    int socket_fd;
    int ret;

    tcpInit(&socket_fd,argv[1],argv[2]);

    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    int addrlen;
    
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
    int ready_cnt=0;

    Train train;
    bzero(&train,sizeof(train));

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
                    bzero(&train,sizeof(train));
                    ret=recv(newFd[j],&train,sizeof(train),0);
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
                    if(train.control_flag==1)
                    {
                        ready_cnt++;
                    }
                    if(ready_cnt==3)
                    {
                        Reshuffle();
                        for(k=0;k<N;k++)
                        {
                            train.dataLen=17*4;
                            train.control_flag=1;
                            memcpy(train.buf,&m_iPoker[k*17],train.dataLen);
                            ret=send(newFd[k],&train,8+train.dataLen,0);
                            ERROR_CHECK(ret,-1,"send");
                        }
                    }
                    //for(k=0;k<N;k++)
                    //{
                    //    if(newFd[k]!=0 && newFd[k]!=newFd[j])
                    //    {
                    //        ret=send(newFd[k],buf,strlen(buf),0);
                    //        ERROR_CHECK(ret,-1,"send");
                    //    }
                    //}
                }
            }
        }
    }
    return 0;
}


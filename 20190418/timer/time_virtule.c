#include <func.h>
void sigFunc(int signum)
{
	time_t now;
	now=time(NULL);
	printf("%s\n",ctime(&now));
}

int main()
{
	signal(SIGVTALRM,sigFunc);
	struct itimerval t;
	bzero(&t,sizeof(t));
	t.it_value.tv_sec=3;	//初始时间间隔
	t.it_interval.tv_sec=2;	//重复时间间隔
	int ret;
	sigFunc(0);
	ret=setitimer(ITIMER_VIRTUAL,&t,NULL);//设置计时器
	ERROR_CHECK(ret,-1,"setitimer");
	sleep(5);
	while(1);
}

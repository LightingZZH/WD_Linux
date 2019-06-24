#include <func.h>
void clean(void *p)
{
	printf("I am clean\n");
	free(p);
}

void* threadFunc(void *p)
{
	p=malloc(20);
	pthread_cleanup_push(clean,p);
	read(STDIN_FILENO,p,20);
	pthread_exit((void*)2);
	pthread_cleanup_pop(0);
}

int main()
{
	pthread_t pthID;
	int ret;
	int threadRet=0;
	ret=pthread_create(&pthID,NULL,threadFunc,NULL);
	THREAD_ERROR_CHECK(ret,"pthread_create");
//	sleep(3);
	ret=pthread_cancel(pthID);
	THREAD_ERROR_CHECK(ret,"pthrad_cancel");
	ret=pthread_join(pthID,(void**)&threadRet);
	THREAD_ERROR_CHECK(ret,"pthread_join");
	printf("child thread's return: %d\n",threadRet);
}

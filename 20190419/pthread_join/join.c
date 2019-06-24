#include <func.h>
void* threadFunc(void *p)
{
	int arg=(int)p;
	printf("the arg of pthread_join is %d\n",arg);
	pthread_exit((void*)2);
}

int main()
{
	pthread_t pthID;
	int threadRet=0;
	int ret;
	ret=pthread_create(&pthID,NULL,threadFunc,(void*)1);
	THREAD_ERROR_CHECK(ret,"pthread_create");
	pthread_join(pthID,(void**)&threadRet);
	printf("I am main thread\n");
	printf("child thread's return: %d\n",threadRet);
	return 0;
}

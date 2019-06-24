#include <func.h>
void* threadFunc(void *p)
{
	printf("I am child thread\n");
	return NULL;
}

int main()
{
	pthread_t pthID;
	int ret=pthread_create(&pthID,NULL,threadFunc,NULL);
	THREAD_ERROR_CHECK(ret,"pthread_creat");
	printf("I am main thread\n");
	ret=pthread_join(pthID,NULL);
	THREAD_ERROR_CHECK(ret,"pthread_join");
	return 0;
}

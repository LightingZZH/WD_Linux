#include <func.h>
#define N 20000000

typedef struct{
	int num;
//	int a;
//	int b;
	pthread_mutex_t mutex;
}Data;

void* threadFunc(void *p)
{
	Data *pThreadInfo=(Data*)p;
	for(int i=0;i<N;i++){
		pthread_mutex_lock(&pThreadInfo->mutex);
		pThreadInfo->num += 1;
	//	pThreadInfo->b += 1;
		pthread_mutex_unlock(&pThreadInfo->mutex);
	}
	//printf("child thread's ticket: %d\n",pThreadInfo->b);
	return NULL;
}

int main()
{
	pthread_t pthID;
	int ret;
	Data threadInfo;
	threadInfo.num=0;
//	threadInfo.a=0;
//	threadInfo.b=0;
	ret=pthread_mutex_init(&threadInfo.mutex,NULL);//初始化锁
	THREAD_ERROR_CHECK(ret,"pthread_mutex_init");
	ret=pthread_create(&pthID,NULL,threadFunc,&threadInfo);
	THREAD_ERROR_CHECK(ret,"pthread_create");
	for(int i=0;i<N;i++){
		pthread_mutex_lock(&threadInfo.mutex);
		threadInfo.num += 1;
//		threadInfo.a += 1;
		pthread_mutex_unlock(&threadInfo.mutex);
	}
//	printf("main thread's ticket: %d\n",threadInfo.a);
	pthread_join(pthID,NULL);
	printf("the sum : %d\n",threadInfo.num);
	ret=pthread_mutex_destroy(&threadInfo.mutex);//销毁锁
	THREAD_ERROR_CHECK(ret,"pthread_mutex_destory");
	return 0;
}

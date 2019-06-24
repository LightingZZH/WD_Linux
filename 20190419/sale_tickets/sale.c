#include <func.h>
#define N 3

typedef struct{
	int amount;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}Ticket;

void* saleWindows1(void *p)
{
	Ticket *pT=(Ticket*)p;
	int count=0;
	while(1){
		pthread_mutex_lock(&pT->mutex);
		if(pT->amount>0){
			printf("saleWindows1,tickets is %d\n",pT->amount);
			pT->amount--;
			count++;
			printf("saleWindows1 is saling\n");
			printf("saleWindows1,tickets is %d\n",pT->amount);
			if(pT->amount==0){
				pthread_cond_signal(&pT->cond);
			}
			pthread_mutex_unlock(&pT->mutex);
		}else{
			pthread_mutex_unlock(&pT->mutex);
			printf("saleWindows1's sale_amount: %d\n",count);
			break;
		}
		sleep(1);
	}
	return NULL;
}

void* saleWindows2(void *p)
{
	
	Ticket *pT=(Ticket*)p;
	int count=0;
	while(1){
		pthread_mutex_lock(&pT->mutex);
		if(pT->amount>0){
			printf("saleWindows2,tickets is %d\n",pT->amount);
			pT->amount--;
			count++;
			printf("saleWindows2 is saling\n");
			printf("saleWindows2,tickets is %d\n",pT->amount);
			if(pT->amount==0){
				pthread_cond_signal(&pT->cond);
			}
			pthread_mutex_unlock(&pT->mutex);
		}else{
			pthread_mutex_unlock(&pT->mutex);
			printf("saleWindows2's sale_amount: %d\n",count);
			break;
		}
		sleep(1);
	}
	return NULL;
}

void* setTickets(void *p)
{
	Ticket *pT=(Ticket*)p;
	pthread_mutex_lock(&pT->mutex);
	if(pT->amount>0){
		pthread_cond_wait(&pT->cond,&pT->mutex);
	}
	pT->amount=20;
	pthread_mutex_unlock(&pT->mutex);
	return NULL;
}

typedef void* (*threadFunc) (void*);	//函数指针

int main()
{
	Ticket t;
	t.amount=20;
	pthread_t pthID[N];
	threadFunc pthreadFunc[N]={saleWindows1,saleWindows2,setTickets};//函数指针数组
	pthread_mutex_init(&t.mutex,NULL);
	pthread_cond_init(&t.cond,NULL);
	for(int i=0;i<N;i++){
		pthread_create(pthID+i,NULL,pthreadFunc[i],&t);
	}
	for(int i=0;i<N;i++){
		pthread_join(pthID[i],NULL);
	}
	printf("sale over\n");
	return 0;
}

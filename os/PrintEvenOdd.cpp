#include <iostream>
#include <pthread.h>
#include <semaphore.h>
     
#define MAX 100
pthread_mutex_t mtx;
pthread_cond_t cond1;
sem_t s1;
sem_t s2;
     
volatile bool isOdd = false;
     
void *PrintEven(void *args)
{
	int i=0;
	while(i<MAX) {
		pthread_mutex_lock(&mtx);
		while(isOdd) {
			pthread_cond_wait(&cond1, &mtx);
		}
		std::cout<<i<<std::endl;
		isOdd = true;
		pthread_mutex_unlock(&mtx);
		pthread_cond_signal(&cond1);
		i+=2;
	}
	return NULL;
}
     
void *PrintOdd(void *args)
{
	int i=1;
	while(i<MAX) {
		pthread_mutex_lock(&mtx);
		while(!isOdd) {
			pthread_cond_wait(&cond1, &mtx);
		};
		std::cout<<i<<std::endl;
		isOdd = false;
		pthread_mutex_unlock(&mtx);
		pthread_cond_signal(&cond1);
		i+=2;
	}
	return NULL;
}
     
     
     
void *PrintEvenSem(void *args)
{
	int i=0;
	while(i<MAX) {
		sem_wait(&s1);
		std::cout<<i<<std::endl;
		sem_post(&s2);
		i+=2;
	}
}
     
void *PrintOddSem(void *args)
{
	int i=1;
	while(i<MAX) {
		sem_wait(&s2);
		std::cout<<i<<std::endl;
		sem_post(&s1);
		i+=2;
	}
}
     
int main()
{
	pthread_t t1, t2;
	pthread_t prodSem;
	pthread_t consSem;

	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond1, NULL);


	sem_init(&s1, 0, 1);
	sem_init(&s2, 0, 0);



	//use semaphore
	pthread_create(&consSem, NULL, PrintEvenSem, NULL);
	pthread_create(&prodSem, NULL, PrintOddSem, NULL);
	pthread_join(prodSem, NULL);
	pthread_join(consSem, NULL);

	//mutex and cond
	pthread_create(&t1, NULL, &PrintOdd, NULL);
	pthread_create(&t2, NULL, &PrintEven, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_cond_destroy(&cond1);
	pthread_mutex_destroy(&mtx);

	pthread_exit(NULL);
	return 0;
}

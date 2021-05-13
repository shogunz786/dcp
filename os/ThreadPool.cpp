#include<queue>
#include <iostream>
#include <pthread.h>
using namespace std;
#define MAX_THREADS 3
class Work{
	public:
		int type;
		int data;
		virtual void *run()=0;
		Work(int d, int t){data=d; type=t;};
};

class WorkA: public Work
{
	public:
		WorkA(int d, int t):Work(d,t){};
		void* run(){ cout<<"I am A"<<endl; }
};
class WorkB: public Work
{
	public:
		WorkB(int d, int t):Work(d,t){};
		void* run(){ cout<<"I am B"<<endl; }
};
queue<Work*> tasks;
pthread_t threads[MAX_THREADS];
pthread_cond_t cond;
pthread_mutex_t mutex;

/* function headers */
void * worker(void *arg);
void submit_work(Work *w);

/**
 * Should execute the submited work tasks through thread pool.
 */
int main(void) {

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    Work *wa = new WorkA(1, 2);
    submit_work(wa);//add work from a different thread
    Work *wb = new WorkB(1, 2);
    submit_work(wb);//add work from a different thread

    /* start all threads */    
    for (int i = 0; i < MAX_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    /* wait all threads to finish */
    for (int i = 0; i < MAX_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

/**
 * Adds a calculation task to queue.
 */
void submit_work(Work *w) {
    /* lock the queue to avoid thread access */
    pthread_mutex_lock(&mutex);
    /* add work task to work queue */
    tasks.push(w);
    pthread_mutex_unlock(&mutex);
    /* signal a thread that it should check for new work */
    pthread_cond_signal(&cond);
}

void * worker(void *arg) { 
    int result;
    Work * theWork;
    for (;;) {
        pthread_mutex_lock(&mutex);
        while (tasks.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        theWork = tasks.front();
	tasks.pop();
        pthread_mutex_unlock(&mutex);
        
        theWork->run();
        delete theWork;
    }
    pthread_exit(NULL);
}

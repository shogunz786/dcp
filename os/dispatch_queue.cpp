#include <pthread.h>
#include <functional>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <string>
#define FLAGS_NO_LOCK 0
#define FLAGS_FILESYSTEM_LOCK 1
#define FLAGS_DATABASE_LOCK 2
#define FLAGS_MODEL_LOCK 4

class BaseTask{
	private:
		int flags;
	public:
		BaseTask(int f){ flags=f;}
		int getFlags(){ return flags;}
		virtual void *run()=0;
};

class dispatch_queue {

public:
	dispatch_queue(std::string name, size_t thread_cnt = 1);
	~dispatch_queue();

	// dispatch and copy
	void dispatch(const BaseTask* op);

private:
	std::string name_;
	pthread_mutex_t lock_;
	vector<pthread_t> threads_;
	queue<BaseTask*> q_;
	pthread_cond_t cv_;
	bool quit_ = false;
	void dispatch_thread_handler(void);
	pthread_mutex_t dblock;
	pthread_mutex_t fslock;
	pthread_mutex_t mlock;
};

dispatch_queue::dispatch_queue(std::string name, size_t thread_cnt) :
	name_(name), threads(thread_cnt, 0)
{
	cv_ = PTHREAD_COND_INITIALIZER;
	lock_ = PTHREAD_MUTEX_INITIALIZER;
	dblock = PTHREAD_MUTEX_INITIALIZER;
	fslock = PTHREAD_MUTEX_INITIALIZER;
	mlock = PTHREAD_MUTEX_INITIALIZER;
	for(size_t i = 0; i < threads_.size(); i++)
		pthread_create(&threads_[i], NULL, dispatch_queue::dispatch_thread_handler, NULL);
}

dispatch_queue::~dispatch_queue()
{
	// Signal to dispatch threads that it's time to wrap up
	quit_ = true;
	pthread_cond_broadcast(&cv_);

	// Wait for threads to finish before we exit
	for(size_t i = 0; i < threads_.size(); i++)
		pthread_join(threads_[i], NULL);
}

void dispatch_queue::dispatch(const BaseTask* op)
{
	pthread_mutex_lock(&lock_);
	q_.push(op);

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	pthread_mutex_unlock(&lock_);
	pthread_cond_broadcast(&cv_);
}

void dispatch_queue::dispatch_thread_handler(void)
{
	pthread_mutex_lock(&lock_);
	do {
		//Wait until we have data or a quit signal
		while (q_.empty() && !quit_) {
			pthread_cond_wait(&cv_, &lock_);
		}

		//after wait, we own the lock
		if(q_.size() && !quit_)
		{
			BaseTask *op = q_.front();
			q_.pop();

			//unlock now that we're done messing with the queue
			pthread_mutex_unlock(&lock_);

			if(op->getFlags() & FLAGS_DATABASE_LOCK)
				pthread_mutex_lock(&dblock);
			if(op->getFlags() & FLAGS_FILESYSTEM_LOCK)
				pthread_mutex_lock(&fslock);
			if(op->getFlags() & FLAGS_MODEL_LOCK)
				pthread_mutex_lock(&mlock);
			op->run();
			if(op->getFlags() & FLAGS_MODEL_LOCK)
				pthread_mutex_unlock(&mlock);
			if(op->getFlags() & FLAGS_FILESYSTEM_LOCK)
				pthread_mutex_unlock(&fslock);
			if(op->getFlags() & FLAGS_DATABASE_LOCK)
				pthread_mutex_unlock(&dblock);

			pthread_mutex_lock(&lock_);

		}
	} while (!quit_);
	pthread_mutex_unlock(&lock_);
}

class pmutex {
	std::size_t next_;
	std::size_t curr_;
	pthread_cond_t cv_;
	pthread_mutex_t mtx_;

	public:
	pmutex() 
	{
		next_=0;
	       	curr_=0; 
		mtx_ = PTHREAD_MUTEX_INITIALIZER;
		cv_ = PTHREAD_COND_INITIALIZER;
	}
	~pmutex(){
		pthread_mutex_destroy(&mtx_);
		pthread_cond_destroy(&cv_);
	}

	void lock()
	{
		int old_state = PTHREAD_CANCEL_ENABLE;
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);
		pthread_mutex_lock(&mtx_);
		const std::size_t request = next_++;
		while (request != curr_) {
			pthread_cond_wait(&cv_, &mtx_);
		}
		pthread_mutex_unlock(&mtx_);
		pthread_setcancelstate(old_state, NULL);
	}

	bool try_lock()
	{
		bool ret = true;
		int old_state = PTHREAD_CANCEL_ENABLE;
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);
		pthread_mutex_lock(&mtx_);
		if (next_ != curr_)
			ret = false;
		else
		{
			++next_;
			ret = true;
		}
		pthread_mutex_unlock(&mtx_);
		pthread_setcancelstate(old_state, NULL);
		return ret;
	}

	void unlock()
	{
		int old_state = PTHREAD_CANCEL_ENABLE;
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);
		pthread_mutex_lock(&mtx_);
		++curr_;
		pthread_cond_broadcast(&cv_);
		pthread_mutex_unlock(&mtx_);
		pthread_setcancelstate(old_state, NULL);
	}
};


#ifndef   FIFO_MUTEX_H
#define   FIFO_MUTEX_H
#include <pthread.h>
#include <errno.h>

/* First in, first out mutex type and associated inline functions.
 * Note that the type is not async signal safe, and therefore should
 * not be used in signal handlers; this is due to condition variables
 * used in the structure internally.
*/

typedef struct fifo_mutex	fifo_mutex_t;
struct fifo_mutex {
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	unsigned int		worker;
	unsigned int		waiter;
	int 			old_state;
};

#define FIFO_MUTEX_INITIALIZER { PTHREAD_MUTEX_INITIALIZER, \
                                 PTHREAD_COND_INITIALIZER, \
                                 1U, 0U, PTHREAD_CANCEL_ENABLE }

static inline int fifo_mutex_init(fifo_mutex_t *const fifo,
                                   pthread_mutexattr_t *const mutexattr,
                                   pthread_condattr_t *const condattr)
{
	if (fifo) {

		pthread_mutex_init(&(fifo->mutex), mutexattr);
		pthread_cond_init(&(fifo->cond), condattr);

		fifo->worker = 1U;
		fifo->waiter = 0U;
		fifo->old_state = PTHREAD_CANCEL_ENABLE;

		return 0;
	}
	return EINVAL;
}

static inline int fifo_mutex_unlock(fifo_mutex_t *const fifo)
{
	int result;

	if (fifo) {
		fifo->worker++;

		result = pthread_cond_broadcast(&(fifo->cond));
		if (result){
			pthread_setcancelstate(fifo->old_state, NULL);
			return result;
		}

		result = pthread_mutex_unlock(&(fifo->mutex));
		if (result){
			pthread_setcancelstate(fifo->old_state, NULL);
			return result;
		}

		pthread_setcancelstate(fifo->old_state, NULL);
		return 0;
	}
	return EINVAL;
}

static inline int fifo_mutex_lock(fifo_mutex_t *const fifo)
{
	int result, waiter;

	if (fifo) {

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &fifo->old_state);
                /* Atomic preincrement */
                waiter = __sync_add_and_fetch((int *)&(fifo->waiter), (int)1);

                /* Obtain the mutex */
		result = pthread_mutex_lock(&(fifo->mutex));
		if (result)
			return result;

                /* This thread? */
		if (waiter == fifo->worker)
			return 0;

		while (waiter != fifo->worker)
			pthread_cond_wait(&(fifo->cond), &(fifo->mutex));

		return 0;
	}
	return EINVAL;
}

#endif /* FIFO_MUTEX_H */

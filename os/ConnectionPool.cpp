/* Create object cache, such as a database connection pool */
template <class T>
class ConnectionPool {
	int maxObjects;
	int allocObjects;
	list<T> freeObjects;
	pthread_mutex_t lock;
	pthread_cond_t cvNoObjects;

	ConnectionPool(int max) : maxObjects(max) {
		pthread_mutex_init(&lock, NULL);
		pthread_cond_init(&cvNoObjects, NULL);
	}

	T* get() {
		T* val = NULL;

		pthread_mutex_lock(&lock);

		// wait until there is an object available if we are maxed out.
		while (allocObjects == maxObjects) {
			pthread_cond_wait(&cvNoObjects, &lock);
		}

		// get the first free object from the list
		if (freeObjects.size()) {
			val = freeObjects.front();
			freeObjects.pop_front();
			allocObjects++;
		}

		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cvNoObjects);

		// if we do not have an object at this point it means that we 
		// should allocate an object and return it.
		// NOTE: we allocate outside the lock since there is no reason
		// to hold it during allocation.
		if (val == NULL) {
			val = new T();
			allocObjects++;
		}

		return val;
	}

	void free(T* val) 
	{
		pthread_mutex_lock(&lock);
		freeObjects.add(val);
		allocObjects--;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cvNoObjects);
	}
}

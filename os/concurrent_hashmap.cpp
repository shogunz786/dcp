template<typename K, typename V>
class ConcurrentMap{
	private:
		pthread_mutex_t m[4];
		map<K,V*> my_map[1001];
		// Let' assume this hash function is a really good one :)
		size_t my_hash(K val) {
			return (unsigned)val % 1001;
		}
		// Let's assume that this is an internal API
		V* hash_search_impl(K key, unsigned h) {
			V* val = NULL;
			if(my_map[h].find(key)!=my_map[h].end())
				val = my_map[h][key];
			return val;	
		}
	public:
                ConcurrentMap(){ //init mutex 
		}


		V* hash_get(K key) {
			int h = my_hash(key);
			int mi = min(h / 250, (unsigned)3);
			V* val = NULL;
			pthread_mutex_lock(&m[h]);
			val=hash_search_impl(key, h);
			pthread_mutex_unlock(&m[h]);
			return val;
		}

		bool hash_put(K key, const V& val) {
			int h = my_hash(key);
			int mi = min(h / 250, (unsigned)3);
			V* val = NULL;
			bool ret = false;
			pthread_mutex_lock(&m[h]);
			val=hash_search_impl(key, h);
			if(!val){
				my_map[h].insert(make_pair(key, val));
				ret = true;
			}
			pthread_mutex_unlock(&m[h]);
			return ret;
		}
};

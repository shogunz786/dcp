struct node {
  string key;
  int value;
  node(int k, int v):key(k),value(v){}
};

class lru{
  int max_cache_size;//maximum size of cache
  list<node> nlist;
  unordered_map<int, list<node>::iterator> mp;//map of key to node with value
 public:
  void put(int key,int value);//set key-value
  int get(int key);//get value
  lru(int size):max_cache_size(size){};//init
};

void lru::get(int key){
  //if key not present return -1
  unordered_map<key,list<node>::iterator>::itertor iter = mp.find(key);
  if(iter == mp.end())
      return -1;
  else{
    //found promote the node higher up the list and return value
    mp[key]=promote(iter->second);
    return mp[key]->value;
  }
}

list<node>::iterator lru::promote(list<node>::iterator iter){
    list<node>::iterator niter = nlist.insert(nlist.begin(),(*iter));//insert at beginning
    nlist.erase(iter);//erase from old location
    return niter;
}

void lru::set(int key, int value)
{
   unordered_map<int, list<node>::iterator>::itertor iter = mp.find(key);
   //if found update
   if(iter != mp.end()){
      mp[key]=promote(iter->second);
      mp[key]->value = value;
   }else{//not found
       if(mp.size()>=max_cache_size){//check if reached cache size
          mp.erase(nlist.back().key);
          nlist.pop_back();//remove from back
       }
       list<node>::iterator iter = nlist.insert(nlist.begin(),node(key,value));
       mp.insert(make_pair(key,iter));
   }
}



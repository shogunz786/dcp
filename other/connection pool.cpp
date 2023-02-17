#include <iostream>
#include <unordered_map>
using namespace std;

class Connection {
  public:
  
  // Must be called once before any read/write calls.
  // This is an expensive operation.
  void open() {};

  string read() {
    return "";
  };

  void write(string data) {};

  // After close is called, read/write may not be called.
  // This is an expensive operation.
  void close() {};
  
  int connectionID() {
    return 0;
  };
};

// Implement a ConnectionPool that reuses connection to minimize
// the number of open and close calls.
//
// 1. The pool should be empty at start-up.
// 
// 2. When a request comes in to get a connection, the pool must return
// a connection from itself if one exists. If the pool is empty, it must
// dynamically create a connection and return that.
//
// 3. The pool must be thread-safe.

class ConnectionPool {
  public:
     unordered_map<string, Connection*> available;
     unordered_map<string, Connection*> inuse;
     mutex mtx;

     Connection* getConnection(){
           mtx.lock();
         if(available.size()){
          Connection *cnt = nullptr;
           for(auto v: available){
            cnt = v.second;
            inuse[v.first]=cnt; 
            available.erase(v.first);
            break;
           }
           mtx.unlock()
           return cnt;
         }else{
          mtx.unlock();
          Connection *cnt = new Connection();
          cnt->open();
          mtx.lock();
          inuse[cnt -> connectionID()] = cnt;
          mtx.unlock();
          return cnt;
         }
     }

     void returnConnection(Connection *cnt){
          mtx.lock();
          inuse.erase(cnt->connectionID());
          available[cnt->connectionID()]=cnt;
          mtx.unlock();
     }

};

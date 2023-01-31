#include <sys/time.h>
#include <algorith>

/*
struct range {
  struct timeval t_sec;
  struct timeval t_usec;
};
*/

typedef std::vector<std::pair<struct timeval, struct timeval>> range;

int mergeIntervals(range &input){
  //O(NlogN) time
  sort(input.begin(), input.end(),
       [](range &a, range &b){
         return (a.first.tv_sec*1000000+a.first.tv_usec) < (b.first.tv_sec*1000000+b.first.tv_usec);
       });
  //O(N*M) time N number of elements and M number of elements moved after erase.
  for(int i=0; i<input.size()-1;){
    auto &a = input[i];
    auto &b = input[i+1];
    if(a.second.tv_sec < b.second.tv_sec && a.second.tv_usec < b.second.tv_usec){
      b.first.tv_sec = a.first.tv_sec;
      b.first.tv_usec = a.first.tv_usec;
      input.erase(input.begin()+i);//O(N)
    }else{
      i++;
    }
  }
  return input.size();
}

int mergeIntervals(range &input){
  sort(input.begin(), input.end(),
       [](range &a, range &b){
         return (a.first.tv_sec*1000000+a.first.tv_usec) < (b.first.tv_sec*1000000+b.first.tv_usec);
       });
  int erase_count = 0;
  for(int i=0; i<input.size()-1;){
    auto &a = input[i];
    auto &b = input[i+1];
    if(a.second.tv_sec == b.first.tv_sec && a.second.tv_usec == b.first.tv_usec){
      a.second.tv_sec = b.second.tv_sec;
      a.second.tv_usec = b.second.tv_usec;
      b.first.tv_sec = a.first.tv_sec;
      b.first.tv_usec = a.first.tv_usec;
      erase_count++;
    }else{
      i++;
    }
  }
  return input.size();
}


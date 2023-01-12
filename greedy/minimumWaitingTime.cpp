//O(nlogn) time and O(n) space
int minimumWaitingTime(vector<int> queries) {
  int waitingTime = 0;
  sort(queries.begin(),queries.end());
  for(int i=1; i<queries.size(); i++){
    waitingTime+=queries[i-1]*(queries.size()-i);
  }
  return waitingTime;
}
/*
1,2,  2,    3       ,6
0,1
  1,1+2,
   ,1+2,1+2,2,
        1+2+2,1+2+2+3
  */

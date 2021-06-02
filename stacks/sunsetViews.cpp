using namespace std;

//O(n) time and O(n) space
vector<int> sunsetViews(vector<int> buildings, string direction) {
  // Write your code here.
	vector<int> v;
	int cur=-1;
	int i=0, count=0, step=1;
	if(direction.compare("EAST")==0){
		i=buildings.size()-1;
		step=-1;
	}
	while(count<buildings.size()){
     if(cur<buildings[i]){
        v.push_back(i);
			 cur=buildings[i];
		 }
		 i+=step;
     count++;
	}
	if(direction.compare("EAST")==0){
	   sort(v.begin(),v.end());
	}
	return v;
}

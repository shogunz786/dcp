using namespace std;

	struct Item{
		int value;
		int arrayno;
		int arrayidx;
	};
class MinHeap{
	public:
	vector<Item> h;
  void insert(int value, int arrayno, int arrayidx){
	  Item it = {value, arrayno, arrayidx};
		h.push_back(it);
		siftUp(h.size()-1);
	}	
	Item remove(){
		Item it = {-1,-1,-1};
		if(h.size()){
			it=h[0];
			swap(h[0],h[h.size()-1]);
			h.pop_back();
			siftDown(0);
		}
		return it;
	}
	
	void siftUp(int i){
		int p=(i-1)/2;
		while(i>0){
			if(h[p].value>h[i].value){
				swap(h[p],h[i]);
			}
			i=p;
			p=(i-1)/2;
		}
	}
	
	void siftDown(int i){
		int l=2*i+1;
		int r=2*i+2;
		int m=i;
		if(l<h.size() && h[l].value<h[m].value){
			m=l;
		}
		if(r<h.size() && h[r].value<h[m].value){
			m=r;
		}
		if(m!=i){
			swap(h[m],h[i]);
			siftDown(m);
		}
	}
};

//O(nlogk+k) time and O(n+k) space
vector<int> mergeSortedArrays(vector<vector<int>> arrays) {
  // Write your code here.
	vector<int> res;
	MinHeap mh;
	int r=0, c=0;
	int i=0;
	for(auto a: arrays){
	 mh.insert(a[0], r, 0);	
	 r++;
	}
	while(mh.h.size()){
		Item it = mh.remove();
		res.push_back(it.value);
		if(it.arrayidx+1 < arrays[it.arrayno].size()){
			mh.insert(arrays[it.arrayno][it.arrayidx+1],
						it.arrayno,
						it.arrayidx+1);
		}
	}
  return res;
}

#include <vector>
#include <iostream>
using namespace std;

// Do not edit the class below except for
// the insert method. Feel free to add new
// properties and methods to the class.

class Heap{
	public:
	vector<int> h;
	void insert(int n){
		h.push_back(n);
		siftUp(h.size()-1);
	}
	int peek(){
		return h.size()?h[0]:-1;
	}
	int remove(){
		int ret=-1;
		if(h.size()){
			ret = h[0];
			swap(h[0],h[h.size()-1]);
			h.pop_back();
			siftDown(0);
		}
		return ret;
	}
	virtual void siftUp(int i)=0;
	virtual void siftDown(int i)=0;
};

class MinHeap: public Heap{
	public:
	void siftUp(int i){
		int p = (i-1)/2;
		while(i>0 && h[i]<h[p])
		{
			swap(h[i],h[p]);
			i=p;
			p=(i-1)/2;
		}
	}
	
	void siftDown(int i){
		int l=i*2+1;
		int r=i*2+2;
		int mn=i;
		if(l<h.size() && h[l]<h[mn])
			mn=l;
		if(r<h.size() && h[r]<h[mn])
			mn=r;
		if(mn!=i){
			swap(h[i],h[mn]);
			siftDown(mn);
		}
	}
};

class MaxHeap: public Heap{
	public:
	void siftUp(int i){
		int p = (i-1)/2;
		while(i>0 && h[i]>h[p])
		{
			swap(h[i],h[p]);
			i=p;
			p=(i-1)/2;
		}
	}
	
	void siftDown(int i){
		int l=i*2+1;
		int r=i*2+2;
		int mx=i;
		if(l<h.size() && h[l]>h[mx])
			mx=l;
		if(r<h.size() && h[r]>h[mx])
			mx=r;
		if(mx!=i){
			swap(h[i],h[mx]);
			siftDown(mx);
		}
	}
};



class ContinuousMedianHandler {
  public:
    double median;
    MaxHeap maxheap;//lowers
	  MinHeap minheap;//highers
	
	//O(logn) time and O(n) space
    void insert(int number) {
			
      // Write your code here.
			//maintain lower half max vals in maxheap
			if(maxheap.h.size()==0 || number<maxheap.peek()){
				maxheap.insert(number);
			}else{
				//maintain upper half mins in minheap
				minheap.insert(number);
			}
	    if((maxheap.h.size()-minheap.h.size())==2){
				minheap.insert(maxheap.remove());
			}else if((minheap.h.size()-maxheap.h.size())==2){
				maxheap.insert(minheap.remove());
			}
			if(maxheap.h.size() == minheap.h.size()){
				median=(double)(maxheap.peek()+minheap.peek())/2;
			}else if(maxheap.h.size() > minheap.h.size()){
				median=maxheap.peek();
			}else{
				median=minheap.peek();
			}
			
		}

    double getMedian() {
      return median;
    }
};

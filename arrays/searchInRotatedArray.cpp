/*
Search for a given number in a sorted array, with unique elements, that has been rotated by some arbitrary number. Return -1 if the number does not exist.
Assume that the array does not contain duplicates.
*/

//O(logN) time and O(logN) space
   int findRotateIndex(vector<int>& arr){
        int l=0, r=arr.size()-1;
        //already sorted
        if(arr[l]<arr[r])
            return 0;
        while(l<=r){
            int m=l+(r-l)/2;
            //mid is less than mid+1 then found the beginning
            if(arr[m]>arr[m+1]){
                return m+1;
                //l is greater than mid move left
            }else if(arr[l]>arr[m]){
                r=m-1;
                //left is ok move right
            }else{
                l=m+1;
            }
        }
        return 0;
    }
    int binary_search(vector<int> &arr, int l, int r, int k){
        while(l<=r){
            int m=l+(r-l)/2;
            //match mid return
            if(arr[m]==k){
                return m;
            }else if(k>arr[m]){
                l=m+1;
            }else{
                r=m-1;
            }
        }
        return -1;
    }
    int search(vector<int>& arr, int k) {
        if(arr.size()==0){
            return -1;
        }else if(arr.size()==1){
            return arr[0]==k?0:-1;
        }
        int b=findRotateIndex(arr);
        int l=0;
        int r=arr.size()-1;
        //match rotate index or beginning or end 
        if(k==arr[b]){
            return b;
        }else if(k==arr[l]){
            return l;
        }else if(k==arr[r]){
            return r;
        //k between rotate index and end
        }else if(k>arr[b] && k<arr[r]){
            return binary_search(arr,b+1,r-1,k);
        //k between left and rotate index
        }else {
             return binary_search(arr,l+1,b-1,k);
        }
    }

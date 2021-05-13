void sum3(vector<int> arr, int match)
{
   sort(arr.begin(), arr.end());//first sort
   int sum = 0;
   int l=0, r =0;
   for(int i=0; i<arr.size()-2; i++)
   {
     l=i+1;
     r=arr.size()-1;
     while(l<r){
       sum = arr[i]+arr[l]+arr[r];//fix i and keep moving r and l
       if(sum == match){
          cout<<arr[i]<<","<<arr[l]<<","<<arr[r]<<endl;
          l++;
          r--;
       }else if(sum >match){
         r--;
       }else
         l++;
     }  
   }
}
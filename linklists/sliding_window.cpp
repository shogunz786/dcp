void sliding_max(vector<int> arr, vector<int> &maxarr, int w)
{
  list<int> indices;
  
  for(int i=0; i<arr.size(); i++)
  {
     //remove indices in the back
     //whose value is less than current index value
     while(!indices.empty() && arr[indices.back()] <= arr[i])
        indicies.pop_back();
 
     //add index to the back
     indices.push_back(i); 
     if(i>=w-1) //index reached width
     {
        //remove index values out of the range from front
        while(indices.front()<= i-w)
          indicies.pop_front();

        //add front as the max value
        maxarr.push_back(arr[indices.front()]);
     }
  }
}

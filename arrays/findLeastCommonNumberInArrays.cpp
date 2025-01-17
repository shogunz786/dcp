/*
 Problem:given three positive integer arrays which are sorted in ascending order.
         You have to find the smallest number that is common in all three arrays. Return -1 if the smallest common number is not found.
 Example:
 input:
 {6, 7, 10, 25, 30, 63, 64};
 {1, 4, 5, 6, 7, 8, 50};
 {1, 6, 10, 14};
 output:
 6
 */


/*
 Iterative Solution:Use one iterator per array and Find array with smallest value and increment the iterator of the array
 till a match is found.
 Time:O(n)
 Space:O(1)
 */

int find_least_common_number(vector<int>& arr1, vector<int>& arr2, vector<int>& arr3) {
  int i = 0, j = 0, k = 0;

  while(i < arr1.size() && j < arr2.size() && k < arr3.size()) {

    // Finding the smallest common number
    if((arr1[i] == arr2[j]) && (arr2[j] == arr3[k]))
      return arr1[i];


    // Let's increment the iterator
    // for the smallest value.

    if(arr1[i] <= arr2[j] &&  arr1[i] <= arr3[k]) {
      i++;
    }

    else if(arr2[j] <= arr1[i] && arr2[j] <= arr3[k]) {
      j++;
    }

    else if(arr3[k] <= arr1[i] && arr3[k] <= arr2[j]) {
      k++;
    }
  }
  return -1;
}

int main(int argc, char* argv[]) {
  vector<int> v1 = {6, 7, 10, 25, 30, 63, 64};
  vector<int> v2 = {1, 4, 5, 6, 7, 8, 50};
  vector<int> v3 = {1, 6, 10, 14};
  int result = find_least_common_number(v1, v2, v3);
  cout << "Least Common Number: " <<result;
}

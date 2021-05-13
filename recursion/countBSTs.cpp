/* BST keys are like a sorted array
 * as we iterate the array each index can beome root
 * from there left elements are in left tree and 
 * right elements are in right tree
 */
int countBSTs(int keys)
{
   int sum = 0, left, right;
   if(keys<=1)
	   return 1;
   for(int root=1; root<=keys; root++)
   {
	 left = countBSTs(root-1);
	 right = countBSTs(keys-root);
	 sum += left*right;
   }
   return sum;
}

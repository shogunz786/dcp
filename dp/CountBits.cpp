/*
Given an integer n, return an array ans of length n + 1 such that for
each i (0 <= i <= n), ans[i] is the number of 1's in the binary representation of i.
*/
class Solution {
public:
    vector<int> countBits(int n) {
        vector<int> res(n+1, 0);
        for(int i=1; i<=n; i++){
            //odd
            //7 - 111 -- one more bit than 7/2 = 3
            //3 - 011
            //even
            //4 - 100 - same number of bits as 4/2 = 2
            //2 - 010
            //use dp to resuse results
            res[i]=res[i/2]+i%2;
        }
        return res;
    }
};

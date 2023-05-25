/*
We are playing the Guessing Game. The game will work as follows:

I pick a number between 1 and n.
You guess a number.
If you guess the right number, you win the game.
If you guess the wrong number, then I will tell you whether the number I picked is higher or lower, and you will continue guessing.
Every time you guess a wrong number x, you will pay x dollars. If you run out of money, you lose the game.
Given a particular n, return the minimum amount of money you need to guarantee a win regardless of what number I pick.
*/
class Solution {
public:
    int rundp(vector<vector<int>> &dp, int s, int e){
        if(s>=e) return 0;
        if(dp[s][e]) return dp[s][e];
        int res = INT_MAX;
        for(int i=s; i<=e; i++){
            //when picking i
            //cost of i + maximum possible worst case scenario
            //consider lowest of such worst case scenarios
            res = min(res,i+max(rundp(dp, s, i-1), rundp(dp, i+1, e)));
        }
        dp[s][e] = res;
        return dp[s][e];
    }
    int getMoneyAmount(int n) {
        vector<vector<int>> dp(n+1, vector<int>(n+1, 0));
        return rundp(dp, 1, n);
    }
};

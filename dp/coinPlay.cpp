/*
 * In a row of coins of values.
 * Play a game against an opponent by alternating turns. In each turn, a player selects either 
 * the first or last coin from the
 * row, removes it from the row permanently, 
 * and receives the value of the coin. Determine the
 * 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 
 * 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 10 10 101 102 
 * 103 104 105 106 107 108 119 0
 * row, removes it from the row permanently, and receives the value of the coin. 
 * Determine the maximum possible amount of money we can definitely win if 
 * we move first. Assume full competency by both players.
 */
/*
 *F(i, j)  represents the maximum value the user can collect from 
 *         i'th coin to j'th coin.
 *
 *    F(i, j)  = Max(Vi + min(F(i+2, j), F(i+1, j-1) ), //start from ith coin 
 *                   Vj + min(F(i+1, j-1), F(i, j-2) )) //start from jth coin
 *Base Cases
 *    F(i, j)  = Vi           If j == i
 *    F(i, j)  = max(Vi, Vj)  If j == i+1
 */

/*i begin index, j end index */
int coin_max_profit_rec(vector<int>& v, int i , int j) {
 if(j == i) return v[i];//last element
 if(j -i == 1) return max(v[i], v[j]);//last two elements
 if(i > j) return INT_MAX;//begin index is less than end index

 //first player picks begin index, second player picks min of available choice either begin+1 or end 
 int p1 = v[i] + min(coin_max_profit_rec(v,i+2,j), coin_max_profit_rec(v,i+1,j-1));
 //first player picks end index, second player picks min of available choice either begin+1 or end-1  
 int p2 = v[j] + min(coin_max_profit_rec(v,i+1,j-1), coin_max_profit_rec(v,i,j-2));
 return max(p1,p2);//pick maximum combination
}

// Returns optimal value possible that a player can
// collect from an array of coins of size n. Note
// than n must be even
// time O(n^2) and space O(n^2)
int optimalStrategyOfGame(int* arr, int n)
{
 // Create a table to store solutions of subproblems
 int table[n][n];

 // Fill table using above recursive formula. Note
 // that the table is filled in diagonal fashion
 for (int gap = 0; gap < n; ++gap) {
  for (int i = 0, j = gap; j < n; ++i, ++j) {

   // Here x is value of F(i+2, j), y is F(i+1, j-1) and
   // z is F(i, j-2) in above recursive formula
   int x = ((i + 2) <= j) ? table[i + 2][j] : 0;
   int y = ((i + 1) <= (j - 1)) ? table[i + 1][j - 1] : 0;
   int z = (i <= (j - 2)) ? table[i][j - 2] : 0;

   table[i][j] = max(arr[i] + min(x, y), arr[j] + min(y, z));
  }
 }

 return table[0][n - 1];
}

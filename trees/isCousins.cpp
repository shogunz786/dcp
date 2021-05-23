/**
 * n a binary tree, the root node is at depth 0, and children of each depth k node are at depth k+1.
   Two nodes of a binary tree are cousins if they have the same depth, but have different parents.
   We are given the root of a binary tree with unique values, and the values x and y of two different nodes in the tree.
   Return true if and only if the nodes corresponding to the values x and y are cousins.

 *
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:

    bool isCousins(TreeNode* root, int x, int y) {
        queue<TreeNode*> q1;
        queue<TreeNode*> q2;
        q1.push(root);
        while(q1.size()){
            int xfound =0, yfound=0;
            //file x and y at same level
            while(q1.size()){
                TreeNode* t = q1.front();
                q1.pop();
                if(t==NULL){
                    continue;
                }
                if(t->val==x){
                    xfound=1;
                }else if(t->val==y){
                    yfound=1;
                }
                //make sure x and y are from not the same parent
                if(t->left && t->right){
                  if((t->left->val == x && t->right->val == y) || 
                    (t->left->val == y && t->right->val == x)){
                    return false;
                  }
                }
                q2.push(t->left);
                q2.push(t->right);
            }
            //transfer to q1
            while(q2.size()){
                q1.push(q2.front());
                q2.pop();
            }
            //if x and y found return true
            if(xfound && yfound){
                return true;
            //if only a or y found return false
            }else if((xfound && !yfound) ||
                     (!xfound && yfound)){
                return false;
            }
        }
        return false;
    }
};

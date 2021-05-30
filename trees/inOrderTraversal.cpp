		//time O(N) space O(N)
		void inOrderRec( TreeNode *root, vector<int> &res){
			if(root==nullptr){
				return;
			}
			inOrderRec(root->left, res);
			res.push_back(root->val);
			inOrderRec(root->right, res);
		}

		//time O(N) space O(N)
		void inOrderIter( TreeNode *root, vector<int> &res){
			stack<TreeNode*> st;
			while(root || !st.empty())
			{
				//stack up the left most nodes
				//got to left most leaf
				if(root){
					st.push(root);
					root=root->left;
				}else{
					//pop and capture current node
					root=st.top();
					st.pop();
					res.push_back(root->val);
					//now stack up the right node's leftmost path
					root=root->right;
				}
			}
		}

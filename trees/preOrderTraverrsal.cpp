//time O(N) space O(N)
		void preOrderRec( TreeNode *root, vector<int> &res){
			if(root==nullptr){
				return;
			}
			res.push_back(root->val);
			preOrderRec(root->left, res);
			preOrderRec(root->right, res);
		}

		//time O(N) space O(N)
		void preOrderIter( TreeNode *root, vector<int> &res){
			stack<TreeNode*> st;
			//init stack with root
			st.push(root);
			while(!st.empty())
			{
				//pop and capture current node
				root=st.top();
				st.pop();
				res.push_back(root->val);
				//push right and then left to stack
				if(root->right)st.push(root->right);
				if(root->left)st.push(root->left);
			}
		}

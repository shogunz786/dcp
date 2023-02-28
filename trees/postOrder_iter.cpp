void postorder(Node *root)
{
	if(root==NULL)
		return;
	stack<Node *> sn;
	while(root || !sn.empty())
	{
		if(root)
		{
			if(root->right)
				sn.push(root->right);
			sn.push(root);
			root = root->left;
		}else{
			Node *temp = sn.top();
			sn.pop();
			if(temp->right == sn.top())
			{
				sn.pop();
				sn.push(temp);
				root = temp->right;
			}else
				cout<<temp->data<<endl;
		}
	}
}

// Iterative function to perform postorder traversal on the tree
void postorderIterative(Node* root)
{
    // return if the tree is empty
    if (root == nullptr) {
        return;
    }
 
    // create an empty stack and push the root node
    stack<Node*> s;
    s.push(root);
 
    // create another stack to store postorder traversal
    stack<int> out;
 
    // loop till stack is empty
    while (!s.empty())
    {
        // pop a node from the stack and push the data into the output stack
        Node* curr = s.top();
        s.pop();
 
        out.push(curr->data);
 
        // push the left and right child of the popped node into the stack
        if (curr->left) {
            s.push(curr->left);
        }
 
        if (curr->right) {
            s.push(curr->right);
        }
    }
 
    // print postorder traversal
    while (!out.empty())
    {
        cout << out.top() << " ";
        out.pop();
    }
}

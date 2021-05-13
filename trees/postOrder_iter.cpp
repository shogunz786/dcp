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

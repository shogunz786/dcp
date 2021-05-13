void kth_smallest(Node *root, int *k, int *smallest)
{
	if(root == NULL || (*k)<=0)
		return;
	kth_smallest(root->left, k, smallest);
	(*k)--;
	if(*k==0)
		*smallest = root->data;
	else
	  kth_smallest(root->right, k, smallest);
}

void iter_kth_smallest(Node *root, int k, int *smallest)
{
	stack<Node*> sn;
	while(root!=NULL && !sn.empty())
	{
		if(root){
			sn.push(root);
			root = root->left;
		}else{
			root = sn.pop(root);
			k--;
			if(k==0){
				*smallest = root->data;
				return;
			}else{
				root = root->right;
				while(root){
					sn.push(root)
					root = root->left;
				}
			}
		}
	}
}

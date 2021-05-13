//works for perfect binary tree
void populateSiblings(Node *root)
{
	if(!root) return;
	if(root->left == NULL && root->right==NULL)return;
	if(root->left && root->right) root->left->next = root->right;
	if(root->next)
		root->right->next = root->next->left;
	polulateSiblings(root->left);
	polulateSiblings(root->right);
}

void populateSiblings_iterative(Node *root)
{
	Node *prev = NULL;
	Node *head = NULL;
	while(root)
	{
		while(root)
		{
			if(root->left){ //if prev available link prev to left
				if(prev){
					prev->next = root->left;
				}else{ //no prev so set left as head to this level
					head = root->left;
				}
				prev = root->left;
			}
			if(root->right){ //if prev available link prev to right
				if(prev){
					prev->next = root->right;
				}else{ //no prev so set right as head to this level
					head = root->right;
				}
				prev = root->right;
			}
			//when no next at this level exit
			root = root->next;
		}
		root = head;
		prev = NULL;
	}
}

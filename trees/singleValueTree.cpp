	// This function increments count by number of single 
// valued subtrees under root. It returns true if subtree 
// under root is Singly, else false. 
bool singleValueTree(Node *root, int &count)
{ 
	// Return false to indicate NULL 
	if (root == NULL) 
	return true; 

	// Recursively count in left and right subtrees also 
	bool left = singleValueTree(root->left, count); 
	bool right = singleValueTree(root->right, count); 

	// If any of the subtrees is not singly, then this 
	// cannot be singly. 
	if (left == false || right == false) 
	return false; 

	// If left subtree is singly and non-empty, but data 
	// doesn't match 
	if (root->left && root->data != root->left->data) 
		return false; 

	// Same for right subtree 
	if (root->right && root->data != root->right->data) 
		return false; 

	// If none of the above conditions is true, then 
	// tree rooted under root is single valued, increment 
	// count and return true. 
	count++; 
	return true; 
} 


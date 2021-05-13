int heightOfK(Node * root)
{
	if(root==NULL)
		return 0;
	int ht = 0;
	for(int i=0; i<root->data.size(); i++)
	{
		ht = max(ht,heightOfK(root->data[i]));
	}
	return ht+1;
}

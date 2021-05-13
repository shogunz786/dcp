class TreeIter{
   private:
     stack<Node*> sn;
     void pushall(Node *root){ while(root){sn.push(root);root=root->left;}
   public:
     TreeIter(Node *root){pushall(root);}
     bool hasNext(){ return !sn.empty();}
     int next(){ Node *root=sn.top(); sn.pop();
	     pushall(root->right); return root->data;}
};

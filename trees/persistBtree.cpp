/*
 * Given the root to a binary tree, implement serialize(root), which serializes the tree into a string,
 * and deserialize(s), which deserializes the string back into the tree.
 */

#include<iostream>
#include "treeNode.h"
#include "treeNodeMgr.h"

//run time O(N) space O(1)
string TreeNodeMgr::serializeIter(TreeNode *root){
	queue<TreeNode*> q;
	ostringstream ostr;
	if(root==nullptr){
		return "# ";
	}
	q.push(root);

	while(!q.empty()){
		TreeNode* temp = q.front();
		q.pop();
		if(temp == nullptr){
			ostr<<"# ";
		}else{
			ostr<<temp->val<<" ";
			q.push(temp->left);
			q.push(temp->right);
		}
	}
	return ostr.str();
}

//run time O(N) space O(1)
TreeNode* TreeNodeMgr::deSerializeIter(string str){
	if(str == "# "){
		return nullptr;
	}
	istringstream istr(str);
	string v;
	istr>>v;
	TreeNode* root = new TreeNode(stol(v));
	queue<TreeNode*> q;
	q.push(root);
	while(!q.empty()){
		TreeNode *temp = q.front();
		q.pop();
		if(istr){
			istr>>v;
			if(v!="#"){
				temp->left=new TreeNode(stol(v));
				q.push(temp->left);
			}
		}
		if(istr){
			istr>>v;
			if(v!="#"){
				temp->right=new TreeNode(stol(v));
				q.push(temp->right);
			}
		}
	}
	return root;
}

int main(int argc, char *argv[]){
	string str("1 2 3 # # 4 5 # # # #");
        TreeNodeMgr mgr;
        cout<<"Recursive Serial/DeSerial"<<endl;
        istringstream istr(str);
        ostringstream ostr;
	TreeNode* root = mgr.deSerializeRecur(istr);
        mgr.serializeRecur(root, ostr);
        string tempStr = ostr.str();
        cout<<"str ="<<str<<endl;
        cout<<"tempStr ="<<tempStr<<endl;

        cout<<"Iterative Serial/DeSerial"<<endl;
	root = mgr.deSerializeIter(str);
        tempStr = mgr.serializeIter(root);
        cout<<"str ="<<str<<endl;
        cout<<"tempStr ="<<tempStr<<endl;
	return 0;
}

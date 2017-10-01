#ifndef TREENODE_H
#define TREENODE_H

#include <bits/stdc++.h>
#define ll long long int 

// Data structure to represent single instance of training or test data.
// key to represent the tuple of attribute values.
// val to represent target value.
struct data_ds
{
	std::vector<std::string> key;
	bool val;
};

class Treenode
{
	private:
		Treenode* parent; // parent of a node
		int ano; // index of attribute having maximum information gain.

	public:
		Treenode();
		int pos; // number of positive examples in data (dataset for the subtree rooted at this node)
		int neg; // number of negative examples in data (dataset for the subtree rooted at this node)
		int cv; // In case ano represents a continous attribute, cv is the value of this attribute that splits data.
		std::pair<int,int> missing; // for handling missing values.
		std::vector< data_ds > data; // dataset for subtree rooted at this node.
		std::set<int> aset; //set of attributes available for subtree rooted at this node.
		std::vector<Treenode*> children; // vector containing reference to object of child nodes.
		std::vector< data_ds > getData();
		void setData(std::vector< data_ds > &data);
		int getAno();
		void setAno(int no);
		Treenode* getParent();
		void setParent(Treenode* node);
		std::vector<Treenode* > getChildren();
		void setChildren(std::vector<Treenode* > children);
		~Treenode();
};

#endif
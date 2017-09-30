#ifndef PRUNING_H
#define PRUNING_H

#include <bits/stdc++.h>
#include "tree.cpp"
#define ll long long int

using namespace std;

struct pnode
{
	//Stores extra data relating to every node of decision tree.
	//tpos, tneg - number of +ve and -ve examples in validation set.
	//neg, pos - number of +ve and -ve examples in training set.
	//correct - number of correctly classified examples from validation set, passing through this node.
	ll tpos, tneg, pos, neg, correct;
	std::vector<pnode*> children;
	pnode();
	~pnode();
};

class PrunedTree
{
	public:
	Tree* tree;			//Decision Tree
	pnode* proot;		//Extra data related to decision tree
	ll tot, correct;	//total validation examples and number of correct ones.
	std::vector< std::vector<std::string> > data;	//validation set examples.
	//For function's description see .cpp file
	void traverse(pnode*&, Treenode*);
	int assign_testdata(pnode*&, Treenode*, vector<string>&);
	void prune(pnode*&, Treenode*&);
	PrunedTree(std::string, std::string);
	void runtest(std::string);
	~PrunedTree();
};

#endif
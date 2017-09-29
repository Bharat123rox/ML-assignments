#include "tree.cpp"
#define ll long long int
#ifndef PRUNING_H
#define PRUNING_H

struct pnode
{
	ll tpos, tneg, pos, neg, correct;
	bool pruned;
	vector<pnode*> children;
	pnode();
	~pnode();
};

class PrunedTree
{
	public:
	Tree tree;
	pnode* proot;
	ll tot, correct;
	std::vector< std::vector<std::string> > data;
	void traverse(pnode*&, Treenode*);
	std::vector< std::vector<std::string> > get_testdata(std::string);
	int assign_testdata(pnode*&, Treenode*, vector<string>&);
	void prune(pnode*&, Treenode*&);
	PrunedTree(Tree&, std::string);
	long double getAccuracy();
	~PrunedTree();
};

#endif
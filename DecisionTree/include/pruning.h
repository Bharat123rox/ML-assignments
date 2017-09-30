#ifndef PRUNING_H
#define PRUNING_H

#include <bits/stdc++.h>
#include "tree.cpp"
#define ll long long int

using namespace std;

struct pnode
{
	ll tpos, tneg, pos, neg, correct;
	std::vector<pnode*> children;
	pnode();
	~pnode();
};

class PrunedTree
{
	public:
	Tree* tree;
	pnode* proot;
	ll tot, correct;
	std::vector< std::vector<std::string> > data;
	void traverse(pnode*&, Treenode*);
	int assign_testdata(pnode*&, Treenode*, vector<string>&);
	void prune(pnode*&, Treenode*&);
	PrunedTree(std::string, std::string);
	void runtest(std::string);
	~PrunedTree();
};

#endif
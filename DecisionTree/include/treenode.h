#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#define ll long long int 
#ifndef TREENODE_H
#define TREENODE_H

struct data_ds
{
	std::vector<std::string> key;
	bool val;
};

class Treenode
{
	private:
		Treenode* parent;
		int ano;

	public:
		Treenode();
		std::vector< data_ds > data;
		std::map<int,bool> aset;
		std::vector<Treenode*> children;
		std::vector< data_ds > getData();
		void setData(std::vector< data_ds > data);
		int getAno();
		void setAno(int no);
		Treenode* getParent();
		void setParent(Treenode* node);
		std::vector<Treenode* > getChildren();
		void setChildren(std::vector<Treenode* > children);
};

#endif
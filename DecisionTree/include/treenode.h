#ifndef TREENODE_H
#define TREENODE_H

#include <bits/stdc++.h>
#define ll long long int 

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
		int pos;
		int neg;
		int cv;
		std::vector< data_ds > data;
		std::set<int> aset; //true if discrete, false if continuous
		std::vector<Treenode*> children;
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
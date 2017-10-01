#ifndef TREE_H
#define TREE_H

#include <bits/stdc++.h>
#include "treenode.cpp"
#define ll long long int

class Tree
{
	public:
		Treenode* root; //Reference to root node of decision tree
		std::vector< data_ds > tdata; // To store the training data.
		int atbno; // No. of attributes in the training data.
		bool rf; // Is decision tree a part of random forest.
		std::map<int,std::set<std::string> > avals; //Storing the possible values of an attribute (index of attrb is map's key)
		std::set<int> cvals; //stores the index of attibutes with continuous values.
		Tree();
		void loadTrainingData(std::string datafile,int no_of_atb);
		void loadDomainKnowledge(std::string datafile);
		int stringToInt(std::string str);
		std::pair<long double,int> splitContinuous(Treenode* node,int ano, std::pair<int,int>&);
		long double splitDiscrete(Treenode* node,int ano, std::pair<int,int>&);
		long double getEntropy(int npos,int nneg);
		std::string intToString(int no);
		void printData();
		void printinfo();
		void setrootNode();
		void makeTree(Treenode* node);
		void traverse(Treenode* node);
		bool predict(std::vector<std::string> &test,Treenode* node);
		void runtest(std::string datafile);
		Treenode* getrootNode();
		Tree(std::string, std::string);
		~Tree();
};

#endif
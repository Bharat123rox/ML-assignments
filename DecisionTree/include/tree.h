#include "treenode.cpp"
#define ll long long int
#ifndef TREE_H
#define TREE_H

class Tree
{
	private:
		Treenode* root;

	public:
		Tree();
		std::vector< data_ds > tdata;
		int atbno;
		std::map<int,std::set<std::string> > avals; //Storing the possible values of an attribute (index of attrb is map's key)
		std::set<int> cvals; //stores the index of attibutes with continuous values.
		void loadTrainingData(std::string datafile,int no_of_atb);
		void loadDomainKnowledge(std::string datafile);
		int stringToInt(std::string str);
		int splitContinuous(Treenode* node,int ano);
		long double getEntropy(int npos,int nneg);
		void printData();
		void printinfo();
};

#endif
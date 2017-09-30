#ifndef RF_H
#define RF_H
#include "pruning.cpp"
#define ll long long int

using namespace std;

class RandomForest
{
	public:
	vector< data_ds > tdata;
	int atbno;
	map<int, set< string> > avals; //Storing the possible values of an attribute (index of attrb is map's key)
	set<int> cvals; //stores the index of attibutes with continuous values.
	vector<Tree*> trees;
	void build();
	RandomForest(string,string);
	void runtest(string);
};

#endif

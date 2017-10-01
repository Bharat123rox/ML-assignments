#include "include/randomforest.cpp"
#include <bits/stdc++.h>
#define ll long long int

using namespace std;

int main()
{
	Tree t("./raw_data/data.csv","./data/description.txt");
	cout<<"***********      ID3      ***********"<<endl;
	t.runtest("./raw_data/testdata.csv");
	cout<<endl;
	cout<<"***********    Pruning    ***********"<<endl;
	PrunedTree p("./raw_data/data.csv","./data/description.txt");
	p.runtest("./raw_data/testdata.csv");
	cout<<endl;
	cout<<"*********** Random Forest ***********"<<endl;
	RandomForest r("./raw_data/data.csv","./data/description.txt");
	r.runtest("./raw_data/testdata.csv");
	return 0;
}
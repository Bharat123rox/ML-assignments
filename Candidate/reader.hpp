#include <bits/stdc++.h>
#define ll long long int
using namespace std;
vector<vector<int> >* v = new vector<vector<int> >;
vector<int>* target = new vector<int>;
fstream inp("inputdata.csv");
fstream inpt("target.csv");
string row,item,t;
char sep=',';
//Prototypes for the functions
void printfull(vector<vector<int> >);
void printtarget(vector<int> v);
void collect();
void printfull(vector<vector<int> > v){
	for(auto i:v){
		for(auto j:i){
			cout<<j<<' ';
		}
		cout<<'\n';
	}
}
void printtarget(vector<int> v){
	for(auto i:v){
		cout<<i<<'\n';
	}
}
void collect()
{
	if(!inp){
		cout<<"Failed to open!";
	}
	if(!inpt){
		cout<<"Target failed to open!";
	}
	while(getline(inpt,t)){
		stringstream tar(t);
		int tr=0;
		tar>>tr;
		target->push_back(tr);
	}
	while(getline(inp,row)){
		stringstream ss(row);
	vector<int>* res = new vector<int>;
		while(getline(ss,item,sep)){
			stringstream s(item);
			int r=0;
			s>>r;
			res->push_back(r);
		}
		v->push_back(*res);
		delete res;
	}
	//printfull(*v);
	//printtarget(*target);
}
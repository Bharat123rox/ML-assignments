#include <bits/stdc++.h>
#define ll long long int
using namespace std;
vector<vector<int> > v;
vector<int> target;
string row,item,t;
char sep=',';
//Prototypes for the functions
void printfull(vector<vector<int> >);
void printtarget(vector<int> v);
void collect();
void printfull(vector<vector<int> > v){
	size_t i,j;
	for(i=0;i<v.size();i++){
		cout<<i<<' ';
		for(j=0;j<v[i].size();j++){
			cout<<v[i][j]<<' ';
		}
		cout<<'\n';
	}
}
void printtarget(vector<int> v){
	size_t i;
	for(i=0;i<v.size();i++){
		cout<<i<<' '<<v[i]<<'\n';
	}
}
void collect()
{
	fstream inp("/media/bharatrlinux/LinuxSecondary/GitHubRepos/ML-assignments/Candidate/Dataset/inputdatafinal.csv",std::fstream::in);
	fstream inpt("/media/bharatrlinux/LinuxSecondary/GitHubRepos/ML-assignments/Candidate/Dataset/target.csv",std::fstream::in);
	if(!inp){
		cout<<"Failed to open! ";
		return;
	}
	if(!inpt){
		cout<<"Target failed to open!";
		return;
	}
	while(getline(inpt,t)){
		stringstream tar(t);
		int tr=0;
		tar>>tr;
		target.push_back(tr);
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
		v.push_back(*res);
		delete res;
		inp.close();
		inpt.close();
		//printfull(*v);
		//printtarget(*target);
	}
}
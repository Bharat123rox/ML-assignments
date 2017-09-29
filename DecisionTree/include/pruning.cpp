#include <bits/stdc++.h>
#include "pruning.h"

#define ll long long int
using namespace std;

pnode::pnode()
{
	tpos=tneg=pos=neg=correct=0;
	pruned = false;
	children.clear();
}

pnode::~pnode()
{
	if(!children.empty()) for(auto ch:children) delete ch;
	if(!children.empty()) children.clear();
}

vector< vector<string> > PrunedTree::get_testdata(string datafile)
{
	int i,j,size;
	size = datafile.length();
	char* buff = new char[size+1];
	vector< vector<string> > ret;
	for(i=0;i<size;i++)
	{
		buff[i] = datafile[i];
	}
	buff[i]='\0';
	ifstream ifil;
	string line;
	int total=0;
	int correct=0;
	ifil.open(buff,std::fstream::in);
	while(!ifil.eof())
	{
		getline(ifil,line);
		size = line.length();
		vector<string> vals;
		string temp="";
		queue<string> wq;
		bool flag = false;
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				if(temp.compare("?")==0)
				{
					temp="";
					wq.push(line);
					flag = true;
					break;
				}
				else
				{
					vals.push_back(temp);
					temp="";
					i++;
				}
			}
			else
			{
				temp.push_back(line[i]);
			}
		}
		if(!flag)
		{
			vals.push_back(temp);
			ret.push_back(vals);
		}
	}
	ifil.close();
	return ret;
}

void PrunedTree::traverse(pnode*& pn, Treenode* tn)
{
	pn=new pnode;
	pn->pos=tn->pos;
	pn->neg=tn->neg;
	if(tn->children.empty()) return;
	pn->children.resize(tn->children.size());
	for(ll i=0;i<tn->children.size();++i) traverse(pn->children[i], tn->children[i]);
}

int PrunedTree::assign_testdata(pnode*& pn, Treenode* tn, vector<string>& td)
{
	if(td[td.size()-1][0]=='<') pn->tneg++;
	else pn->tpos++;
	if(tn->getAno()==-1 || tn->pos==0 || tn->neg==0)
	{
		if(td[td.size()-1][0]=='<' && pn->pos<pn->neg){
			pn->correct++;
			return 1;
		}
		else if(td[td.size()-1][0]=='>' && pn->pos>=pn->neg){
			pn->correct++;
			return 1;
		}
		return 0;
	}
	int ano = tn->getAno();
	if(this->tree.cvals.size()>0 && this->tree.cvals.find(ano)!=this->tree.cvals.end())
	{
		int cv = (tn->children[0])->cv;
		int val = this->tree.stringToInt(td[ano]);
		bool ret;
		if(val<=cv) ret = assign_testdata(pn->children[0], tn->children[0], td);
		else ret = assign_testdata(pn->children[1], tn->children[1], td);
		pn->correct+=ret;
		return ret;
	}
	set<string>::iterator it = this->tree.avals[ano].begin();
	int ind = distance(it,this->tree.avals[ano].find(td[ano]));
	bool ret=assign_testdata(pn->children[ind], tn->children[ind], td);
	pn->correct+=ret;
	return ret;
}

void PrunedTree::prune(pnode*& pn, Treenode*& tn)
{
	if(pn->children.empty()) return;
	for(ll i=0;i<tn->children.size();++i) prune(pn->children[i], tn->children[i]);
	pn->correct = 0;
	for(auto ch:pn->children) pn->correct += ch->correct;
	int newc=0;
	if(pn->pos<pn->neg) newc=pn->tneg;
	else newc=pn->tpos;
	if(newc>=pn->correct){
		pn->correct = newc;
		pn->children.clear();
		tn->children.clear();
		tn->setAno(-1);
	}
	return;	
}

PrunedTree::PrunedTree(Tree& t, string datafile)
{
	this->tree=t;
	data = get_testdata(datafile);
	traverse(this->proot, t.getrootNode());
	tot=0;
	correct=0;
	cout<<proot->pos<<" "<<proot->neg<<endl;
	for(auto td: data){
		tot++;
		correct+=assign_testdata(proot, t.getrootNode(), td);
	}
	data.clear();
	prune(proot, this->tree.root);
	cout<<this->tree.root->getAno()<<endl;
}

long double PrunedTree::getAccuracy()
{
	return ((long double)(proot->correct))/tot;
}

PrunedTree::~PrunedTree()
{
	delete proot;
}
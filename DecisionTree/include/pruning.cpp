#include "pruning.h"

#define ll long long int
using namespace std;

pnode::pnode()
{
	tpos=tneg=pos=neg=correct=0;
	children.clear();
}

pnode::~pnode()
{
	if(!children.empty()) for(auto ch:children) delete ch;
	if(!children.empty()) children.clear();
}

void PrunedTree::traverse(pnode*& pn, Treenode* tn)
{
	//Assigns extra data for every decision tree node.
	pn=new pnode;
	pn->pos=tn->pos;
	pn->neg=tn->neg;
	if(tn->children.empty()) return;
	pn->children.resize(tn->children.size());
	for(ll i=0;i<tn->children.size();++i) traverse(pn->children[i], tn->children[i]);
}

int PrunedTree::assign_testdata(pnode*& pn, Treenode* tn, vector<string>& td)
{
	//Runs each test data from validation set and calculates correct, tneg and tpos.
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
	if(this->tree->cvals.size()>0 && this->tree->cvals.find(ano)!=this->tree->cvals.end())
	{
		int cv = (tn->children[0])->cv;
		int val;
		if(td[ano]=="?") val=((td[td.size()-1][0]=='>')?tn->missing.second:tn->missing.first);
		else val=this->tree->stringToInt(td[ano]);
		bool ret;
		if(val<=cv) ret = assign_testdata(pn->children[0], tn->children[0], td);
		else ret = assign_testdata(pn->children[1], tn->children[1], td);
		pn->correct+=ret;
		return ret;
	}
	set<string>::iterator it = this->tree->avals[ano].begin();
	int ind;
	if(td[ano]=="?") ind=((td[td.size()-1][0]=='>')?tn->missing.second:tn->missing.first);
	else ind = distance(it,this->tree->avals[ano].find(td[ano]));
	bool ret=assign_testdata(pn->children[ind], tn->children[ind], td);
	pn->correct+=ret;
	return ret;
}

void PrunedTree::prune(pnode*& pn, Treenode*& tn)
{
	//Performs pruning by removing nodes in a bottom up fashion and recording change in accuracy.
	if(pn->children.empty()) return;
	for(ll i=0;i<tn->children.size();++i) prune(pn->children[i], tn->children[i]);
	pn->correct = 0;
	for(auto ch:pn->children) pn->correct += ch->correct;
	int newc=0;
	if(pn->pos<pn->neg) newc=pn->tneg;
	else newc=pn->tpos;
	if(newc>=pn->correct){			//Node's children are removed if new accuracy > existing accuracy.
		pn->correct = newc;
		pn->children.clear();
		tn->children.clear();
		tn->setAno(-1);
	}
	return;	
}

PrunedTree::PrunedTree(string trainingdata, string domknow)
{
	//Accepts data and builds pruned decision tree.
	tree = new Tree();
	tree->loadDomainKnowledge(domknow);
	tree->loadTrainingData(trainingdata,tree->atbno);
	int sz = tree->tdata.size();
	vector<int> tmpvec;
	auto t2 = tree->tdata;
	tree->tdata.clear();
	//training data further split randomly, 2/3-new training set and 1/3-validation set.
	for(int i=0;i<sz;i++) tmpvec.push_back(i);
	random_shuffle(tmpvec.begin(), tmpvec.end());
	int i=0;
	for(;i<=((2*sz)/3);i++) tree->tdata.push_back(t2[tmpvec[i]]);
	for(;i<sz;i++){
		t2[tmpvec[i]].key.push_back(((t2[tmpvec[i]].val)?">50k":"<=50k"));
		data.push_back(t2[tmpvec[i]].key);
	}
	t2.clear();
	tree->setrootNode();
	tree->makeTree(tree->getrootNode());
	traverse(this->proot, tree->getrootNode());
	tot=0;
	correct=0;
	for(auto td: data){
		tot++;
		correct+=assign_testdata(proot, tree->getrootNode(), td);
	}
	data.clear();
	prune(proot, this->tree->root);
}

PrunedTree::~PrunedTree()
{
	delete proot;
}

void PrunedTree::runtest(string datafile)
{
	//Runs test data and calculates accuracy.
	this->tree->runtest(datafile);
}
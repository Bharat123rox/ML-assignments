#include "tree.h"
#define ll long long int
using namespace std;

Tree::Tree()
{
	this->root = NULL;
	this->tdata.clear();
	this->avals.clear();
	this->avals.clear();
	this->rf = false;
}

Tree::Tree(string trainingdata, string domknow)
{
	this->root = NULL;
	this->tdata.clear();
	this->avals.clear();
	this->avals.clear();
	this->rf = false;
	loadDomainKnowledge(domknow);
	loadTrainingData(trainingdata, atbno);
	setrootNode();
	makeTree(getrootNode());
}

bool compare(pair<int,bool> p1,pair<int,bool> p2)
{
	return p1.first<p2.first;
}

bool compare2(pair<string,int>p1, pair<string,int> p2)
{
	return p1.second<p2.second;
}

string Tree::intToString(int no)
{
	string ans="";
	while(no>0)
	{
		char ch = (char)(no%10)+48;
		ans.push_back(ch);
		no = no/10;
	}
	int i;
	int size = ans.length();
	for(i=0;i<size/2;i++)
	{
		char temp = ans[i];
		ans[i]=ans[size-1-i];
		ans[size-1-i]=temp;
	}
	return ans;
}

int Tree::stringToInt(string str)
{
	int i;
	int size = str.length();
	int ans = 0;
	int j = 1;
	for(i=size-1;i>=0;i--)
	{
		int val = (int)str[i]-48;
		ans = ans + val*j;
		j = j*10;
	}
	return ans;
}

long double Tree::getEntropy(int npos,int nneg)
{
	int ntot = npos + nneg;
	long double entro;
	if(npos==0 || nneg==0)
	{
		return 0;
	}
	entro = (npos*1.0000/ntot)*log2(ntot*1.00000/npos) + (nneg*1.0000000/ntot)*log2(ntot*1.00000/nneg);
	return entro;
}

/*Loads all possible attribute values into avals, if it takes discrete values. 
Loads attribute index into cvals if values are continous*/
void Tree::loadDomainKnowledge(string datafile)
{
	ifstream ifil;
	int i,size;
	size=datafile.length();
	char* buff = new char[size+1];
	for(i=0;i<size;i++)
	{
		buff[i] = datafile[i];
	}
	buff[i]='\0';
	ifil.open(buff,fstream::in);
	string line;
	int ind = 0;
	while(!ifil.eof())
	{
		getline(ifil,line);
		//cout<<line<<endl;
		line.pop_back();
		size = line.length();
		set<string> vals;
		string temp = "";
		i=0;
		while(line[i]!=':')
		{
			i++;
		}
		i+=2;
		while(i<size)
		{
			if(line[i]==',')
			{
				vals.insert(temp);
				temp="";
				i++;
			}
			else
			{
				temp.push_back(line[i]);
			}
			i++;
		}
		if(temp.compare("continuous")!=0)
		{
			vals.insert(temp);
			avals[ind]=vals;
		}
		else
		{
			cvals.insert(ind);
		}
		ind++;
	}
	this->atbno = ind;
	delete[] buff;
	ifil.close();
}

/*Loads the training data from file into tdata*/
void Tree::loadTrainingData(string datafile,int no_of_atb)
{
	ifstream ifil;
	int i,j,size;
	queue<data_ds> wq;
	size = datafile.length();
	char* buff = new char[size+1];
	for(i=0;i<size;i++)
	{
		buff[i] = datafile[i];
	}
	buff[i]='\0';
	ifil.open(buff,fstream::in);
	string line;
	while(!ifil.eof())
	{
		data_ds ds;
		string temp="";
		getline(ifil,line);
		size = line.length();
		vector<string> vals;
		bool flag = false;
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				vals.push_back(temp);
				temp="";
				i++;
			}
			else
			{
				temp.push_back(line[i]);
			}
		}
		size = vals.size();
		ds.key = vals;
		if(temp.compare("<=50K")==0) ds.val=false;
		else ds.val=true;
		tdata.push_back(ds);
	}
	delete[] buff;
	ifil.close();
}

void Tree::printData()
{
	int i,size,j;
	size = tdata.size();
	for(i=0;i<size;i++)
	{
		for(j=0;j<atbno;j++)
		{
			cout<<tdata[i].key[j]<<",";
		}
		cout<<tdata[i].val<<endl;
	}
}

void Tree::printinfo()
{
	int i,size;
	for(i=0;i<atbno;i++)
	{
		set<string>::iterator it = avals[i].begin();
		cout<<i<<": ";
		while(it!=avals[i].end())
		{
			cout<<*it<<",";
			it++;
		}
		if(cvals.find(i)!=cvals.end())
		{
			cout<<"continous";
		}
		cout<<endl;
	}
}

pair<long double,int> Tree::splitContinuous(Treenode* node,int ano, pair<int,int>& missing)
{
	int i,size;
	size=node->data.size();
	vector< pair<int,bool> > arr;
	pair<int,bool> temp;
	int tpos=0;
	int tneg=0;
	missing = make_pair(0,0);
	for(i=0;i<size;i++){
		auto tmpd = node->data[i];
		if(tmpd.key[ano]=="?") continue;
		if(tmpd.val==false) missing.first+=stringToInt(tmpd.key[ano]), tneg++;
		else missing.second+=stringToInt(tmpd.key[ano]), tpos++;
	}
	missing.first=int(missing.first/tneg);
	missing.second=int(missing.second/tpos);
	tneg=tpos=0;
	for(i=0;i<size;i++)
	{
		if(node->data[i].key[ano]=="?") node->data[i].key[ano] = intToString(((node->data[i].val)?missing.second:missing.first)); 
		temp.first = stringToInt(node->data[i].key[ano]);
		temp.second = node->data[i].val;
		if(temp.second==false)
		{
			tneg++;
		}
		else
		{
			tpos++;
		}
		arr.push_back(temp);
	}
	sort(arr.begin(),arr.end(),compare);
	int lpos=0;
	int lneg=0;
	long double tentro = getEntropy(tpos,tneg);
	size=arr.size();
	long double lentro=0;
	long double igmax=-1;
	int mval=arr[0].first;
	if(arr[0].second==false)
	{
		lneg++;
	}
	else
	{
		lpos++;
	}
	for(i=1;i<size;i++)
	{
		if(arr[i].second!=arr[i-1].second)
		{
			int ttot = tpos+tneg;
			int ltot = lpos+lneg;
			long double e1 = getEntropy(lpos,lneg);
			long double e2 = getEntropy(tpos-lpos,tneg-lneg);
			long double e = (ltot*1.000000/ttot)*e1 + e2*((ttot-ltot)*1.00000/ttot);
			long double ig = tentro - e;
			if(ig>igmax)
			{
				igmax = ig;
				mval = (arr[i].first+arr[i-1].first)/2;
			}
		}
		if(arr[i].second==false)
		{
			lneg++;
		}
		else
		{
			lpos++;
		}
	}
	return make_pair(igmax,mval);
}

long double Tree::splitDiscrete(Treenode* node,int ano, pair<int,int>& missing)
{
	int i,j,size;
	size = avals[ano].size();
	vector< pair<int,int> >vals;
	set<string>::iterator it = avals[ano].begin();
	set<string>::iterator ti = it;
	while(it!=avals[ano].end())
	{
		vals.push_back(make_pair(0,0));
		it++;
	}
	it=ti;
	size=node->data.size();
	int tneg=0;
	int tpos=0;
	vector<int> cntp(avals[ano].size(), 0), cntn(avals[ano].size(), 0);
	for(i=0;i<size;i++)
	{
		if(node->data[i].key[ano]=="?") continue;
		int ind = distance(it,avals[ano].find(node->data[i].key[ano]));
		if(node->data[i].val==false) cntn[ind]++;
		else cntp[ind]++;
	}
	int mx=0,idx=0;
	for(i=0;i<avals[ano].size();i++) if(mx<cntn[i]) mx=cntn[i],idx=i;
	missing.first=idx;
	mx=0,idx=0;
	for(i=0;i<avals[ano].size();i++) if(mx<cntp[i]) mx=cntp[i],idx=i;
	missing.second=idx;
	for(i=0;i<size;i++)
	{
		//cout<<*avals[ano].find(node->data[i].key[ano])<<endl;
		int ind;
		if(node->data[i].key[ano]=="?") ind=((node->data[i].val)?missing.second:missing.first);
		else ind=distance(it,avals[ano].find(node->data[i].key[ano]));
		if(node->data[i].val==false)
		{
			vals[ind].second++;
			tneg++;
		}
		else
		{
			vals[ind].first++;
			tpos++;
		}
	}
	long double tentro = getEntropy(tpos,tneg);
	size=avals[ano].size();
	long double ig=0;
	long double entro=0;
	for(i=0;i<size;i++)
	{
		entro+=getEntropy(vals[i].first,vals[i].second)*((vals[i].first+vals[i].second)*1.0000000/(tpos*1.00+tneg));		
	}
	ig = tentro-entro;
	return ig;
}

Treenode* Tree::getrootNode()
{
	return this->root;
}

void Tree::setrootNode()
{
	root = new Treenode();
	root->setParent(NULL);
	int i;
	for(i=0;i<atbno;i++)
	{
		root->aset.insert(i);
	}
	root->setData(this->tdata);
}

void Tree::makeTree(Treenode* node)
{
	if(node->pos==0 || node->neg==0 || node->aset.size()==0 || node->data.size()==0)
	{
		node->setAno(-1);
		return;
	}
	auto tmpaset=node->aset;
	if(rf){
		//In case the decision tree is a random forest, we take a random subset of attributes.
		int sz = node->aset.size();
		sz = int(sqrt(sz));
		vector<int> tmpv;
		for(auto qq: node->aset) tmpv.push_back(qq);
		random_shuffle(tmpv.begin(),tmpv.end());
		node->aset.clear();
		for(int i=0;i<sz;i++) node->aset.insert(tmpv[i]);
	}
	set<int>::iterator it = node->aset.begin();
	long double igmax = -1;
	int nt=0;
	int chosenOne = *it;
	int splitval=-1;
	bool ff = false;
	pair<int,int> tmpm;
	while(it!=node->aset.end())
	{
		if(cvals.size()>0 && cvals.find(*it)!=cvals.end())
		{
			//continuous valued attributes, calculating igmax for split.
			pair<long double,int> val = splitContinuous(node,*it, tmpm); //splitContinuous returns igmax
			long double ig = val.first;
			if(ig>=igmax)
			{
				ff=true;
				igmax = ig;
				chosenOne = *it;
				node->missing = tmpm;
				splitval=val.second;
			}
		}
		else
		{
			long double val = splitDiscrete(node,*it, tmpm);
			if(val>igmax)
			{
				node->missing = tmpm;
				ff=false;
				igmax = val;
				chosenOne = *it;
			}
		}
		it++;
		nt++;
	}
	//cout<<endl;
	//cout<<"Chosen "<<chosenOne<<endl;
	node->setAno(chosenOne);
	//cout<<node->getAno()<<endl;
	int i,j,size;
	int ano = node->getAno();
	node->aset = tmpaset;
	node->aset.erase(ano);
	if(rf && !(node->aset.empty()))
	{
		int sz = node->aset.size();
		sz = int(sqrt(sz+1));
		vector<int> tmpv;
		for(auto qq: node->aset) tmpv.push_back(qq);
		random_shuffle(tmpv.begin(),tmpv.end());
		node->aset.clear();
		for(int i=0;i<sz;i++) node->aset.insert(tmpv[i]);
	}
	size=node->data.size();
	it = node->aset.begin();
	if(cvals.size()>0 && cvals.find(ano)!=cvals.end())
	{
		node->children.resize(2);
		node->children[0] = new Treenode();
		node->children[1] = new Treenode();
		(node->children[0])->aset=node->aset;
		(node->children[1])->aset=node->aset;
		//cout<<"The: "<<splitval<<" "<<node->aset.size()<<" "<<node->data.size()<<" "<<ff<<endl;
		size=node->data.size();
		for(i=0;i<size;i++)
		{
			int tv;
			if(node->data[i].key[ano]=="?") tv=((node->data[i].val)?node->missing.second:node->missing.first);
			else tv = stringToInt(node->data[i].key[ano]);
			int ind=1;
			if(tv<=splitval)
			{
				ind=0;
			}
			(node->children[ind])->data.push_back(node->data[i]);
			if(node->data[i].val==false)
			{
				(node->children[ind])->neg++;
			}
			else
			{
				(node->children[ind])->pos++;
			}
		}
		(node->children[0])->cv=splitval;
		(node->children[1])->cv=splitval;
		//cout<<node->data.size()<<" "<<(node->children[0])->data.size()<<" "<<(node->children[1])->data.size()<<endl;
	}
	else
	{
		size=avals[ano].size();
		//cout<<size<<" "<<ano<<endl;
		node->children.resize(size);
		set<string>::iterator it = avals[ano].begin();
		for(i=0;i<size;i++)
		{
			node->children[i] = new Treenode();
			(node->children[i])->aset=node->aset;
		}
		size=node->data.size();
		//cout<<"These: "<<ano<<" "<<size<<endl;
		for(i=0;i<size;i++)
		{

			int ind; 
			if(node->data[i].key[ano]=="?") ind=((node->data[i].val)?node->missing.second:node->missing.first);
			else ind = distance(it,avals[ano].find(node->data[i].key[ano]));
			//cout<<ind<<endl;
			(node->children[ind])->data.push_back(node->data[i]);
			if(node->data[i].val==false)
			{
				(node->children[ind])->neg++;
			}
			else
			{
				(node->children[ind])->pos++;
			}
		}
		//cout<<"hello"<<endl;
	}
	//cout<<"That: "<<node->aset.size()<<endl;
	node->data.clear();
	node->aset.clear();
	size = node->children.size();
	for(i=0;i<size;i++)
	{
		makeTree(node->children[i]);
	}
}

void Tree::traverse(Treenode* node)
{
	if(node->getAno()==-1 || node->pos==0 || node->neg==0 )
	{
		if(node->pos<node->neg)
		{
			cout<<"No"<<endl;
		}
		else
		{
			cout<<"Yes"<<endl;
		}
		return;
	}
	int i,size;
	size=node->children.size();
	//cout<<node->getAno()<<" "<<node->pos<<" "<<node->neg<<endl;
	int ano = node->getAno();
	if(cvals.size()>0 && cvals.find(ano)!=cvals.end())
	{
		cout<<ano<<" "<<"<="<<intToString((node->children[0])->cv)<<endl;
		traverse(node->children[0]);
		cout<<ano<<" "<<">"<<intToString((node->children[1])->cv)<<endl;
		traverse(node->children[1]);
	}
	else
	{
		set<string>::iterator it = avals[node->getAno()].begin();
		for(i=0;i<size;i++)
		{
			cout<<node->getAno()<<" "<<*it<<endl;
			traverse(node->children[i]);
			it++;
		}
	}
}

bool Tree::predict(vector<string> &test, Treenode* node)
{
	if(node->getAno()==-1 || node->pos==0 || node->neg==0)
	{
		if(node->pos<node->neg)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	int ano = node->getAno();
	if(cvals.size()>0 && cvals.find(ano)!=cvals.end())
	{
		int cv = (node->children[0])->cv;
		int val;
		if(test[ano]=="?") val=((test[test.size()-1][0]=='>')?node->missing.second:node->missing.first);
		else val = stringToInt(test[ano]);
		if(val<=cv)
		{
			return predict(test,node->children[0]);
		}
		return predict(test,node->children[1]);
	}
	set<string>::iterator it = avals[ano].begin();
	int ind;
	if(test[ano]=="?") ind=((test[test.size()-1][0]=='>')?node->missing.second:node->missing.first);
	else ind = distance(it,avals[ano].find(test[ano]));
	return predict(test,node->children[ind]);
}

void Tree::runtest(string datafile)
{
	int i,j,size;
	size = datafile.length();
	//cout<<datafile<<endl;
	char* buff = new char[size+1];
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
	int ii=0;
	while(!ifil.eof())
	{
		getline(ifil,line);
		size = line.length();
		vector<string> vals;
		string temp="";
		queue<string> wq;
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				vals.push_back(temp);
				temp="";
				i++;
			}
			else
			{
				temp.push_back(line[i]);
			}
		}
		vals.push_back(temp);
		size=vals.size();
		bool cl = true;
		if(vals[size-1].compare("<=50K.")==0)
		{
			cl = false;
		}
		vals.pop_back();
		size--;
		bool ans = predict(vals,this->root);
		if(ans==cl)
		{
			correct++;
		}
		total++;
	}
	ifil.close();
	cout<<"Total data : "<<total<<", Correct : "<<correct<<endl;
	cout<<"Accuracy: "<<((correct*100.0)/total)<<"%"<<endl;
}

Tree::~Tree()
{
	this->tdata.clear();
	this->avals.clear();
	this->avals.clear();
	delete this->root;
}


#include "tree.h"
#include <fstream>
#include <queue>
#include <iterator>
#define ll long long int
using namespace std;

Tree::Tree()
{
	this->root = NULL;
	this->tdata.clear();
	iter=0;
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
	queue<string> wq;
	size = datafile.length();
	char* buff = new char[size+1];
	for(i=0;i<size;i++)
	{
		buff[i] = datafile[i];
	}
	buff[i]='\0';
	ifil.open(buff,fstream::in);
	string line;
	vector<ll> pcvals;
	vector<ll> ncvals;
	map<int, vector< pair<string,int> > > pdf;
	map<int, vector< pair<string,int> > > ndf;
	size = cvals.size();
	int npos=0;
	int nneg=0;
	for(i=0;i<size;i++)
	{
		pcvals.push_back(0);
		ncvals.push_back(0);
	}
	for(i=0;i<atbno;i++)
	{
		if(cvals.find(i)==cvals.end())
		{
			size = avals[i].size();
			pair<string,int> temp;
			set<string>::iterator it = avals[i].begin();
			while(it!=avals[i].end())
			{
				pdf[i].push_back(make_pair(*it,0));
				ndf[i].push_back(make_pair(*it,0));
				it++;
			}
		}
	}
	int lno=0;
	while(!ifil.eof())
	{
		lno++;
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
				if(temp.compare("?")==0)
				{
					wq.push(line);
					flag=true;
					temp="";
					break;
				}
				vals.push_back(temp);
				temp="";
				i++;
			}
			else
			{
				temp.push_back(line[i]);
			}
		}
		if(!flag)
		{
			vals.push_back(temp);
			size = vals.size();
			ds.key = vals;
			if(vals[size-1].compare("<=50K")==0)
			{
				ds.val=false;
			}
			else
			{
				ds.val=true;
			}
			for(i=0;i<size;i++)
			{
				if(cvals.find(i)==cvals.end())
				{
					string av = vals[i];
					int lls = avals[i].size();
					if(ds.val==false)
					{
						for(j=0;j<lls;j++)
						{
							if(ndf[i][j].first.compare(av)==0)
							{
								ndf[i][j].second++;
								break;
							}
						}
					}
					else
					{
						for(j=0;j<lls;j++)
						{
							if(pdf[i][j].first.compare(av)==0)
							{
								pdf[i][j].second++;
								break;
							}
						}
					}
				}
				else
				{
					int ind = distance(cvals.begin(),cvals.find(i));
					//cout<<ind<<endl;
					if(ds.val==false)
					{
						ncvals[ind]=ncvals[ind]+stringToInt(vals[i]);
						nneg++;
					}
					else
					{
						npos++;
						pcvals[ind]=pcvals[ind]+stringToInt(vals[i]);
					}
				}
			}
			tdata.push_back(ds);
		}
		//cout<<lno<<endl;
	}
	//cout<<"done"<<endl;
	delete[] buff;
	ifil.close();
	size = cvals.size();
	for(i=0;i<size;i++)
	{
		pcvals[i] = floor((pcvals[i]*1.00000/npos)+0.5);
		ncvals[i] = floor((ncvals[i]*1.00000/nneg)+0.5);
		//cout<<i<<" "<<pcvals[i]<<" "<<ncvals[i]<<endl;
	}
	for(i=0;i<size;i++)
	{
		if(cvals.find(i)==cvals.end())
		{
			sort(pdf[i].begin(),pdf[i].end(),compare2);
			sort(ndf[i].begin(),ndf[i].end(),compare2);
		}
	}
	for(i=0;i<size;i++)
	{
		if(cvals.find(i)==cvals.end())
		{
			int lls = avals[i].size();
			for(j=1;j<size;j++)
			{
				pdf[i][j].second=pdf[i][j-1].second+pdf[i][j].second;
				ndf[i][j].second=ndf[i][j-1].second+ndf[i][j].second;	
			}
		}
	}
	/*while(!wq.empty())
	{
		line = wq.front();
		wq.pop();
		size = line.length();
		vector<string> vals;
		string temp="";
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				j=i+1;
			}
		}
		bool flag = false;
		while(j<size)
		{
			temp.push_back(j);
			j++;
		}
		if(temp.compare(">50K.")==0)
		{
			flag = true;
		}
		temp="";
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				if(temp.compare("?")==0)
				{
					if(cvals.find(i)!=cvals.end())
					{
						if(flag)
						{
							temp=intToString(pcvals[i]);
						}
						else
						{
							temp=intToString(ncvals[i]);
						}
					}
					else
					{
						if(flag)
						{
							int lls = 
						}
						else
						{

						}
					}
				}
				vals.push_back(temp);
				temp="";
				i++;
			}
			else
			{
				temp.push_back(line[i]);
			}
		}
	}*/
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

pair<long double,int> Tree::splitContinuous(Treenode* node,int ano)
{
	int i,size;
	size=node->data.size();
	vector< pair<int,bool> > arr;
	pair<int,bool> temp;
	int tpos=0;
	int tneg=0;
	for(i=0;i<size;i++)
	{
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

long double Tree::splitDiscrete(Treenode* node,int ano)
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
	for(i=0;i<size;i++)
	{
		//cout<<*avals[ano].find(node->data[i].key[ano])<<endl;
		int ind = distance(it,avals[ano].find(node->data[i].key[ano]));
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
	iter++;
	if(node->pos==0 || node->neg==0 || node->aset.size()==0 || node->data.size()==0)
	{
		node->setAno(-1);
		return;
	}
	set<int>::iterator it = node->aset.begin();
	long double igmax = -1;
	int nt=0;
	int chosenOne = *it;
	int splitval=-1;
	bool ff = false;
	while(it!=node->aset.end())
	{
		//cout<<it->first<<endl;
		//cout<<*it<<" ";
		if(cvals.size()>0 && cvals.find(*it)!=cvals.end())
		{
			//cout<<"idhar"<<endl;
			//continuous valued attributes, calculating igmax for split.
			pair<long double,int> val = splitContinuous(node,*it); //splitContinuous returns igmax
			long double ig = val.first;
			//cout<<nt<<" "<<ig<<endl;
			if(ig>=igmax)
			{
				ff=true;
				igmax = ig;
				chosenOne = *it;
				splitval=val.second;
			}
		}
		else
		{
			//cout<<"udhar"<<endl;
			long double val = splitDiscrete(node,*it);
			if(val>igmax)
			{
				ff=false;
				igmax = val;
				chosenOne = *it;
			}
		}
		it++;
		nt++;
	}
	//cout<<endl;
	node->setAno(chosenOne);
	//cout<<node->getAno()<<endl;
	int i,j,size;
	int ano = node->getAno();
	node->aset.erase(ano);
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
			int tv = stringToInt(node->data[i].key[ano]);
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
			int ind = distance(it,avals[ano].find(node->data[i].key[ano]));
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
	set<string>::iterator it = avals[node->getAno()].begin();
	for(i=0;i<size;i++)
	{
		cout<<node->getAno()<<" "<<*it<<endl;
		traverse(node->children[i]);
		it++;
	}
}
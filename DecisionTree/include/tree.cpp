#include "tree.h"
#include <fstream>
#define ll long long int
using namespace std;

Tree::Tree()
{
	this->root = NULL;
	this->tdata.clear();
}

bool compare(pair<int,bool> p1,pair<int,bool> p2)
{
	return p1.first<p2.first;
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
		avals[ind].insert("?");
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
	int i,size;
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

int Tree::splitContinuous(Treenode* node,int ano)
{
	int i,size;
	size=tdata.size();
	vector< pair<int,bool> > arr;
	pair<int,bool> temp;
	int tpos=0;
	int tneg=0;
	cout<<"Idhar"<<endl;
	for(i=0;i<size;i++)
	{
		cout<<i<<endl;
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
	cout<<"IDhar"<<endl;
	sort(arr.begin(),arr.end(),compare);
	int lpos=0;
	int lneg=0;
	double tentro = getEntropy(tpos,tneg);
	size=arr.size();
	long double lentro=0;
	long double igmax=-1;
	int mval=0;
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
				mval = i;
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
	return mval;
}
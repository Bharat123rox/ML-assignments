#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <queue>
#include <cfloat>
#define ll long long int

using namespace std;
int fromString(string str)
{
	int size = str.length();
	int i=0;
	int ans = 0;
	int j=1;
	for(i=size-1;i>=0;i--)
	{
		int val = (int)str[i]-48;
		ans=ans + val*j;
		j = j*10;
	}
	return ans;
}

bool compare(pair<int,bool> p1,pair<int,bool> p2)
{
	return p1.first<p2.first;
}

long double getEntropy(int npos,int nneg)
{
	long double entro;
	long double ntotal = npos+nneg;
	if(npos==0 || nneg==0)
	{
		return 0;
	}
	entro = (npos*1.00000/ntotal)*log2(ntotal*1.000000/npos)+(nneg*1.00000/ntotal)*log2(ntotal*1.0000/nneg);
	return entro;
}

int main()
{
	ifstream ins;
	ins.open("/home/nitish/Documents/ML/ML-assignments/DecisionTree/raw_data/data.csv", std::fstream::in);
	string line;
	int noatr=14;
	int i;
	queue<string> wq;
	vector< pair<int,bool> >arr[6];
	int lkp[15];
	int ilkp[6];
	for(i=0;i<=14;i++)
	{
		lkp[i]=-1;
	}
	lkp[0]=0;
	lkp[2]=1;
	lkp[4]=2;
	lkp[10]=3;
	lkp[11]=4;
	lkp[12]=5;
	ilkp[0]=0;
	ilkp[1]=1;
	ilkp[2]=4;
	ilkp[3]=10;
	ilkp[4]=11;
	ilkp[5]=12;
	int size=0;
	int tpos=0;
	int tneg=0;
	int ttrain=0;
	while(!ins.eof())
	{
		getline(ins,line);
		//cout<<line<<endl;	
		size = line.length();
		if(size==1 && line[0]=='\n')
		{
			continue;
		}
		vector<string> vals;
		string temp="";
		bool flag = false;
		//cout<<size<<endl;
		for(i=0;i<size;i++)
		{
			if(line[i]==',')
			{
				if(temp.compare("?")==0)
				{
					wq.push(line);
					flag = true;
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
			bool fl = true;
			ttrain++;
			tpos++;
			if(vals[14].compare("<=50K")==0)
			{
				fl = false;
				tneg++;
				tpos--;
			}
			for(i=0;i<14;i++)
			{
				if(lkp[i]!=-1)
				{
					arr[lkp[i]].push_back(make_pair(fromString(vals[i]),fl));
				}
			}
		}
	}

	long double tentro=getEntropy(tpos,tneg);
	long double 
	vector<long double> unif_vals;
	for(i=0;i<6;i++)
	{
		int j;
		//cout<<i<<endl;
		size = arr[i].size();
		sort(arr[i].begin(),arr[i].end(),compare);
		int lpos=0;
		int lneg=0;
		int lentro=0;
		long double igmax=-1;
		long double mval=DBL_MIN;
		if(arr[i][0].second==false)
		{
			lneg++;
		}
		else
		{
			lpos++;
		}
		for(j=1;j<size;j++)
		{
			if(arr[i][j].second!=arr[i][j-1].second)
			{
				int ltot = lneg+lpos;
				int lrem = ttrain - ltot;
				long double e1 = getEntropy(lpos,lneg)*((ltot*1.00000)*1.0000/ttrain);
				long double e2 = getEntropy(tpos-lpos,tneg-lneg)*((lrem*1.00000)*1.0000/ttrain);
				double e = e1+e2;
				long double ig = (tentro-e);
				if(ig>igmax)
				{
					igmax = ig;
					mval=(arr[i][j].first*1.0000+arr[i][j-1].first)*1.000/2;
				}
			}
			if(arr[i][j].second==false)
			{
				lneg++;
			}
			else
			{
				lpos++;
			}
		}
		unif_vals.push_back(mval);
	}
	ins.close();
	return 0;
}
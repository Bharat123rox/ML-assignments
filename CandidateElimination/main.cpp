#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define Q 1000000007
#define NL -Q

class Reader
{
	public:
	void printv(vector<int> v)
	{
		//Prints vector.
		for(int i=0;i<v.size();i++){
			if(v[i]==Q) cout<<"all ";
			else if(v[i]==NL) cout<<"NULL ";
			else cout<<v[i]<<' ';
		}
		cout<<endl;
	}
	void printvov(vector< vector<int> > v)
	{
		//Prints vector of vector.
		for(int i=0;i<v.size();i++) printv(v[i]);
	}
	vector< vector<int> > data;	//Stores data read from the file.
	private:
	void read(string str)
	{
		//Reads the data and stores it. str is file name where data is stored.
		string row,item;
		fstream inp(str,std::fstream::in);
		if(!inp){
			cout<<"Failed to open!";
			return;
		}
		getline(inp,row);
		while(getline(inp,row)){
			stringstream ss(row);
			vector<int> res;
			getline(ss,item,',');
			while(getline(ss,item,',')){
				stringstream s(item);
				int r=0;
				s>>r;
				res.push_back(r);
		    }
			data.push_back(res);
		    res.clear();
		}
		inp.close();
	}
	public:
	Reader(string str)
	{
		read(str);
	}
};

class CandidateElimination
{
	vector< vector<int> > gb;		//General boundary
	vector<int> sb,q,nl;			//Specific boundary, most general hypothesis, most specific hypothesis, repectively.
	vector< vector<int> > data;	    //Data for the algo.
	int target;						//Value of the target concept.
	vector<vector<int> > vals;		//Value which each attribute can take.
	bool general(vector<int> &v1,vector<int> &v2)
	{ 
		//Returns true if v1 is strictly general than v2, false otherwise
		for(int i=0;i<v1.size();i++) if(((v1[i]!=Q&&v1[i]!=NL&&v1[i]!=v2[i]&&v2[i]!=NL)||(v1[i]==NL&&v2[i]!=NL))) return false;
		return true;
	}
	bool specific(vector<int> &v1,vector<int> &v2)
	{ 
		//Returns true if v1 is strictly general than v2, false otherwise
		return general(v2,v1);
	}
	vector<int> mingen(vector<int>& v1, vector<int>& v2)
	{
		//Returns minimum generalization of v1 which satifies v2.
		vector<int> ret;
		for(int i=0;i<v1.size();i++){
			if(v1[i]==NL) ret.push_back(v2[i]);
			else if(v1[i]==v2[i]) ret.push_back(v1[i]);
			else ret.push_back(Q); 
		}
		return ret;
	}
	vector< vector<int> > minspec(vector<int>& v1, vector<int>& v2)
	{
		//Returns minimum specialization of v1 which does not satify v2.
		vector< vector<int> > ret;
		vector<int> tmp=v1;
		if(v1==v2) return ret;
		for(int i=0;i<v1.size();i++){
			if(tmp[i]==Q){
				for(int val:vals[i]) if(val!=v2[i]){
					tmp[i]=val;
					ret.push_back(tmp);
				}
			}
			else{
				tmp[i]=NL;
				ret.push_back(tmp);
			}
			tmp[i]=v1[i];
		}
		return ret;
	}
	void compute_boundaries()
	{
		//Computes gb and sb. Follows candidate elimination algorithm, (Refer Machine Learning, Tom Mitchell) 
		for(auto td:data){
			int tar=td[td.size()-1];
			auto hyp=td;
			hyp.pop_back();
			if(tar==target){
				map<int,int> del;
				auto tmp=gb;
				tmp.clear();
				for(int i=0;i<gb.size();i++) if(!general(gb[i],hyp)) del[i]=1;
				for(int i=0;i<gb.size();i++) if(del[i]!=1) tmp.push_back(gb[i]);
				gb=tmp;
				if(gb.empty()){
					sb.clear();
					return;
				}
				if(!general(sb,hyp)) sb=mingen(sb,hyp);
				bool flag=false;
				for(auto gbi:gb) if(general(gbi,sb)) flag=true;
				if(!flag){
					gb.clear();
					sb.clear();
					return;
				}
			}
			else{
				if(general(sb,hyp)){
					gb.clear();
					sb.clear();
					return;
				}
				auto tmp=gb;
				gb.clear();
				for(auto gbi:tmp){
					if(general(gbi,hyp)){
						auto specs=minspec(gbi,hyp);
						for(auto h:specs) if(specific(sb,h)) gb.push_back(h);
					}
					else if(specific(sb,gbi)) gb.push_back(gbi);
				}
				tmp=gb;
				gb.clear();
				for(int i=0;i<tmp.size();i++){
					bool flag=true;
					for(int j=0;j<tmp.size();j++){
						if(i==j) continue;
						if(general(tmp[j],tmp[i])) flag=false;
					}
					if(flag) gb.push_back(tmp[i]);
				}
			}
		}
		if(gb.empty()||sb.empty()) gb.clear(),sb.clear();
		return;
	}
	public:
	CandidateElimination(Reader r,int t)
	{
		data=r.data;
		if(data.empty()) return;
		q.resize(data[0].size()-1,Q);
		nl.resize(data[0].size()-1,NL);
		gb.push_back(q);
		sb=nl;
		target=t;
		//Calculate possible values for each attribute.
		for(int i=0;i<data[0].size()-1;i++){
			set<int> stmp;
			for(int j=0;j<data.size();j++) stmp.insert(data[j][i]);
			vector<int> vtmp;
			for(int stp:stmp) vtmp.push_back(stp);
			vals.push_back(vtmp);
		}
		compute_boundaries();
		if(gb.empty()) cout<<"Target hypothesis doesn't exist.\n";
		else{
			cout<<"Specific Boundary:\n";
			r.printv(sb);
			cout<<"General Boundary:\n";
			r.printvov(gb);
			cout<<"\n";
		}
	}
};

int main()
{
	Reader r("data.csv");
	for(int i=1;i<8;i++){
		cout<<"Target: "<<i<<endl;
		CandidateElimination c(r,i);
	}
    return 0;
}
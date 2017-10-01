#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define Q 1000000007
#define NL -Q

/*!
   \class Reader
 * This class reads input from the data file and stores it into data
   \sa printv(),printvov(),read(),data
 */
class Reader
{
	public:
	/*! 
	 * Prints a vector according to hypothesis format.
	   \param v The vector to be printed.
	   \sa printvov()
	 */
	void printv(vector<int> v)
	{
		for(int i=0;i<v.size();i++){
			if(v[i]==Q) cout<<"all ";
			else if(v[i]==NL) cout<<"NULL ";
			else cout<<v[i]<<' ';
		}
		cout<<endl;
	}
	/*! 
	 * Prints a vector of vectors according to hypothesis format.
	   \param v The vector of vectors.
	   \sa printv()
	*/
	void printvov(vector< vector<int> > v)
	{   
		for(int i=0;i<v.size();i++) printv(v[i]);
	}
	//! Stores data read from the file.
	vector< vector<int> > data;	
	private:
	/*!	
		\fn read() 
	 * Reads the data and stores it into the variable data.
	 * Furthermore, the target values are also stored in the variable data.  
	    \param str File name where data is stored.
	    \sa Reader()
	*/
	void read(string str)
	{
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
	/*! Constructor for the class Reader()
		\param str File name is given here.
		\sa read() 
	*/
	Reader(string str)
	{
		read(str);
	}
};

/*! 
	\class CandidateElimination	
 *  The class which performs the actual Candidate Elimination Algorithm.
 	\sa general(), specific(), mingen(), minspec(), compute_boundaries()
 */
class CandidateElimination
{
	vector< vector<int> > gb;		///<General Boundary
	vector<int> sb;					///<Specific Boundary
	vector<int> q;					///<Most general hypothesis
	vector<int> nl;					///<Most specific hypothesis
	vector< vector<int> > data;	    ///<Data for the algo.
	int target;						///<Value of the target concept.
	vector<vector<int> > vals;		///<Value which each attribute can take.
	/*!
		\fn general()
	 *  Compares two hypotheses to see which is more general.	
	   \param 	v1 First hypothesis to be compared.
	   \param   v2 Second hypothesis to be compared. 
	   \return true if v1 is strictly general than v2, False otherwise.
	 */
	bool general(vector<int> &v1,vector<int> &v2)
	{ 
		for(int i=0;i<v1.size();i++) if(((v1[i]!=Q&&v1[i]!=NL&&v1[i]!=v2[i]&&v2[i]!=NL)||(v1[i]==NL&&v2[i]!=NL))) return false;
		return true;
	}
	/*!
		\fn specific()
	 * Compares two hypotheses to see which is more specific.
	    \param 	 v1  First hypothesis to be compared.
	    \param   v2  Second hypothesis to be compared.
	 	\return true if v1 is strictly specific than v2, false otherwise
	 	\sa general()
	 */	
	bool specific(vector<int> &v1,vector<int> &v2)
	{ 
		return general(v2,v1);
	}
	/*!
	    \fn mingen()
	 * Function to check Minimum Generalization.
	 	\param v1 Hypothesis for which minimum generalization is required
	 	\param v2 Concept to be satisfied. 
		\return Minimum generalization of v1 which satifies v2.
	 */	
	vector<int> mingen(vector<int>& v1, vector<int>& v2)
	{
		vector<int> ret;
		for(int i=0;i<v1.size();i++){
			if(v1[i]==NL) ret.push_back(v2[i]);
			else if(v1[i]==v2[i]) ret.push_back(v1[i]);
			else ret.push_back(Q); 
		}
		return ret;
	}
	/*!
	    \fn minspec()
	 *  Function to check Minimum Specialization. 
	 	\param v1 Hypothesis for which minimum specialization is required.
	 	\param v2 Concept to be checked against.
	 	\return Minimum specialization of v1 which does not satify v2.
	 */	
	vector< vector<int> > minspec(vector<int>& v1, vector<int>& v2)
	{
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
	/*!
	\fn compute_boundaries()
	 * Computes General Boundary and Specific Boundary. 
	 * Follows candidate elimination algorithm, (Refer Machine Learning, Tom Mitchell) 
	 */
	void compute_boundaries()
	{
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
		if(gb.empty()||sb.empty()) {gb.clear();sb.clear();}
		return;
	}
	public:

	/*
	 \brief Constructor for the Candidate Elimination Class.
	 \param r The filename
	 \param t The target number (from 1-7)
	 \sa Reader()
	 */
	CandidateElimination(Reader r,int t)
	{
		data=r.data;
		if(data.empty()) return;
		q.resize(data[0].size()-1,Q);
		nl.resize(data[0].size()-1,NL);
		gb.push_back(q);
		sb=nl;
		target=t;
		/*!
		 * Calculate ALL possible values for each attribute.
		 */
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

/*!
   \fn main()
   \brief Execute the Candidate Elimination Algorithm by calling the CandidateElimination Class.
   \sa CandidateElimination(),Reader()
 */	
int main()
{
	Reader r("data.csv");
	for(int i=1;i<8;i++){
		cout<<"Target: "<<i<<endl;
		CandidateElimination c(r,i);
	}
    return 0;
}
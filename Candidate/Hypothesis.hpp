#include <bits/stdc++.h>
#include "const.h"
#define ll long long int
class Hypothesis{
	//Defines a hypothesis of the form <1,0,null,1,0,null,1,1,1,q,q,0,4,q,0,1> for example
	ll N;
	std::vector<int> values;
	public:
		Hypothesis();
		Hypothesis(std::vector<int> v);
		std::vector<int> getHypothesis();
		bool is_general_than(std::vector<int> &v1,std::vector<int> &v2);
		bool is_specific_than(std::vector<int> &v1,std::vector<int> &v2);
};
Hypothesis::Hypothesis(){;} 							//Default Constructor, in case things mess up
Hypothesis::Hypothesis(std::vector<int> v){
	values=v;	
}
std::vector<int> Hypothesis::getHypothesis(){ 			
	//Getter Function
	return this->values;
}
bool Hypothesis::is_general_than(std::vector<int> &v1, std::vector<int> &v2){ 
	//Returns true if strictly general than, false otherwise
	int flag=0,ct=0,i;
	for(i=0;i<v1.size();i++){
		if((v2[i]==q&&v1[i]<q)||(v2[i]>null&&v1[i]==null)){
			flag=1; break;
		}
		else if(v2[i]!=v1[i]){
			flag=1; break;
		}
		if((v1[i]==q&&v2[i]<q)||(v1[i]>null&&v2[i]==null)){
			++ct;
		}	
	}
	if(!ct) flag=1;
	if(flag) return false;
	else return true;
}
bool Hypothesis::is_specific_than(std::vector<int> &v1, std::vector<int> &v2) {
	//Returns true if strictly specific than, false otherwise
	int flag=0,ct=0,i;
	for(i=0;i<v1.size();i++){
		if((v1[i]==q&&v2[i]<q)||(v1[i]>null&&v2[i]==null)){
			flag=1; break;
		}
		else if(v1[i]!=v2[i]){
			flag=1; break;
		}
		if((v2[i]==q&&v1[i]<q)||(v2[i]>null&&v1[i]==null)){
			++ct;
		}	
	}
	if(!ct) flag=1;
	if(flag) return false;
	else return true;
}
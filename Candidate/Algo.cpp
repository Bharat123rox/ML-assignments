#include <bits/stdc++.h>
#include "Hypothesis.hpp"
#include "reader.hpp"
#include "const.h"
using namespace std;
vector<vector<int> > gb[8]; //General Boundary where gb[i] is the General Bound for Target Class i [1-7]
vector<vector<int> > sb[8]; //Specific Boundary where sb[i] is the Specific Bound for Target Class i [1-7]
int main(int argc, char const *argv[])
{
	int i;
	for(i=1;i<=7;i++){
		gb[i].push_back(vector<int> (16,q));	
		sb[i].push_back(vector<int> (16,null));
	}
	for(i=1;i<=7;i++){
		//Algorithm begins here
			
	}
	return 0;
}
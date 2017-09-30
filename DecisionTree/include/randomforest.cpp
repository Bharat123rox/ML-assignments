#include "randomforest.h"

using namespace std;

RandomForest::RandomForest(string trainingdata, string domknow)
{
	Tree* t = new Tree();
	t->loadDomainKnowledge(domknow);
	t->loadTrainingData(trainingdata, t->atbno);
	this->tdata = t->tdata;
	this->atbno = t->atbno;
	this->cvals = t->cvals;
	this->avals = t->avals;
	delete t;
	build();
}

void RandomForest::build()
{
	int sz = tdata.size(),totsz;
	totsz = sz;
	sz = int(sqrt(sz+1));
	this->trees.resize(sz);
	srand(time(NULL));
	for(int i=0;i<sz;i++){
		trees[i] = new Tree();
		trees[i]->atbno = this->atbno;
		trees[i]->cvals = this->cvals;
		trees[i]->avals = this->avals;
		for(int j=0;j<sz;j++){
			int idx = rand()%totsz;
			trees[i]->tdata.push_back(this->tdata[idx]);
		}
		//trees[i]->rf = true;
		trees[i]->setrootNode();
		trees[i]->makeTree(trees[i]->getrootNode());
	} 
}

void RandomForest::runtest(string datafile)
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
			size=vals.size();
			bool cl = true;
			if(vals[size-1].compare("<=50K.")==0)
			{
				cl = false;
			}
			vals.pop_back();
			size--;
			int sz = this->trees.size(), pp=0, nn=0;
			for(auto tr:trees){
				bool val = tr->predict(vals,tr->root);
				if(val) pp++;
				else nn++;
			}
			if((pp>=nn)==cl)
			{
				correct++;
			}
			total++;
		}
	}
	ifil.close();
	cout<<"Total data : "<<total<<", Correct : "<<correct<<endl;
	cout<<"Accuracy: "<<((correct*1.0)/total)<<endl;
}

#include "tree.cpp"
#define ll long long int

using namespace std;

int main()
{
	Tree tree;
	tree.loadDomainKnowledge("/home/nitish/Documents/ML/ML-assignments/DecisionTree/data/description.txt");
	//cout<<tree.atbno<<endl;
	tree.loadTrainingData("/home/nitish/Documents/ML/ML-assignments/DecisionTree/raw_data/data.csv",tree.atbno);
	int i,size=0;
	//std::set<int>::iterator it = tree.cvals.begin();
	//Treenode* root = new Treenode();
	//root->data = tree.tdata;
	/*while(it!=tree.cvals.end())
	{
		cout<<*it<<" ";
		cout<<tree.splitContinuous(root,*it).first<<endl;
		it++;
	}*/
	tree.setrootNode();
	tree.printData();
	//tree.printinfo();
	tree.makeTree(tree.getrootNode());
	tree.traverse(tree.getrootNode());
	return 0;
}
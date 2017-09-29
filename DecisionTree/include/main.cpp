#include "pruning.cpp"
#define ll long long int

using namespace std;

int main()
{
	Tree tree;
	tree.loadDomainKnowledge("C:/Users/vThejas/Documents/Github/ML-assignments/DecisionTree/data/description.txt");
	tree.loadTrainingData("C:/Users/vThejas/Documents/Github/ML-assignments/DecisionTree/raw_data/data.csv",tree.atbno);
	int i,size=0;
	tree.setrootNode();
	//tree.printData();
	tree.makeTree(tree.getrootNode());
	cout<<"sfggff\n";
	//tree.traverse(tree.getrootNode());
	tree.runtest("C:/Users/vThejas/Documents/Github/ML-assignments/DecisionTree/raw_data/testdata.csv");
	PrunedTree* pq=new PrunedTree(tree, "C:/Users/vThejas/Documents/Github/ML-assignments/DecisionTree/raw_data/testdata.csv");
	pq->tree.runtest("C:/Users/vThejas/Documents/Github/ML-assignments/DecisionTree/raw_data/testdata.csv");
	return 0;
}
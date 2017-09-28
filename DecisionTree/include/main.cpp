#include "tree.cpp"
#define ll long long int

using namespace std;

int main()
{
	Tree tree;
	tree.loadDomainKnowledge("/home/nitish/Documents/ML/ML-assignments/DecisionTree/data/description.txt");
	tree.loadTrainingData("/home/nitish/Documents/ML/ML-assignments/DecisionTree/raw_data/data.csv",tree.atbno);
	int i,size=0;
	tree.setrootNode();
	tree.printData();
	tree.makeTree(tree.getrootNode());
	tree.traverse(tree.getrootNode());
	tree.runtest("/home/nitish/Documents/ML/ML-assignments/DecisionTree/raw_data/testdata.csv");
	return 0;
}
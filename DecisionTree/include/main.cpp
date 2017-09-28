#include "tree.cpp"
#define ll long long int

using namespace std;

int main()
{
	Tree tree;
	tree.loadDomainKnowledge("/home/nitish/Documents/ML/ML-assignments/DecisionTree/data/sample2.txt");
	tree.loadTrainingData("/home/nitish/Documents/ML/ML-assignments/DecisionTree/data/sample1.txt",tree.atbno);
	int i,size=0;
	tree.setrootNode();
	tree.printData();
	tree.makeTree(tree.getrootNode());
	tree.traverse(tree.getrootNode());
	vector<string> val;
	val.push_back("Overcast");
	val.push_back("Hot");
	val.push_back("Normal");
	val.push_back("Strong");
	cout<<tree.predict(val,tree.getrootNode())<<endl;
	return 0;
}
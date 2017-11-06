#include <bits/stdc++.h>

using namespace std;

class NaiveBayesClassifier
{
	private:
	
	struct instance
	{
		map<int, int> words;
		bool sentiment;
	};

	double p_prob, n_prob;
	vector<double> p_prior, n_prior;
	vector<instance> p_data, n_data, tdata;
	map<int, string> vocab;
	map<int, int> p_wc, n_wc;

	vector<instance> read_data(const string&);
	void train();
	pair<bool, bool> predict(const instance&);

	public:

	NaiveBayesClassifier(const string&, const string&, int);
	void evaluate(const string&);
};

vector<NaiveBayesClassifier::instance> NaiveBayesClassifier::read_data(const string& fl)
{
	vector<instance> ret;
	ifstream in;
	in.open(fl.c_str());
	string tmp;
	in >> tmp;
	while(true){
		instance inst;
		inst.sentiment = (atoi(tmp.c_str()) >= 5) ? true : false;
		while(true){	
			if(!(in >> tmp)){
				tmp = "";
				break;
			}
			int pos = tmp.find(":"); 
			if(pos == string::npos) break;
			int val = atoi(tmp.substr(0, pos).c_str());
			int cnt = atoi(tmp.substr(pos + 1).c_str());
			inst.words[val] = cnt;
		}
		ret.push_back(inst);
		if(tmp == "") break;
	}
	return ret;
}

void NaiveBayesClassifier::train()
{
	p_prob = (1.0*p_data.size()) / (p_data.size() + n_data.size());
	n_prob = (1.0*n_data.size()) / (p_data.size() + n_data.size());
	int tot_words = 0;
	p_prior.resize(vocab.size(), 1.0);
	for(auto inst : p_data) for(auto i : inst.words) tot_words += i.second;
	for(int i = 0; i < vocab.size(); i++) 
		if(p_wc.find(i) != p_wc.end()) p_prior[i] = (p_wc[i] + 1) / (1.0 * (tot_words + vocab.size()));	
	tot_words = 0;
	n_prior.resize(vocab.size(), 1.0);
	for(auto inst : n_data) for(auto i : inst.words) tot_words += i.second;
	for(int i = 0; i < vocab.size(); i++) 
		if(n_wc.find(i) != n_wc.end()) n_prior[i] = (n_wc[i] + 1) / (1.0 * (tot_words + vocab.size()));
	return;
}

NaiveBayesClassifier::NaiveBayesClassifier(const string& tr_data_file, const string& vocab_file, int mode = 0)
{
	ifstream in; 
	in.open(vocab_file.c_str());
	string tmp;
	int cur = 0;
	while(in >> tmp) vocab[cur++] = tmp;
	in.close();
	auto data = read_data(tr_data_file);
	for(auto inst : data){
		if(inst.sentiment){
			p_data.push_back(inst);
			for(auto i : inst.words) p_wc[i.first] += i.second;
		}
		else{
			n_data.push_back(inst);
			for(auto i : inst.words) n_wc[i.first] += i.second;
		}
	}
	data.clear();
	train();
}

pair<bool, bool> NaiveBayesClassifier::predict(const NaiveBayesClassifier::instance& inst)
{
	double pos = log(p_prob), neg = log(n_prob);
	for(auto i : inst.words){
		pos += i.second * log(p_prior[i.first]);
	}
	for(auto i : inst.words) neg += i.second * log(n_prior[i.first]);
	return make_pair(pos >= neg, inst.sentiment);
}

void NaiveBayesClassifier::evaluate(const string& fl)
{
	auto data = read_data(fl);
	int tot = 0;
	for(auto inst : data){
		auto ret = predict(inst);
		if(ret.first == ret.second) tot++;
	}
	cout<< "Accuracy: " << (1.0*tot)/data.size() <<endl;
	return;
}

int main()
{
	NaiveBayesClassifier n("train.txt", "vocab.txt");
	n.evaluate("test.txt");
	return 0;
}
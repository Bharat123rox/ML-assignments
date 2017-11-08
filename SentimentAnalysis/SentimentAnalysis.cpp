#include <bits/stdc++.h>

using namespace std;

/*!
 \class NaiveBayesClassifier
 \brief Represents the implementation of a Naive Bayes Classifier template/blueprint.
 \sa instance, words,sentiment, read_data(), train(), predict(), evaluate()
*/
class NaiveBayesClassifier
{
	private:
	
	/*!
	 \struct instance
	 \brief Represents a document as an instance in the dataset, converting it to a \'bag-of-words\' model.
	 \sa words, sentiment 
	 */

	struct instance
	{
		/*!
		 \var words
		 Represents a word index and its frequency in the document, according to the bag-of-words model.
		 \sa sentiment
		 */

		map<int, int> words;
		
		/*!
		\var sentiment
		 Contains information whether a positive or negative sentiment has been expressed by the document.
		 \sa words
		 */

		bool sentiment;
	};
	/*!
	 \var p_prob
	 Probability of positives. 
	 \sa n_prob
	 */
	/*!
	 \var n_prob
	 Probability of negatives.
	 \sa p_prob 
	 */
	double p_prob, n_prob;
	/*!
	 \var p_prior
	 Prior probability of positive sentiment given the word. 
	 \sa n_prior
	 */
	/*!
	 \var n_prior
	 Prior probability of negative sentiment given the word.
	 \sa p_prior
	 */
	vector<double> p_prior, n_prior;
	vector<instance> p_data, n_data, tdata;
	/*!
	 \var vocab
	 Represents all distinct set of words/vocabulary of the entire dataset.
	 */
	map<int, string> vocab;
	/*!
	 \var p_wc
	 Wordcount of positive words whose probabilities express positive sentiment in the document.
	 \sa n_wc
	 */
	/*!
	 \var n_wc
	 Wordcount of positive words whose probabilities express negative sentiment in the document.
	 \sa p_wc
	 */
	map<int, int> p_wc, n_wc;
	set<int> stopwords;
	bool mode;

	/*!
	\fn read_data()
	\brief Takes in a text file as input, parses it and converts it into a \'bag-of-words\' form.
	\param fl The input text file
	\param binarize If true, Binary Naive Bayes is implemented, else, only Naive Bayes is implemented.
	\return The instance as a bag-of-words converted from the document.
	\sa train(), predict(), evaluate()
	 */

	vector<instance> read_data(const string&);

	/*!
	\fn train()
	\brief Trains the Naive Bayes Classifier by calculating prior probabilities with the help of a given training text file.
	\sa read_data(), predict(), evaluate()
	 */

	void train();	

	/*!
	\fn predict()
	\brief Tries to predict the sentiment of a document given a test document or test instance.
	\param inst A bag-of-words instance generated from a document for testing.
	\return A tuple of the form (Predicted Sentiment, Actual Sentiment)
	\sa train(), read_data(), evaluate()
	 */

	pair<bool, bool> predict(const instance&);

	public:

    /*!
     \fn NaiveBayesClassifier()
     \param tr_data_file The text file consisting of training documents from the dataset.
     \param vocab_file The vocabulary file consisting of all distinct words in the dataset. 
     \param mode (Non binary (false) or binary (true) Naive Bayes Algorithm)
     \sa read_data(), train(), predict(), evaluate()
     */

	NaiveBayesClassifier(const string&, const string&, bool, const string&);

	/*!
	\fn evaluate()
	\brief Evaluates the performance of the NaiveBayesClassifier by F-Score and Accuracy metrics.
	\param fl Test dataset/Test documents file with which the algorithm's performance is tested. 
	 */

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
			inst.words[val] = (!mode) ? cnt : 1;
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
	for(auto inst : p_data) for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) tot_words += i.second;
	for(int i = 0; i < vocab.size(); i++) if(stopwords.find(i) == stopwords.end()) p_prior[i] = (p_wc[i] + 1) / (1.0 * (tot_words + vocab.size()));	
	tot_words = 0;
	n_prior.resize(vocab.size(), 1.0);
	for(auto inst : n_data) for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) tot_words += i.second;
	for(int i = 0; i < vocab.size(); i++) if(stopwords.find(i) == stopwords.end()) n_prior[i] = (n_wc[i] + 1) / (1.0 * (tot_words + vocab.size()));
	return;
}

NaiveBayesClassifier::NaiveBayesClassifier(const string& tr_data_file, const string& vocab_file, bool mode = false, const string& sw = "")
{
	ifstream in; 
	in.open(vocab_file.c_str());
	string tmp;
	this -> mode = mode;
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
	for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) pos += i.second * log(p_prior[i.first]);
	for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) neg += i.second * log(n_prior[i.first]);
	return make_pair(pos >= neg, inst.sentiment);
}

void NaiveBayesClassifier::evaluate(const string& fl)
{
	auto data = read_data(fl);
	int tot = 0,tp = 0,tn = 0,fn = 0,fp = 0;
	for(auto inst : data){
		auto ret = predict(inst);
		if(ret.first == ret.second) {
			tot++;
			if(ret.first) tp++;
			else tn++;
		}
		else {
			if(ret.first) fp++;
			else fn++;
		}
	}
	double precision = (1.0*tp)/(tp+fp), recall = (1.0*tp)/(tp+fn);
	cout<< "Accuracy: " << (1.0*tot)/data.size() <<endl;
	cout<< "Precision: " << precision <<endl;
	cout<< "Recall: " << recall <<endl;
	cout<< "F-Score: " << (2.0*precision*recall)/(precision+recall) <<endl;
	return;
}

int main()
{
	NaiveBayesClassifier n("train.txt", "vocab.txt",true);
	n.evaluate("test.txt");
	return 0;
}
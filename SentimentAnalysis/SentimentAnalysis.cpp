include <bits/stdc++.h>

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
	\var invmap
	Inverse mapping from a stopword to the index of the stopword, which is then marked as an invalid index.
	*/ 
	map<string,int> invmap;
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
	/*!
	\var stopwords
	Set of all stopwords not to be considered by the Naive Bayes (or Binarized) Classifier
	\sa mode
	*/
	set<int> stopwords;
	/*!
	\var mode
	Mode which decides whether to implement Naive Bayes Classifier in binarized or non-binarized mode.
	\sa stopwords
	*/
	bool mode;

	/*!
	\fn read_data()
	\brief Takes in a text file as input, parses it and converts it into a \'bag-of-words\' form.
	\param fl The input text file
	\return The instance as a bag-of-words converted from the document.
	\sa train(), predict(), evaluate(), getStats()
	 */

	vector<instance> read_data(const string&);	

	/*!
	\fn predict()
	\brief Tries to predict the sentiment of a document given a test document or test instance.
	\param inst A bag-of-words instance generated from a document for testing.
	\return A tuple of the form (Predicted Sentiment, Actual Sentiment)
	\sa train(), read_data(), evaluate(), getStats()
	 */

	pair<bool, bool> predict(const instance&);

	/*!
	\fn train()
	\brief Trains the Naive Bayes Classifier by calculating prior probabilities with the help of a given training text file.
	\sa read_data(), predict(), evaluate(), getStats()
	 */

	void train();

	public:

    /*!
     \fn NaiveBayesClassifier()
     Constructor for the Naive Bayes Classifier.
     \param tr_data_file The text file consisting of training documents from the dataset.
     \param vocab_file The vocabulary file consisting of all distinct words in the dataset. 
     \param mode (If false, Non-Binary or if true, Binary Naive Bayes Algorithm)
     \param sw File consisting of stopwords.
     \sa read_data(), train(), predict(), evaluate()
     */

	NaiveBayesClassifier(const string&, const string&, bool, const string&);

	/*!
     \fn NaiveBayesClassifier()
     \param tr_data_file The text file consisting of training documents from the dataset.
     \param vocab_file The vocabulary file consisting of all distinct words in the dataset. 
     \param mode (If false, Non-Binary or if true, Binary Naive Bayes Algorithm)
     \param sw File consisting of stopwords.
     \sa read_data(), train(), predict(), evaluate(), getStats()
     */
	NaiveBayesClassifier(NaiveBayesClassifier*, vector<instance>&, bool);

	/*!
	\fn evaluate()
	\brief Evaluates the performance of the NaiveBayesClassifier by F-Score and Accuracy metrics.
	\param par The parent constructor.
	\param data The bag-of-words data
	\param mode False if Non-Binary, True if Binary Naive Bayes Classifier
	\sa read_data(), train(), predict(), getStats()
	 */

	double evaluate(const string&, bool);

	/*!
	\fn getStats()
	\brief Fetches statistics such as running time and accuracy of the program.
	\param acc_file The file in which accuracy for various sizes of the training set is stored.
	\param time_file The file in which time taken to run the program for various input sizes is stored.
	\param tr_data_file Text file with training documents from the dataset.
	\sa read_data(), train(), predict(), evaluate()
	 */

	void getStats(const string&, const string&, const string& );
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
	string tmp;
	ifstream in;
	in.open(vocab_file.c_str());
	this -> mode = mode;
	int cur = 0;
	while(in >> tmp) 
	{	
		vocab[cur] = tmp;
		invmap[tmp] = cur;
		cur++;
	}
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
	if(sw.size()>1)
	{
		in.open(sw.c_str());
		while(!in.eof())
		{
			in>>tmp;
			if(invmap.find(tmp)!=invmap.end())
			{
				stopwords.insert(invmap[tmp]);
			}
		}
		in.close();
	}
	train();
}

NaiveBayesClassifier::NaiveBayesClassifier(NaiveBayesClassifier* par, vector<instance>& data, bool mode = false)
{
	this->vocab = par->vocab;
	this->invmap = par->invmap;
	this->mode = mode;
	for(auto inst : data)
	{
		if(inst.sentiment)
		{
			p_data.push_back(inst);
			for(auto i : inst.words) 
			{
				if(this->mode==false)
				{	
					p_wc[i.first] += i.second;
				}
				else
				{
					p_wc[i.first]+=1;
				}
			}
		}
		else{
			n_data.push_back(inst);
			for(auto i : inst.words) 
			{
				if(this->mode==false)
				{
					n_wc[i.first] += i.second;
				}
				else
				{
					n_wc[i.first]+=1;
				}
			}
		}
	}
	this->stopwords = par->stopwords;
	train();
}

void NaiveBayesClassifier::getStats(const string& accfile, const string& timefile, const string& tr_data_file)
{
	vector<instance> trdata = read_data(tr_data_file.c_str());
	int low = 500;
	int high = 10000;
	int diff = 3000;
	ofstream ofil1,ofil2;
	ofil1.open(timefile.c_str());
	ofil2.open(accfile.c_str());
	while(low<=high)
	{
		vector<instance> temp = trdata;
		random_shuffle(temp.begin(),temp.end());
		temp.resize(low);
		clock_t t1 = clock();
		NaiveBayesClassifier n1(this,temp);
		double acc1 = n1.evaluate("test.txt",false);
		clock_t t2 = clock();
		NaiveBayesClassifier n2(this,temp,true);
		double acc2 = n2.evaluate("test.txt",false);
		ofil1<<low<<", "<<t2-t1<<endl;
		ofil2<<low<<", "<<acc1<<", "<<acc2<<endl;
		low+=diff;
	}
	ofil1.close();
	ofil2.close();
}

pair<bool, bool> NaiveBayesClassifier::predict(const NaiveBayesClassifier::instance& inst)
{
	double pos = log(p_prob), neg = log(n_prob);
	for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) pos += i.second * log(p_prior[i.first]);
	for(auto i : inst.words) if(stopwords.find(i.first) == stopwords.end()) neg += i.second * log(n_prior[i.first]);
	return make_pair(pos >= neg, inst.sentiment);
}

double NaiveBayesClassifier::evaluate(const string& fl, bool printa=true)
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
	double neg_precision = (1.0*tn)/(tn+fn), neg_recall = (1.0*tn)/(tn+fp);
	double accuracy = (1.000*tot)/data.size();
	if(printa)
	{
		cout<< "Accuracy: " << accuracy <<endl;
		cout<< "Positive Precision: " << precision <<endl;
		cout<< "Positive Recall: " << recall <<endl;
		cout<< "F-Score: " << (2.0*precision*recall)/(precision+recall) <<endl;
		cout<< "Negative Precision: " << neg_precision <<endl;
		cout<< "Negative Recall: " << neg_recall <<endl;
		cout<< "F-Score: " << (2.0*neg_precision*neg_recall)/(neg_precision+neg_recall) <<endl;
		cout<<"\n********************************\n\n";
	}
	return accuracy;
}

int main()
{
	auto sw = {"", "shortswords.txt", "longswords.txt"};
	for(auto str: sw){
		NaiveBayesClassifier n1("train.txt", "vocab.txt",false,str);
		n1.evaluate("test.txt");
		NaiveBayesClassifier n2("train.txt", "vocab.txt",true, str);
		n2.evaluate("test.txt");
	}

	return 0;
}

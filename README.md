# ML-assignments
Solutions to Machine Learning Assignments of BITS Pilani Hyderabad Campus 

The following algorithms were implemented:

## Candidate Elimination

This is one method of Concept Learning with the **_assumption that the concept is present in the given Hypothesis Space_**

The algorithm was implemented according to pseudocode given in Tom Mitchell Machine Learning, 1997

**Results:-** Wherever the assumption held good, the algorithm was able to exactly generate the Version Space (~100% accuracy)

## Decision Trees

Basic Concept Learning was implemented in the form of Decision Trees, later pruning was added to prevent overfitting and Random Forests were also added, which are an ensemble over several decision trees in order to get better results.
After several experiments, it was found that considering O(sqrt(n)) trees and testing O(sqrt(n)) attributes at each node was found to give best results (relatively)

**Results:-** An accuracy of 80% was achieved on the naive Decision Trees, which improved to ~82% after pruning and  ~84% after Random Forest Classifier was added.

## Sentiment Analysis using Naive Bayes Classifier

A Naive Bayes Classifier was used to perform Sentiment Analysis on several Documents from a Movie Reviews dataset from [here](http://ai.stanford.edu/~amaas/data/sentiment/) with the **_assumption that occurrence of words are mutually conditionally independent and positionally independent_** .The documents were preprocessed into a bag-of-words model after which the Naive Bayes Classifier was trained with the logarithm of prior probabilities since the probability values themselves were small.
Results were compared with and without Binarizing (i.e.Just checking if a word occurs or not, irrespective of its frequency)
Also, results were compared with and without removing Stopwords given by the files, [Long Stopwords](https://github.com/Bharat123rox/ML-assignments/blob/master/SentimentAnalysis/longswords.txt), [Short Stopwords](https://github.com/Bharat123rox/ML-assignments/blob/master/SentimentAnalysis/shortswords.txt)

**Results:-** An accuracy of 81.3% was achieved with no stopwords, 82.7% was achieved with short list of stopwords and 82.3% with a longer list of stopwords on Naive Bayes.

After binarizing, accuracies are 83% with no stopwords, 83.8% with short stopwords and 83.3% with longer list of stopwords 

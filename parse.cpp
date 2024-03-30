#include <bits/stdc++.h>
map<int,set<string>> forward_index; // document ID: vector of words
map<string,set<int>> inverted_index; // word : set of documents
map<int,map<string,float>> overall_term_frequency;
map<string,int> inverted_document_count;
map<string,float> inverted_document_frequency;
using namespace std;
void parse_page(string str,int documentID)
{
    stringstream s(str); 
    // Term Frequency (Fraction of times a word appears in the document)
    map<string,int> term_count;
    map<string,float> term_frequency;
    string word;  
    int count = 0;
    while (s >> word)
    {
        term_count[word]++;
        inverted_index[word].insert(documentID);
        forward_index[documentID].insert(word);
        count++;
    }
    for(auto it:term_count)
    {
        term_frequency[it.first]=float(it.second)/count;
        inverted_document_count[it.first]++;
    }
    overall_term_frequency[documentID]=term_frequency;
}
void parse_documents(vector<string> documents,vector<int> documentIDs)
{
    for(int i=0;i<documents.size();i++)
    {
        parse_page(documents[i],documentIDs[i]);
    }
    int documentCount=documents.size();
    for(auto it:inverted_document_count)
    {
        inverted_document_frequency[it.first]=log(documentCount/it.second);
    }
}
void search_word(string word)
{
    // Get the document IDs containing the word
    // For each document d, calculate tf(word,d)*idf(word,d)
    // sort descending based on tf*idf
    // print descending search results
    set<int> documents=inverted_index[word];
    map<int,float> scores;
    for(auto it:documents)
    {
        float tf=overall_term_frequency[it][word];
        float idf=inverted_document_frequency[word];
        score=tf*idf;
        scores[it]=score;
    }
    for (auto it = scores.rbegin(); it != scores.rend(); it++) {
        cout<<"Document ID: "<<it.first<<" Score: "<<it.second<<endl;
    }
}
int main()
{
    vector<string> documents;
    vector<int> documentIDs;
    parse_documents(documents,documentIDs);
    search_word("")
    

}
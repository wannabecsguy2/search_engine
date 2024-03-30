#include <bits/stdc++.h>
map<int,set<string>> forward_index; // document ID: vector of words
map<string,set<int>> inverted_index; // word : set of documents
map<int,map<string,float>> overall_term_frequency;
map<string,int> inverted_document_count;
map<string,float> inverted_document_frequency;
using namespace std;
const int ALPHABET_SIZE = 26;
 
// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
 
    // isEndOfWord is true if the node represents
    // end of a word
    vector<int> documentIDs;
    bool isEndOfWord;
};
 
// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode =  new TrieNode;
 
    pNode->isEndOfWord = false;
    vector<int> temp;
    pNode->documentIDs=temp;
 
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
 
    return pNode;
}
 
// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key,int documentId)
{
    struct TrieNode *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
        pCrawl->documentIDs.push_back(documentId);
        pCrawl = pCrawl->children[index];
    }
 
    // mark last node as leaf
    pCrawl->documentIDs.push_back(documentId);
    pCrawl->isEndOfWord = true;
}
 
// Returns true if key presents in trie, else
// false
vector<int> search(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;
 
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!pCrawl->children[index])
        {
            vector<int> temp;
            return temp;
        }
        pCrawl = pCrawl->children[index];
    }
 
    return (pCrawl->documentIDs);
}
struct Document
{
    int documentID;
    string content;
    string url;
    string title;
};
struct TrieNode *root = getNode();
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
void parse_documents(vector<Document> documents)
{
    for(int i=0;i<documents.size();i++)
    {
        parse_page(documents[i].content,documents[i].documentID);
        insert(root, documents[i].title,documents[i].documentID);
    }
    int documentCount=documents.size();
    for(auto it:inverted_document_count)
    {
        inverted_document_frequency[it.first]=log(documentCount/it.second);
    }
    // Search for different keys
    vector<string> words={"the","these","their","thaw"};
    for(int i=0;i<words.size();i++)
    {
        vector<int> temp=search(root,words[i]);
        cout<<"Word: "<<words[i]<<endl;
        cout<<"Size: "<<temp.size()<<endl;
        for(int j=0;j<temp.size();j++)
        cout<<temp[j]<<" ";
        cout<<endl;
    }
    // cout<<"the"<<" --- "<<output[search(root, "the")]<<endl;
    // cout<<"these"<<" --- "<<output[search(root, "these")]<<endl;
    // cout<<"their"<<" --- "<<output[search(root, "their")]<<endl;
    // cout<<"thaw"<<" --- "<<output[search(root, "thaw")]<<endl;
}
void search_word(string word)
{
    //Search the title Trie
    vector<int> temp=search(root,word);
    cout<<"Word: "<<word<<endl;
    cout<<"Size: "<<temp.size()<<endl;
    for(int j=0;j<temp.size();j++)
    cout<<temp[j]<<" ";
    cout<<endl;
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
        float score=tf*idf;
        scores[it]=score;
    }
    for (auto it = scores.rbegin(); it != scores.rend(); it++) {
        cout<<"Document ID: "<<it->first<<" Score: "<<it->second<<endl;
    }
}
int main()
{
    //vector<string> documents;
    //vector<int> documentIDs;
    //parse_documents(documents,documentIDs);
    //search_word("");
    vector<Document> documents;
    parse_documents(documents);
    search_word("Python");

    

}
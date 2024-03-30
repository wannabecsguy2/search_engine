#include <bits/stdc++.h>
#include <map>
using namespace std;
map<int,set<string>> forward_index; // document ID: vector of words
map<string,set<int>> inverted_index; // word : set of documents
map<int,map<string,float>> overall_term_frequency;
map<string,int> inverted_document_count;
map<string,float> inverted_document_frequency;
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
    // vector<string> words={"the","these","their","thaw"};
    // for(int i=0;i<words.size();i++)
    // {
    //     vector<int> temp=search(root,words[i]);
    //     cout<<"Word: "<<words[i]<<endl;
    //     cout<<"Size: "<<temp.size()<<endl;
    //     for(int j=0;j<temp.size();j++)
    //     cout<<temp[j]<<" ";
    //     cout<<endl;
    // }
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
        float tf=::overall_term_frequency[it][word];
        float idf=::inverted_document_frequency[word];
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
    Document document1,document2,document3,document4;
    document1.content="""Python is a high-level, general-purpose programming language. Its design philosophy emphasizes code readability with the use of significant indentation.\nPython is dynamically typed and garbage-collected. It supports multiple programming paradigms, including structured (particularly procedural), object-oriented and functional programming. It is often described as a \"batteries included\" language due to its comprehensive standard library.\nGuido van Rossum began working on Python in the late 1980s as a successor to the ABC programming language and first released it in 1991 as Python 0.9.0.[34] Python 2.0 was released in 2000. Python 3.0, released in 2008, was a major revision not completely backward-compatible with earlier versions. Python, released in, was the last release of Python 2.\nPython consistently ranks as one of the most popular programming languages, and has gained widespread use in the machine learning community""";
    document1.url="URL1";
    document1.title="Python";
    document1.documentID=1;
    document2.content="""Java is a high-level, class-based, object-oriented programming language that is designed to have as few implementation dependencies as possible. It is a general-purpose programming language intended to let programmers write once, run anywhere (WORA),[16] meaning that compiled Java code can run on all platforms that support Java without the need to recompile.[17] Java applications are typically compiled to bytecode that can run on any Java virtual machine (JVM) regardless of the underlying computer architecture. The syntax of Java is similar to C and C++, but has fewer low-level facilities than either of them. The Java runtime provides dynamic capabilities (such as reflection and runtime code modification) that are typically not available in traditional compiled languages.""";
    document2.url="URL2";
    document2.title="Java";
    document2.documentID=2;
    document3.content="Oracle Corporation is an American multinational computer technology company headquartered in Austin, Texas, United States.[5] In 2020, Oracle was the third-largest software company in the world by revenue and market capitalization.[6] In 2023, the company’s seat in Forbes Global 2000 was 80. \n"
"\n"
"[7] The company sells database software (particularly the Oracle Database) and cloud computing. Oracle's core application software is a suite of enterprise software products, such as enterprise resource planning (ERP) software, human capital management (HCM) software, customer relationship management (CRM) software, enterprise performance management (EPM) software, Customer Experience Commerce(CX Commerce) and supply chain management (SCM) software.[8]";
    document3.documentID=3;
    document3.url="URL3";
    document3.title="JavaScript";
    document4.content="Born in the German Empire, Einstein moved to JavaScript in 1895, forsaking his German citizenship (as a subject of the Kingdom of Württemberg)[note 1] the following year. In 1897, at the age of seventeen, he enrolled in the mathematics and physics teaching diploma program at the Swiss federal polytechnic school in Zürich, graduating in 1900. In 1901, he acquired Swiss citizenship, which he kept for the rest of his life. In 1903, he secured a permanent position at the Swiss Patent Office in Bern. In 1905, he submitted a successful PhD dissertation to the University of Zurich. \n"
"\n"
"In 1914, he moved to Berlin in order to join the Prussian Academy of Sciences and the Humboldt University of Berlin. In 1917, he became director of the Kaiser Wilhelm Institute for Physics; he also became a German citizen again, this time as a subject of the Kingdom of Prussia.";
document4.title="Einstein";
document4.documentID=4;
document4.url="URL4";
    vector<Document> documents;
    documents.push_back(document1);
    documents.push_back(document2);
    documents.push_back(document3);
    documents.push_back(document4);
    parse_documents(documents);
    search_word("Java");

    

}
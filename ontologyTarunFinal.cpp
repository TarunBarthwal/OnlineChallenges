//======================================================
// Author      : Tarun Barthwal
// Description : Ontology's solution
//======================================================

#define DEBUG
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>

using namespace std;

//======================================================
// Function to check if character is a legal alphabet
//======================================================
bool isValidAlphabet(const char& a) {
  if (((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z')))
    return true;
  return false;
}

//======================================================
// Stores count as # of children of the trie
// next stores the address to next trie node
//======================================================
class QuestionTrieNode {
public:
  int count;
  unordered_map <char,QuestionTrieNode*> next;
  QuestionTrieNode() {
    count = 0;
  }
/*
  ~QuestionTrieNode() {
    cout<<"\nkilling..";
    for (unordered_map<char,QuestionTrieNode*>::iterator itr = next.begin(); itr != next.end(); itr++)
      delete itr->second;
  }
  */
};

typedef unordered_map<char,QuestionTrieNode*> QTrieMap;
typedef unordered_map<char,QuestionTrieNode*>::iterator QTrieMapPtr;

//======================================================
// Node of Topic tree
// subTopic list of nodes
// Questions stored as Trie
//======================================================
class Node {
public:
  string topicName;
  list<Node> subTopic;
  QuestionTrieNode qnode;

  Node(string name) : topicName(name) {
    }

  Node() {
    }

  #ifdef DEBUG
    void printQuestionTrieNode(QuestionTrieNode* current) {
        for (QTrieMapPtr itr = (current->next).begin(); itr != (current->next).end(); itr++) {
          cout<<itr->first;
          printQuestionTrieNode(itr->second);
        }
    }

    void print() {
      cout<<"\n Topic - "<<topicName;
      cout<<"\n   Size - "<<subTopic.size();
      for (list<Node>::iterator a = subTopic.begin(); a != subTopic.end(); a++)
        cout<<"\n\t Sub Topic - "<<(*a).topicName;
      cout<<"\nQuestion count - "<<qnode.count<<"\n";
      printQuestionTrieNode(&qnode);
    }
  #endif

  void insertQuestion(const string& question) {
    if (question.size() == 0)
      return;
    if (!isValidAlphabet(question[0]))
      return;

    QuestionTrieNode* current = &qnode;
    (current->count)++;
    for (int i = 0; i < question.size(); i++) {
      current = ((current->next)[question[i]] = new QuestionTrieNode);
      (current->count)++;
    }
    return;
  }
};

typedef unordered_map <string,Node*> MapNode;

string parseNextWord(const string& readLine, int& pos) {

  // Checking edge cases
  if(pos >= readLine.size()) {
    cout<<"end of string...";
    return string ("");
  }

  // Skipping white spaces
  while((pos <= readLine.size()) && (readLine[pos] == ' '))
    pos++;

  if(pos >= readLine.size()) {
    cout<<"end of string...";
    return string ("");
  }

  // Returning special characters that are permissible '(',')',':'
  if (readLine[pos] == '(') {
    pos++;
    return string("(");
  }

  if (readLine[pos++] == ')') {
    pos++;
    return string(")");
  }

  if (readLine[pos] == ':') {
    pos++;
    return string(":");
  }

  // Parse next word until it is not a valid character
  string word;

  while(isValidAlphabet(readLine[pos]))
    word += readLine[pos++];
  return word;
}

void createTreeHelper(Node& parent, MapNode& Topic, const string& readLine, int& pos) {
  Node* lastTopic = &parent;

  string word = parseNextWord(readLine,pos);

  while(1) {
    if (word == "(")
      createTreeHelper((*lastTopic), Topic, readLine, pos);
    else if (word == ")")
      return;
    else {
      (parent.subTopic).push_back(word);
      Topic[word] = &((parent.subTopic).back());
      lastTopic = &((parent.subTopic).back());
    }

    word = parseNextWord(readLine,pos);
  }
  return;
}

void createTree(Node& root, MapNode& Topic, const string& readLine) {
  int pos = 0;
  string word = parseNextWord(readLine,pos);

  root.topicName = word;
  Topic[word] = &root;

  word = parseNextWord(readLine,pos);
  if (word == "(")
    createTreeHelper(root,Topic, readLine, pos);
}

void insertTopicQuestion(MapNode& Topic, const int& M) {
  string readLine;
  for (int i = 0; i < M; i++) {
    getline(cin,readLine);

    int pos = 0;
    string topic = parseNextWord(readLine,pos);
    string colon = parseNextWord(readLine,pos);
    pos++;
    string question = readLine.substr(pos);

    Topic[topic]->insertQuestion(question);
  }
}

int countQuestion(Node& current, const string& query) {
  int count = 0;

  QuestionTrieNode* temp = &(current.qnode);
  QTrieMapPtr it;

  for(int i = 0; i < query.size(); i++) {
    if (i == 0)
      it = (temp->next).find(query[i]);
    else
      it = (it->second->next).find(query[i]);

    if (it == temp->next.end())
      return 0;
  }
  return (it->second->count);
}

void outputQueryHelper(Node& current, const string& query, int& count) {
  count += countQuestion(current, query);
  for (list<Node>::iterator nPtr = current.subTopic.begin(); nPtr != current.subTopic.end(); nPtr++)
    outputQueryHelper((*nPtr), query, count);
}

void outputQuery(MapNode& Topic, const int& K) {
  string readLine;

  for (int i = 0; i < K; i++) {
    getline(cin,readLine);
    int pos = 0;
    string topic = parseNextWord(readLine,pos);
    string query = readLine.substr(pos+1);
    int count = 0;

    outputQueryHelper(*(Topic[topic]), query, count);
    cout<<count<<"\n";
  }
}

void parser(Node& root, MapNode& Topic) {
  int N,M,K;
  cin>>N;
  cin.ignore();

  string readLine;
  getline(cin,readLine);

  if ( N > 0)
    createTree (root,Topic,readLine);

  cin>>M;
  cin.ignore();

  if (M > 0)
    insertTopicQuestion(Topic,M);

  cin>>K;
  cin.ignore();

  if(K>0)
    outputQuery(Topic, K);

  return;
}

int main()
{
  Node root;
  MapNode Topic;
  parser(root,Topic);

  #ifdef DEBUG
    cout<<"\nPrinting tree now:\n";
      string topic = "Birds";
      Topic[topic]->print();
      topic = "Eagles";
      Topic[topic]->print();
      topic = "Reptiles";
      Topic[topic]->print();
      topic = "Animals";
      Topic[topic]->print();
      cout<<"\nMap Size: "<<Topic.size();
  #endif

  return 0;
}

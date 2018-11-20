#include "Trie.h"
#include <ctype.h>

using namespace std;
TrieSet::TrieSet(){
	root = new TrieNode;
	root->parent = NULL;
	root->inSet = false;
	for(int i = 0; i < 26; i++){
		root->children[i] = NULL;
	}
	size = 0;
};
TrieSet::~TrieSet(){
	for (set<string>::iterator it = words.begin(); it != words.end(); ++it)
		remove(*it);
};

void TrieSet::insert(std::string input){
	if(inTrie(input))
		return;
	TrieNode* temp = root;
	int index;
	for(unsigned int i = 0; i < input.length(); i++){
		index = tolower(input[i]) - 'a';
		if(temp->children[index] == NULL){
			temp->children[index] = new TrieNode;
			temp->children[index]->parent = temp;
			temp->children[index]->inSet = false;
			for(int j = 0; j < 26; j++)
				temp->children[index]->children[j] = NULL;
		}
		temp = temp->children[index];
	}
	temp->inSet = true;
	size++;
	words.insert(input);
}


void TrieSet::remove (std::string input){
	if(!inTrie(input))
		return;
	TrieNode* temp = root;
	for (unsigned int i = 0; i < input.length(); i++){
		int index = tolower(input[i]) - 'a';
		temp = temp->children[index];
	}
	temp->inSet = false;
	if(isLeafNode(temp)){
		temp->inSet = false;
		int count = input.length() - 1;
		while(!temp->inSet && isLeafNode(temp)){
			TrieNode* parent = temp->parent;
			delete temp;
			if(parent == NULL)
				break;
			parent->children[tolower(input[count]) - 'a'] = NULL;
			temp = parent;
			count--;
		}
	}
}


TrieNode* TrieSet::prefix(std::string px){
	
}

bool TrieSet::inTrie(std::string input){
	TrieNode* temp = root;
	for (unsigned int i = 0; i < input.length(); i++){
		int index = tolower(input[i]) - 'a';
		if(temp->children[index] == NULL)
			return false;
		temp = temp->children[index];
	}
	return true;
}

bool TrieSet::isLeafNode(TrieNode* node){
	for(int i = 0; i < 26; i++){
		if(node->children[i] != NULL)
			return false;
	}
	return true;
}
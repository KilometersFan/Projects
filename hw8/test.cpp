#include "Trie.h"
#include "Dictionary.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	Dictionary dict("dictionary.txt");
	TrieSet trie;
	for(set<string>::iterator it = dict.dictionary.begin(); it != dict.dictionary.end(); it++)
		trie.insert(*it);
	// trie.insert("zebra");
	// trie.insert("don't");
	// trie.insert("zyzzyvas");
	cout << trie.inTrie("zebra") << endl;
	cout << trie.inTrie("don't") << endl;
	trie.remove("zyzzyvas");
	cout << trie.inTrie("zyzzyvas") << endl;
	trie.remove("zyzzyvas");
	trie.remove("zyzzyvas");
	trie.remove("zyzzyvas");
	return 0;
} 
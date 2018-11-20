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
	cout << trie.inTrie("zebra") << endl;

	return 0;
} 
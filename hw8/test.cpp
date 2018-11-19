#include "Trie.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	TrieSet trie;
	trie.insert("a");
	trie.insert("apple");
	trie.insert("zebra");
	cout << trie.inTrie("apple") << endl;
	cout << trie.inTrie("appl") << endl;
	cout << trie.inTrie("a") << endl;
	cout << trie.inTrie("bapple") << endl;
	cout << trie.inTrie("apples") << endl;
	return 0;
} 
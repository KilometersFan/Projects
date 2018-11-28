#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include "Trie.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include <set>
#include <bits/stdc++.h> 
#include <algorithm>
#include <queue>

void scrabble(std::vector<Player*> &players, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie);
void validMoveCheck(std::string &str);
bool initializeBoard(std::string &initFile, Board& board, std::set<std::pair<size_t, size_t>> &occupiedCoords);
void permute(std::string s,size_t i,size_t n, std::set<std::string>& combos);
void findSubsets(std::string str, std::string out, size_t count, std::set<std::string>& sets);
void getVerticalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords, std::set<std::pair<size_t, size_t>>& startCoords);
void getHorizontalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords, std::set<std::pair<size_t, size_t>>& startCoords);
void getValidHorizontalMoves(std::set<Tile*> hand, size_t x, size_t y, Board& board, std::string word, std::string move, std::set<std::string> &validMoves, TrieSet& trie, bool connected, TrieNode* node);
void getValidVerticalMoves(std::set<Tile*> hand, size_t x, size_t y, Board& board, std::string word, std::string move, std::set<std::string> &validMoves, TrieSet& trie, bool connected, TrieNode* node);

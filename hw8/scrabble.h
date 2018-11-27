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

void scrabble(std::vector<Player*> &players, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie);
void validMoveCheck(std::string &str);
bool initializeBoard(std::string &initFile, Board& board, std::set<std::pair<size_t, size_t>> &occupiedCoords);
void permute(std::string str, std::string out, std::set<std::string>& combos);
void findSubsets(std::string str, std::string out, size_t count, std::set<std::string>& sets);
std::set<std::pair<size_t, size_t>> getVerticalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords);
std::set<std::pair<size_t, size_t>> getHorizontalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& occupiedCoords);
void getValidHorizontalMoves(std::string str, std::set<std::pair<size_t, size_t>> &startCoords, Board& board, std::set<std::string> &moves, TrieSet& trie);
void getValidVerticalMoves(std::string str, std::set<std::pair<size_t, size_t>> &startCoords, Board& board, std::set<std::string> &moves, TrieSet& trie);

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
#include <algorithm>
#include <sstream>

void scrabble(std::vector<Player*> &players, Board &board, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie);
void validMoveCheck(std::string &str);
bool initializeBoard(std::string &initFile, Board& board);
void getVerticalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& startCoords);
void getHorizontalStartCoords(size_t length, Board &board, std::set<std::pair<size_t, size_t>>& startCoords);
bool validAdjacentWord(size_t x, size_t y, Tile* tile, Board& board, TrieSet& trie, bool horizontal);
void findHorizontalMovesHelper(std::set<Tile*>hand, size_t x, size_t y, TrieSet& trie, Board& board, TrieNode* node, std::string move, std::set<std::string>& moves);
void findHorizontalMoves(std::set<Tile*> hand, size_t x, size_t y, TrieSet& trie, Board& board, std::set<std::string>& moves);
void findVerticalMoves(std::set<Tile*> hand, size_t x, size_t y, TrieSet& trie, Board& board, std::set<std::string>& moves);
void findVerticalMovesHelper(std::set<Tile*>hand, size_t x, size_t y, TrieSet& trie, Board& board, TrieNode* node, std::string move, std::set<std::string>& moves);
size_t findStartLocation(size_t x, size_t y, Board& board, bool horizontal);
std::string returnMaxPointMove(std::set<std::string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie);
std::string returnMaxLengthMove(std::set<std::string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie);
void returnCPUTiles(Player& player, Move* m);


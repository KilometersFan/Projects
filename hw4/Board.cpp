#include "Board.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

Board::Board (std::string board_file_name){
	ifstream ifile(board_file_name);
	ifile >> _x;
	ifile >> _y;
	ifile >> _sx;
	ifile >> _sy;

	for (size_t i = 1; i <= _x; i++){
		for (size_t j = 1; j <= _y; j++){
			char c;
			ifile >> c;
			bool startCoord = false;
			unsigned int LMult = 1;
			unsigned int WMult = 1;
			pair<size_t, size_t> location = make_pair(i, j);
			if(i == _sx && j == _sy)
				startCoord = true;
			if(c - '0' >= 0&& c- '0' <=9)
				LMult = c - '0';
			else if (c == 't')
				WMult = 3;
			else if (c == 'd')
				WMult = 2;
			_board[location] = new Square(LMult, WMult, startCoord);
		}
	}
}

Board::~Board (){
	for (size_t i = 1; i <= _x; i++){
		for (size_t j = 1; j <= _y; j++){
			pair<size_t, size_t> square = make_pair(i, j);
			delete _board[square];
		}
	}
}

/* returns a vector of all words that would be formed by executing the
       given move. The first element of the pair is a string containing the word
       formed, and the second element is the score that that word earns
       (with all multipliers, but not the 50-point bonus for all letters).
getSquare
   Words returned are all in uppercase.

   The last entry of the vector is always the "main" word formed
   in the direction chosen by the user; the others could be in arbitrary
   order. (This is helpful for backtracking search.)

   This function does not check that the words formed are actually in the dictionary.
   The words returned from this function must be checked against the dictionary to
   determine if the move is legal.	*/
vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	vector<pair<string, unsigned int>> results;
	pair<string, unsigned int> word("",-1);
	for (size_t i = 0; i < m.getPlayerTiles().size(); i++){
		if(horizontal)
			word = getAdjacentWords(x + i, y, horizontal, m.getPlayerTiles()[i]);
		else 
			word = getAdjacentWords(x, y + i, horizontal, m.getPlayerTiles()[i]);
		if(word.second >= 0 && word.first != "")
			results.push_back(word);
	}
	word = getOriginalWord(x, y, horizontal, m.getPlayerTiles());
	results.push_back(word);
	return results;
}

bool Board::validPlaceMove(const PlaceMove &m){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	vector<Tile*> tiles = m.getPlayerTiles();
	Square* square;
	size_t k = 0;
	int squaresToFill = tiles.size();
	bool startCoord = false;
	bool connected = false;
	while (squaresToFill != 0 && !connected && !startCoord){
		square = getSquare(x, y + k);
		if(square->isStart()){
			startCoord = true;
		}
		connected = validPlaceMoveHelper(x - 1, y + k);
		if(connected)
			break;
		connected = validPlaceMoveHelper(x + 1, y + k);
		if(connected)
			break;
		connected = validPlaceMoveHelper(x, y + k - 1);
		if(connected)
			break;
		connected = validPlaceMoveHelper(x, y + k + 1);
		if(connected)
			break;
		squaresToFill--;
		k++;
	}	
	if(connected || startCoord)
		return true;
	return false;
}

bool Board::validPlaceMoveHelper(size_t x, size_t y){
	Square* temp = getSquare(x, y);
	if(temp->isOccupied())
		return true;
	return false;
}

pair<string, unsigned int> Board::getAdjacentWords(size_t x, size_t y, bool horizontal, Tile* tile){
	Square* square = getSquare(x, y); 
	pair<string, unsigned int> buff("", -1);
	if(square->isOccupied())
		return buff;
	buff = getAdjacentWordsHelper(x, y, square, horizontal, tile);	
	return buff;
}
//TODO Condense this function
pair<string, unsigned int> Board::getAdjacentWordsHelper(size_t &x, size_t &y, Square* &square, bool &horizontal, Tile* &tile){
	unsigned int score = tile->getPoints() * square->getLMult();
	unsigned int wordMult = square->getWMult();
	size_t i = 0;
	pair<string, unsigned int> result;
	Square* newSquare;
	if(horizontal){
	  	newSquare = getSquare(x - 1, y);
		while (newSquare->isOccupied() && x - i - 1 >0){
			i++;
			newSquare = getSquare(x - i - 1, y);
		}
	}
	else {
		newSquare = getSquare(x, y- 1);
		while (newSquare->isOccupied() && y - i - 1 >0){
			i++;
			newSquare = getSquare(x, y- i - 1);
		}
	}
	size_t k = 0;
	size_t top_x = x - i;
	size_t top_y = y - i;
	string word = "";
	if(horizontal)	{
		newSquare = getSquare(top_x, y);
		while ((newSquare->isOccupied() || top_x + k == x) && top_x + k <= getRows()){
			if(top_x + k == x){
				word += tile->getUse();
			}
			else {
				word += newSquare->getLetter();
				score += newSquare->getScore();
			}
			k++;
			newSquare = getSquare(top_x + k, y);

		}
	}
	else {
		newSquare = getSquare(x, top_y);
		while ((newSquare->isOccupied() || top_y + k == y) && top_y + k <= getColumns()){
			if(top_y + k == y){
				word += tile->getUse();
			}
			else {
				word += newSquare->getLetter();
				score += newSquare->getScore();
			}
			k++;
			newSquare = getSquare(x, top_y + k);

		}
	}
	
	score *= wordMult;
	if(word.length() > 1)
		result = make_pair(word, score);
	return result;
		
}
//TODO Condense this function
pair<string, unsigned int> Board::getOriginalWord(size_t &x, size_t &y, bool &horizontal, vector<Tile*> tiles){
	unsigned int score = 0;
	unsigned int wordMult = 1;
	size_t i = 0;
	pair<string, unsigned int> result;
	Square* newSquare;
	if(!horizontal){
	  	newSquare = getSquare(x - 1, y);
		while (newSquare->isOccupied() && x - i - 1 >0){
			i++;
			newSquare = getSquare(x - i - 1, y);
		}
	}
	else {
		newSquare = getSquare(x, y- 1);
		while (newSquare->isOccupied() && y - i - 1 >0){
			i++;
			newSquare = getSquare(x, y- i - 1);
		}
	}
	size_t k = 0;
	size_t top_x = x - i;
	size_t top_y = y - i;
	string word = "";
	int squaresToFill = tiles.size();

	if(!horizontal)	{
		newSquare = getSquare(top_x, y);
		while ((newSquare->isOccupied() || squaresToFill != 0) && top_x + k <= getRows()){
			if(!newSquare->isOccupied()){
				word += tiles[0]->getUse();
				wordMult *= newSquare->getWMult();
				score+= tiles[0]->getPoints() * newSquare->getLMult();
				tiles.erase(tiles.begin());
				squaresToFill--;
			}
			else {
				word += newSquare->getLetter();
				score += newSquare->getScore();
			}
			k++;
			newSquare = getSquare(top_x + k, y);

		}
	}
	else {
		newSquare = getSquare(x, top_y);
		while ((newSquare->isOccupied() || squaresToFill != 0) && top_y + k <= getColumns()){
			if(!newSquare->isOccupied()){
				word += tiles[0]->getUse();
				wordMult *= newSquare->getWMult();
				score+= tiles[0]->getPoints() * newSquare->getLMult();
				tiles.erase(tiles.begin());
				squaresToFill--;
			}
			else {
				word += newSquare->getLetter();
				score += newSquare->getScore();
			}
			k++;
			newSquare = getSquare(x, top_y + k);

		}
	}
	score *= wordMult;
	result = make_pair(word, score);
	return result;
}
/* Executes the given move by taking tiles and placing them on the board.
   This function does not check for correctness of the move, so could
   segfault or cause other errors if called for an incorrect move.
   When a blank tile '?' is placed on the board, it is treated as a letter,
   i.e., the corresponding square has that letter (with score 0) placed on it.
*/
void Board::executePlaceMove (const PlaceMove & m){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	Square* square = getSquare(x, y);
	size_t i = 0;
	vector<Tile*> playerTiles = m.getPlayerTiles();
	cout << playerTiles.size() << endl;
	size_t spacesToFill = playerTiles.size();
	while (spacesToFill != 0){
		if(horizontal)
			square = getSquare(x, y + i);
		else 
			square = getSquare(x + i, y);
		if(!square->isOccupied()){
			square->placeTile(playerTiles[0]);
			playerTiles.erase(playerTiles.begin());
			spacesToFill--;
		}
		i++;
	}
}

/* Returns a pointer to the Square object representing the
   (y,x) position of the board. Indexing starts at 1 here.
   This is needed only to display the board. */
Square * Board::getSquare (size_t x, size_t y) const {
	pair<size_t, size_t> location = make_pair(x,y);
	Square * square = _board.find(location)->second;
	return square;
}

/* Returns the number of rows of the board.*/
size_t Board::getRows() const{
	return _x;
}

/* Returns the number of columns of the board.*/
size_t Board::getColumns() const{
	return _y;
}
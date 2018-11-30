#include "Board.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

//creates board with desired squres using the inputted file
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
			pair<size_t, size_t> location = make_pair(j, i);
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
//destroys each square
Board::~Board (){
	for (size_t i = 1; i <= _x; i++){
		for (size_t j = 1; j <= _y; j++){
			pair<size_t, size_t> square = make_pair(i, j);
			delete _board[square];
		}
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

std::pair<size_t, size_t> Board::getStartCoords() const{
	return make_pair(_sy, _sx);
}

//checks if a word is connected to at least one tile or if a word is the first move
bool Board::validPlaceMove(const PlaceMove &m, bool firstmove){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	vector<Tile*> tiles = m.getPlayerTiles();
	//if out of bouands return invalid
	try {
		if(x < 1 || x > getColumns() || y < 1 || y > getRows())
			throw MoveException("Error: Word placement is out of bounds.");
		Square* square = getSquare(x, y);
		//if placed on occupied square return invalid
		if(square->isOccupied())
			throw MoveException("Error: Word is placed on an occupied square.");
		size_t k = 0;
		int squaresToFill = tiles.size();
		bool connected = false;
		bool startCoord = false;
		//loop through squares to check validty
		while (squaresToFill != 0){
			bool topConnected = false;
			bool bottomConnected = false;
			bool leftConnected = false;
			bool rightConnected = false;
			size_t top_x = x + k;
			size_t top_y = y + k;
			//see if connected to another tile or if a tile is on the start square
			if(!horizontal){
				if(top_y > getRows()){
					throw MoveException("Error: Word exceeds row boundary of board");
				}
				square = getSquare(x, y + k);
				if(!square->isOccupied()){
					if(x - 1 > 0 && y + k <= getRows())
						leftConnected = validPlaceMoveHelper(x - 1, top_y);
					if(x + 1 <= getColumns() && y + k <= getRows())
						rightConnected = validPlaceMoveHelper(x + 1, top_y);
					if(top_y -1 > 0)
						topConnected = validPlaceMoveHelper(x, top_y - 1);
					if(top_y + 1 <= getRows()) 
						bottomConnected = validPlaceMoveHelper(x, top_y + 1);
					if(topConnected || bottomConnected || leftConnected || rightConnected)
						connected = true;
					squaresToFill--;
				}
			}
			else{
				if(top_x > getColumns()){
					throw MoveException("Error: Word exceeds column boundary of board.");
				}
				square = getSquare(x + k, y);
				if(!square->isOccupied()){
					if(top_x - 1 > 0)
						leftConnected = validPlaceMoveHelper(top_x - 1, y);
					if(top_x + 1 <= getColumns())
						rightConnected = validPlaceMoveHelper(top_x + 1, y);
					if(top_x <= getColumns() && y -1 > 0)
						topConnected = validPlaceMoveHelper(top_x, y - 1);
					if(top_x <= getColumns() && y + 1 <= getRows())
						bottomConnected = validPlaceMoveHelper(top_x, y + 1);
					if(topConnected || bottomConnected || leftConnected || rightConnected)
						connected = true;
					squaresToFill--;
				}
			}
			if(square->isStart() && !square->isOccupied()){
				startCoord = true;
			}
			k++;
		}
		if(connected || startCoord || firstmove)
			return true;
		else 
			throw MoveException("Error: Word was not placed on the start tile or was not connected.");
	}
	catch (MoveException &e){
		if(m.getPlayer()->getType() == "human")
			cout << e.getMessage() << endl;
		return false;
	}
	return false;
}
//checks to see if an adjacent square is occupied
bool Board::validPlaceMoveHelper(size_t x, size_t y){
	Square* temp = getSquare(x, y);
	if(temp->isOccupied())
		return true;
	return false;
}
//returns all words formed by a place move
vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	vector<pair<string, unsigned int>> results;
	pair<string, unsigned int> word("",0);
	Square * square;
	size_t spacesToFill = m.getPlayerTiles().size();
	size_t i = 0;
	size_t count = 0;
	//loops until the whole tile strings adjacent words are checked
	while (spacesToFill != 0){
		word = make_pair("", 0);
		if(!horizontal){
			square = getSquare(x, y + i);
			if(!square->isOccupied()){
				word = getAdjacentWords(x , y+ i, horizontal, m.getPlayerTiles()[count]);
				count++;
				spacesToFill--;
			}
		}
		else {
			square = getSquare(x + i , y);
			if(!square->isOccupied()){
				word = getAdjacentWords(x+ i, y , horizontal, m.getPlayerTiles()[count]);
				count++;
				spacesToFill--;
			}
		}
		if(word.second >= 0 && word.first != ""){
			results.push_back(word);
		}
		i++;
	}
	word = getOriginalWord(m, m.getPlayerTiles());
	if(word.first.length() > 1)
		results.push_back(word);
	return results;
}
//gets all adjacent moves. If a word was horizontally placed, returns all vertical words form by each tile
pair<string, unsigned int> Board::getAdjacentWords(size_t x, size_t y, bool horizontal, Tile* tile){
	Square* square = getSquare(x, y); 
	pair<string, unsigned int> buff("", 0);
	if(!square->isOccupied())
		buff = getAdjacentWordsHelper(x, y, square, horizontal, tile);	
	return buff;
}
//builds the adjacent words and points
pair<string, unsigned int> Board::getAdjacentWordsHelper(size_t &x, size_t &y, Square* &square, bool &horizontal, Tile* &tile){
	unsigned int score = tile->getPoints() * square->getLMult();
	unsigned int wordMult = square->getWMult();
	size_t i = 0;
	pair<string, unsigned int> result;
	Square* newSquare;
	//finds top most adjacent tile or left most adjacent tile
	if(!horizontal && x - 1 > 0){
	  	newSquare = getSquare(x - 1, y);
		while (newSquare->isOccupied() && x - i - 1 >0){
			i++;
			if(x - i - 1 > 0)
				newSquare = getSquare(x - i - 1, y);
		}
	}
	else if (horizontal && y - 1 > 0){
		newSquare = getSquare(x, y- 1);
		while (newSquare->isOccupied() && y - i - 1 >0){
			i++;
			if(y - i - 1 > 0)
				newSquare = getSquare(x, y- i - 1);
		}
	}
	size_t k = 0;
	size_t top_x = x - i;
	size_t top_y = y - i;
	string word = "";
	//builds word from top most/left most tile
	if(!horizontal)	{
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
			if(top_x+k <= getRows())
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
			if(top_y + k <= getColumns())
				newSquare = getSquare(x, top_y + k);
		}
	}
	score *= wordMult;
	if(word.length() > 1)
		result = make_pair(word, score);
	return result;
		
}
//gets original word
pair<string, unsigned int> Board::getOriginalWord(const PlaceMove &m, vector<Tile*> tiles){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	unsigned int score = 0;
	unsigned int wordMult = 1;
	size_t i = 0;
	size_t handSize = tiles.size();
	pair<string, unsigned int> result;
	Square* newSquare;
	//finds top most/left most tile of the word
	if(horizontal && x - 1 >0){
		newSquare = getSquare(x - 1, y);
		while (newSquare->isOccupied() && x - i - 1 >0){
			i++;
			if( x - i - 1 > 0)
				newSquare = getSquare(x - i - 1, y);
		}
	}
	else if (!horizontal && y - 1 > 0){
		newSquare = getSquare(x, y - 1);
		while (newSquare->isOccupied() && y - i - 1 >0){
			i++;
			if(y - i - 1 > 0)
				newSquare = getSquare(x, y - i - 1);
		}
	}
	size_t k = 0;
	size_t top_x = x - i;
	size_t top_y = y - i;
	string word = "";
	int squaresToFill = tiles.size();
	//builds word from top most/ left most tile
	//also adds points/multiploiers to score
	if(horizontal)	{
		newSquare = getSquare(top_x + k, y);
		while ((newSquare->isOccupied() || squaresToFill != 0) && top_x + k <= getColumns()){
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
			if(top_x + k <= getColumns())
				newSquare = getSquare(top_x + k, y);
		}
	}
	else {
		newSquare = getSquare(x, top_y+k);
		while ((newSquare->isOccupied() || squaresToFill != 0) && top_y + k <= getRows()){
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
			if(top_y + k <= getRows())
				newSquare = getSquare(x, top_y+k);
		}
	}
	score *= wordMult;
	if(handSize == m.getMaxTiles())
		score += 50;
	result = make_pair(word, score);
	return result;
}
//places each tile down on the board in the appropriate square
void Board::executePlaceMove (const PlaceMove & m){
	size_t x = m.getX();
	size_t y = m.getY();
	bool horizontal = m.isHorizontal();
	Square* square = getSquare(x, y);
	size_t i = 0;
	vector<Tile*> playerTiles = m.getPlayerTiles();
	size_t spacesToFill = playerTiles.size();
	while (spacesToFill != 0){
		if(!horizontal)
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

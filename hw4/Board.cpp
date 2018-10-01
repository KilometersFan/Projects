#include "Board.h"
#include <fstream>
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
			unsigned int LMult = 0;
			unsigned int WMult = 0;
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
			delete _board[std::pair<size_t, size_t> (i, j)];
		}
	}
}

/* returns a vector of all words that would be formed by executing the
       given move. The first element of the pair is a string containing the word
       formed, and the second element is the score that that word earns
       (with all multipliers, but not the 50-point bonus for all letters).

   Words returned are all in uppercase.

   The last entry of the vector is always the "main" word formed
   in the direction chosen by the user; the others could be in arbitrary
   order. (This is helpful for backtracking search.)

   This function does not check that the words formed are actually in the dictionary.
   The words returned from this function must be checked against the dictionary to
   determine if the move is legal.	*/
vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m) const{
	pair<string, unsigned int> temp ("Hi", 0);
	vector<pair<string, unsigned int>> vectTemp;
	vectTemp.push_back(temp);
	return vectTemp;

}

/* Executes the given move by taking tiles and placing them on the board.
   This function does not check for correctness of the move, so could
   segfault or cause other errors if called for an incorrect move.
   When a blank tile '?' is placed on the board, it is treated as a letter,
   i.e., the corresponding square has that letter (with score 0) placed on it.
*/
void Board::executePlaceMove (const PlaceMove & m){

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
/*
 * Board.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The class Board encodes the state of the Scrabble board.
   It provides functions for displaying the board,
   and for returning the words (and score) that would be obtained by a move.
*/

#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Tile.h"
#include "Square.h"
#include "Move.h"
#include "Dictionary.h"
#include <utility>
#include "Exceptions.h"
class Board
{
	
public:
	/* constructor that initializes the board state with an empty board
           and the size and bonuses and start square based on the given file. */
	Board (std::string board_file_name);

	~Board ();

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
	std::vector<std::pair<std::string, unsigned int>> getPlaceMoveResults(const PlaceMove &m);

	bool validPlaceMove(const PlaceMove &m, bool firstmove);

	/* Executes the given move by taking tiles and placing them on the board.
	   This function does not check for correctness of the move, so could
	   segfault or cause other errors if called for an incorrect move.
	   When a blank tile '?' is placed on the board, it is treated as a letter,
	   i.e., the corresponding square has that letter (with score 0) placed on it.
	*/
	void executePlaceMove (const PlaceMove & m);

	/* Returns a pointer to the Square object representing the
       (y,x) position of the board. Indexing starts at 1 here.
	   This is needed only to display the board. */
	Square * getSquare (size_t x, size_t y) const;

	/* Returns the number of rows of the board.*/
	size_t getRows() const;

	/* Returns the number of columns of the board.*/
	size_t getColumns() const;

private:
	size_t _x;
	size_t _y;
	size_t _sx;
	size_t _sy;
	std::map<std::pair<size_t, size_t>, Square*> _board;

	std::pair<std::string, unsigned int> getAdjacentWords(size_t x, size_t y, bool isHor, Tile* tile);

	std::pair<std::string, unsigned int> getAdjacentWordsHelper(size_t &x, size_t &y, Square* &square, bool &horizontal, Tile* &tile);
	
	std::pair<std::string, unsigned int> getOriginalWord(const PlaceMove &m, std::vector<Tile*> tiles);

	bool validPlaceMoveHelper(size_t x, size_t y);

};


#endif /* BOARD_H_ */

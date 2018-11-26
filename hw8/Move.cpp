#include "Move.h"
#include "Board.h"
#include <sstream>
#include <algorithm>
#include <locale>
#include <string>
#include <iostream>
using namespace std;

Move::~Move(){
	
}

Move::Move(Player * player) {
	_player = player;
}
//helps with checking validity of a move
bool Move::isValidMove() {
	return validMove;
}
//same as isValidMove
void Move::setValidMove(bool m) {
	validMove = m;
}

Player* Move::getPlayer() const{
	return _player;
}
//do nothing but is a valid move
PassMove::PassMove(Player * player) : Move(player){
	setValidMove(true);
}
//gets tiles inputted by user, checks if they have them, then returns the tiles to appropriate location
ExchangeMove::ExchangeMove(string tileString, Player * p) : Move(p){
	_tileString = tileString;
	try {
		if(_player->hasTiles(tileString, false))
			_playerTiles = _player->takeTiles(_tileString, false);
		else if (tileString.length() == 0)
			throw MoveException("Error: Player did not specify any tiles.");
		else
			throw MoveException("Error: Player does not have tiles: " + _tileString);
		setValidMove(true);
	}
	catch (MoveException &e){
		setValidMove(false);
		cout << e.getMessage() << endl;
	}
	
}
//reads in direction, location, and tilestring if player has the tiles inputted, 
PlaceMove::PlaceMove (size_t x, size_t y, bool horizontal, string tileString, Player * p) : Move(p){
	_x = x;
	_y = y;
	_horizontal = horizontal;
	_tileString = tileString;
	try {
		if(_player->hasTiles(tileString, true))
			_playerTiles = _player->takeTiles(_tileString, true);
		else if (tileString.length() == 0)
			throw MoveException("Error: Player did not specify any tiles.");
		else{
			throw MoveException("Error: Player does not have tiles: " + _tileString);
		}
		setValidMove(true);
	}
	catch (MoveException &e){
		setValidMove(false);
		cout << e.getMessage() << endl;
	}
}

//takes in input, creates desired move with parameters
Move * Move::parseMove(string moveString, Player &p){
	transform(moveString.begin(), moveString.end(), moveString.begin(), ::tolower);
	// cout << moveString << endl;
	stringstream ss(moveString);
	string moveType = "";
	string tileString = "";
	char dir;
	size_t x = 0;
	size_t y = 0;
	bool hor = false;
	ss >> moveType;
	//builds appropriate move type based on user input
	try{
		if(moveType == "pass"){
		return new PassMove(&p);
		}
		else if (moveType == "exchange"){
			ss >> tileString;
			return new ExchangeMove(tileString, &p);
		}
		else if(moveType == "place"){
			ss >> dir;
			ss >> y;
			ss >> x;
			ss >> tileString;
			if(dir == '-')
				hor = true;	
			return new PlaceMove(x, y, hor, tileString, &p);
		}
		throw MoveException("Error: invalid action " + moveType);
	}
	catch (MoveException &m){
		cerr << m.getMessage() << endl;
	}
	//this line is for the bytes autochecker since it returns an error if there is no return at the end
	return new PassMove(&p);
}
//does nothing
void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary, bool initTrue){
	return;
}
//adss tiles to bag, draws same number of tiles from bag into player's hand
//returns what new tiles were added
void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary, bool initTrue){
	if(!isValidMove())
		return;
	bag.addTiles(_playerTiles);
	vector<Tile*>newTiles = bag.drawTiles(_playerTiles.size());cout << "Tiles added to your hand: ";
	for (vector<Tile*>::iterator it = newTiles.begin(); it != newTiles.end(); it++){
		cout << (*it)->getLetter();
	}
	cout << endl;
	_player->addTiles(newTiles);
	return;
}
//gets column number
size_t PlaceMove::getX() const{
	return _x;
}
//gets row number
size_t PlaceMove::getY() const{
	return _y;
}
//gets whether a move is horizontal or vertical
bool PlaceMove::isHorizontal() const{
	return _horizontal;
}
//gets player hands max number
size_t PlaceMove::getMaxTiles() const{
	return _player->getMaxTiles();
}

size_t PlaceMove::getStringLength() const{
	return _tileString.length();
}

//gets all words formed by a move and places it on the board if the words are in the dictionary and in bounds
//also returns the new score, tiles added, and words formed.
void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary, bool initTrue){
	bool legalWord = true;
	bool validMove = board.validPlaceMove(*this, initTrue);
	vector<pair<string, unsigned int>> words;
	size_t points = 0;
	//checks validty of move in several stages: placement, legality of a word, size of word
	try{
		if(validMove)
			words = board.getPlaceMoveResults(*this);
		else{
			throw MoveException("Error: Word was placed in an invalid location.\nrow: " + to_string(getY()) + " col: " + to_string(getX()));
		}
		if(words.size() == 0){
			throw MoveException("Error: Single letters are not words.");
		}
		for (vector<pair<string, unsigned int>>::iterator it = words.begin(); it != words.end(); it++){
			legalWord = dictionary.isLegalWord(it->first);
			if(legalWord){
				cout << "Word created: " + it->first << endl;
				points += it->second;
			}
			else{
				throw MoveException("Error: Word was not found in the dictionary \"" + it->first + "\"");
			}
		}
		_player->setScore(points);
		cout << "New score: " << _player->getScore() << endl;
		board.executePlaceMove(*this);
		vector<Tile*>newTiles = bag.drawTiles(_playerTiles.size());
		cout << "Tiles added to your hand: ";
		for (vector<Tile*>::iterator it = newTiles.begin(); it != newTiles.end(); it++){
			cout << (*it)->getLetter();
		}
		cout << endl;
		_player->addTiles(newTiles);
		setValidMove(true);

	}
	catch (MoveException &e){
		setValidMove(false);
		if(_player->getType() == "human")
			cout << e.getMessage() << endl;
		_player->addTiles(getPlayerTiles());
	}
}

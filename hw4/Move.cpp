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

PassMove::PassMove(Player * player) : Move(player){
}

ExchangeMove::ExchangeMove(string tileString, Player * p) : Move(p){
	_tileString = tileString;
	try {
		if(_player->hasTiles(tileString, false))
			_playerTiles = _player->takeTiles(_tileString, false);
		else
			throw MoveException("Error: Player does not have tiles: " + _tileString);
		setValidMove(true);
	}
	catch (MoveException &e){
		setValidMove(false);
		cout << e.getMessage() << endl;
	}
	
}

PlaceMove::PlaceMove (size_t x, size_t y, bool horizontal, string tileString, Player * p) : Move(p){
	_x = x;
	_y = y;
	_horizontal = horizontal;
	_tileString = tileString;
	try {
		if(_player->hasTiles(tileString, true))
			_playerTiles = _player->takeTiles(_tileString, true);
		else{
			throw MoveException("Error: Player does not have tiles: " + _tileString);
		}
	}
	catch (MoveException &e){
		setValidMove(false);
		cout << e.getMessage() << endl;
	}
}


Move * Move::parseMove(string moveString, Player &p){
	transform(moveString.begin(), moveString.end(), moveString.begin(), ::tolower);
	cout << moveString << endl;
	stringstream ss(moveString);
	string moveType = "";
	string tileString = "";
	char dir;
	size_t x;
	size_t y;
	bool hor = false;
	ss >> moveType;
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
		throw MoveException("Error: invalid action: " + moveType);
	}
	catch (MoveException &m){
		setValidMove(false);
		cerr << m.getMessage() << endl;
	}
}

void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	return;
}

void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	bag.addTiles(_playerTiles);
	vector<Tile*>newTiles = bag.drawTiles(_playerTiles.size());cout << "Tiles added to your hand:";
	for (vector<Tile*>::iterator it = newTiles.begin(); it != newTiles.end(); it++){
		cout << (*it)->getLetter();
	}
	cout << endl;
	_player->addTiles(newTiles);
	setValidMove(true);
	return;
}

size_t PlaceMove::getX() const{
	return _x;
}

size_t PlaceMove::getY() const{
	return _y;
}

bool PlaceMove::isHorizontal() const{
	return _horizontal;
}
size_t PlaceMove::getMaxTiles() const{
	return _player->getMaxTiles();
}
void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	bool legalWord = true;
	bool validMove = board.validPlaceMove(*this);
	vector<pair<string, unsigned int>> words;
	size_t points = 0;
	try{
		if(validMove)
			words = board.getPlaceMoveResults(*this);
		else{ 
			setValidMove(false);
			throw MoveException("Error: Word was placed in an invalid location.");
		}
		for (vector<pair<string, unsigned int>>::iterator it = words.begin(); it != words.end(); it++){
			legalWord = dictionary.isLegalWord(it->first);
			if(legalWord){
				cout << "Word created: " + it->first << endl;
				points += it->second;
			}
			else{
				throw MoveException("Error: Word was not found in the dictionary: " + it->first);
			}
		}
		_player->setScore(points);
		cout << _player->getScore() << endl;
		board.executePlaceMove(*this);
		vector<Tile*>newTiles = bag.drawTiles(_playerTiles.size());
		cout << "Tiles added to your hand: " << endl;
		for (vector<Tile*>::iterator it = newTiles.begin(); it != newTiles.end(); it++){
			cout << (*it)->getLetter();
		}
		cout << endl;
		_player->addTiles(newTiles);
		setValidMove(true);

	}
	catch (MoveException &e){
		setValidMove(false);
		cout << e.getMessage() << endl;
		_player->addTiles(getPlayerTiles());
	}
}
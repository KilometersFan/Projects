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
			throw MoveException("Player does not have tiles: " + _tileString);
	}
	catch (MoveException &e){
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
		else
			throw MoveException("Player does not have tiles: " + _tileString);
	}
	catch (MoveException &e){
		cout << e.getMessage() << endl;
	}
}


Move * Move::parseMove(string moveString, Player &p){
	transform(moveString.begin(), moveString.end(), moveString.begin(), ::tolower);
	stringstream ss(moveString);
	string moveType = "";
	string tileString = "";
	char dir;
	size_t x;
	size_t y;
	bool hor = false;
	ss >> moveType;
	if(moveType == "pass"){
		return new PassMove(&p);
	}
	else if (moveType == "exchange"){
		ss >> tileString;
		return new ExchangeMove(tileString, &p);
	}
	else {
		ss >> dir;
		ss >> x;
		ss >> y;
		ss >> tileString;
		if(dir == '-')
			hor = true;
		return new PlaceMove(x, y, hor, tileString, &p);
	}
}

void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	return;
}

void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	bag.addTiles(_playerTiles);
	_player->addTiles(bag.drawTiles(_playerTiles.size()));
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

void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	bool legalWord = true;
	vector<pair<string, unsigned int>> words = board.getPlaceMoveResults(*this);
	try{
		for (vector<pair<string, unsigned int>>::iterator it = words.begin(); it != words.end(); it++){
			legalWord = dictionary.isLegalWord(it->first);
			cout <<"LEGAL WORD IS: " <<it->first << endl;
			if(!legalWord || !board.validPlaceMove(*this))
				throw MoveException("Invalid Move!");
		}
		cout << "Valid Move!" << endl;
		board.executePlaceMove(*this);
		_player->addTiles(bag.drawTiles(_playerTiles.size()));
	}
	catch (MoveException &e){
		cout << e.getMessage() << endl;
		_player->addTiles(getPlayerTiles());
	}
}
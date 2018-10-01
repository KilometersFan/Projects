#include "Move.h"
#include <sstream>
#include <algorithm>
#include <locale>
#include <string>
using namespace std;
Move::~Move(){

}

Move::Move(Player * player) {
	_player = player;
}

PassMove::PassMove(Player * player) : Move(player){
	_player = player;
}

ExchangeMove::ExchangeMove(std::string tileString, Player * p) : Move(p){
	_tileString = tileString;
}

PlaceMove::PlaceMove (size_t x, size_t y, bool horizontal, std::string tileString, Player * p) : Move(p){
	_x = x;
	_y = y;
	_horizontal = horizontal;
	_tileString = tileString;
}


Move * Move::parseMove(std::string moveString, Player &p){
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
	
}

void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary){

}
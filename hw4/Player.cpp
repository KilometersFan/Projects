#include "Player.h"
#include <iostream>
#include <locale>
using namespace std;

/* Constructor giving the player the given name, and setting their points to 0.
Does not give the player any tiles.
*/
Player::Player (string const & name, size_t maxTiles){
	_name = name;
	_tileCount = maxTiles;
	_score = 0;
	
}

/* Destructor for a player. Deletes all the tiles the player still has. */
Player::~Player (){
	if(_hand.size()){
		for (set<Tile*>::iterator it = _hand.begin(); it != _hand.end(); it++){
			delete *it;
		}
	}
}

/* Returns the set of tiles the player currently holds. */
set<Tile*> Player::getHandTiles() const {
	return _hand;
}

/* Reads a move string and confirms that the player has the tiles necessary to
execute this move.

The move string is case-insensitive.

The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
If resolveBlanks is true, then when the string contains a '?',
the next letter is interpreted as the letter to use the blank as.

By definition, if this function returns true, then takeTiles() would
succeed.
*/

bool Player::hasTiles(string const & move, bool resolveBlanks) const{
	bool hasTile = false;
	set<Tile*> temp = _hand;
	for(unsigned int i = 0; i < move.length(); i++){
		hasTile = false;
		for(set<Tile*>::iterator it = temp.begin(); it != temp.end(); it++){
			if(!resolveBlanks){
				if(move[i] == (*it)->getLetter()){
					hasTile = true;
					temp.erase(it);
					break;
				}
			}
			else {
				if(move[i] == (*it)->getLetter()){
					if(move[i] == '?'){
						++i;
					}
					hasTile = true;
					temp.erase(it);
					break;
				}
			}
		}
		if(!hasTile)
			return hasTile;
	}
	return hasTile;
}


/* Reads a move string, finds the corresponding tiles in the player's hand, and
removes and returns those tiles in the order they appear in the move string.

The move string is case-insensitive.

The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
If resolveBlanks is true, then when the string contains a '?',
the next letter is interpreted as the letter to use the blank as,
and the "use" field of the tile is set accordingly.

The move string is assumed to have correct syntax.
*/
vector<Tile*> Player::takeTiles (string const & move, bool resolveBlanks){
	vector<Tile*> tilesToRemove;
	for(unsigned int i = 0; i < move.length(); i++){
		for(set<Tile*>::iterator it = _hand.begin(); it != _hand.end(); it++){
			if(!resolveBlanks){
				if(move[i] == (*it)->getLetter()){
					tilesToRemove.push_back(*it);
					_hand.erase(it);
					break;
				}
			}
			else {
				if(move[i] == (*it)->getLetter()){
					if(move[i] == '?'){
						(*it)->useAs(move[i+1]);
						++i;
					}
					tilesToRemove.push_back(*it);
					_hand.erase(it);
					break;
				}
			}
		}
	}
	return tilesToRemove;
}

// Adds all the tiles in the vector to the player's hand.
void Player::addTiles (vector<Tile*> const & tilesToAdd){
	for (size_t i = 0; i < tilesToAdd.size(); i++){
		_hand.insert(tilesToAdd[i]);
	}
}
//returns max hand size
size_t Player::getMaxTiles() const{
	return _tileCount;
}
//gets player name
string Player::getName() const {
	return _name;
}
//gets player score
size_t Player::getScore() const {
	return _score;
}
//adds to player score
void Player::setScore(size_t points) {
	_score += points;
}
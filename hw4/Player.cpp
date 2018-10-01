#include "Player.h"
#include <iostream>
#include <locale>
using namespace std;

/* Constructor giving the player the given name, and setting their points to 0.
Does not give the player any tiles.
*/
Player::Player (std::string const & name, size_t maxTiles): _tileCount(maxTiles){
	_name = name;
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
std::set<Tile*> Player::getHandTiles() const {
	for (set<Tile*>::iterator it = _hand.begin(); it != _hand.end(); it++){
			std::cout << *it << std::endl;
		}
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

bool Player::hasTiles(std::string const & move, bool resolveBlanks) const{
	return true;
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
std::vector<Tile*> Player::takeTiles (std::string const & move, bool resolveBlanks){
	std::vector<Tile*> myvect;
	return myvect;
}

// Adds all the tiles in the vector to the player's hand.
void Player::addTiles (std::vector<Tile*> const & tilesToAdd){

}
size_t Player::getMaxTiles() const{
	return _tileCount;
}
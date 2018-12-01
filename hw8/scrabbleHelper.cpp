#include "scrabble.h"
using namespace std;
void getHorizontalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& startCoords) {
	size_t xMax = board.getColumns();
	size_t yMax = board.getRows();
	//loops over every square on the board
	for(size_t x = 1; x <= xMax; x++){
		for(size_t y = 1; y <= yMax; y++){
			//for every occupied square, find all spaces a player can build off of to connect with this square
			if(board.getSquare(x,y)->isOccupied()){
				size_t i = 1;
				unsigned int count = 0;
				//gets all empty coords left of the square
				while(x - i > 0 && count != length){
					if(!board.getSquare(x - i, y)->isOccupied()){
						startCoords.insert(make_pair(x-i, y));
						count++;
					}
					i++;
				}
				//gets coords 1 right of the coord
				if(x + 1 <= xMax){
					if(!board.getSquare(x+1, y)->isOccupied()){
						startCoords.insert(make_pair(x+1, y));
					}
				}
				size_t j = 0;
				count = 0;
				//gets coords one above and left of the square
				if(y - 1 > 0){
					if(!board.getSquare(x, y-1)->isOccupied()){
						while(x - j > 0 && count != length){
							if(!board.getSquare(x-j, y -1)->isOccupied()){
								startCoords.insert(make_pair(x-j,y-1));
								count++;
							}
							j++;
						}
					}
				}
				j=0;
				//gets coords one below and leff
				if(y + 1 <= yMax){
					if(!board.getSquare(x, y+1)->isOccupied()){
						while(x - j > 0 && count != length){
							if(!board.getSquare(x-j, y +1)->isOccupied()){
								startCoords.insert(make_pair(x-j,y+1));
								count++;
							}
							j++;
						}
					}
				}
			}
		}
	}
}
void getVerticalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& startCoords) {
	size_t xMax = board.getColumns();
	size_t yMax = board.getRows();
	//loops over every square on the board
	for(size_t y = 1; y <= yMax; y++){
		for(size_t x = 1; x <= xMax; x++){
			//for every occupied square, find all spaces a player can build off of to connect with this square
			if(board.getSquare(x,y)->isOccupied()){
				size_t i = 1;
				unsigned int count = 0;
				//gets all empty coords above the square
				while(y - i > 0 && count != length){
					if(!board.getSquare(x, y- i)->isOccupied()){
						startCoords.insert(make_pair(x, y- i));
						count++;
					}
					i++;
				}
				size_t j = 0;
				count = 0;
				//gets coords 1 below the coord
				if(y + 1 <= yMax){
					if(!board.getSquare(x, y+1)->isOccupied()){
						startCoords.insert(make_pair(x, y+1));
					}
				}
				//gets coords one left and above of the square
				if(x - 1 > 0){
					if(!board.getSquare(x-1, y)->isOccupied()){
						while(y - j > 0 && count != length){
							if(!board.getSquare(x-1, y -j)->isOccupied()){
								startCoords.insert(make_pair(x-1,y-j));
								count++;
							}
							j++;
						}
					}
				}
				j=0;
				//gets coords right of the square
				if(x + 1 <= xMax){
					if(!board.getSquare(x+1, y)->isOccupied()){
						while(y - j > 0 && count != length){
							if(!board.getSquare(x+1, y - j)->isOccupied()){
								startCoords.insert(make_pair(x+1,y - j));
								count++;
							}
							j++;
						}
					}
				}
			}
		}
	}
}
void findHorizontalMovesHelper(set<Tile*>hand, size_t x, size_t y, TrieSet& trie, Board& board, TrieNode* node, string move, set<string>& moves){
	if(node == nullptr || x > board.getColumns() || y > board.getRows())
		return;
	//checks if word is in the trie and either the ai is on a blank square or at the edge fo the board
	if(node->inSet && (x <= board.getColumns() || !board.getSquare(x, y)->isOccupied())){
		unsigned int prefixCount = 0;
		if(x == board.getColumns()){
			//adds last letter to the move, checks it for legality
			if(board.getSquare(x, y)->isOccupied()){
				char letter = board.getSquare(x, y)->getLetter();
				if(node->children[letter - 'a'] != nullptr){
					if(node->inSet)
						moves.insert(move);
				}
				return;
			}
			//cycles through tiles to see if a word can be made
			else {
				for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
					char letter = (*it)->getLetter();
					if(letter == '?'){
						for(int i = 0; i < 26; i++){
							char blankLetter = 'a' + i;
							TrieNode* temp = node->children[i];
							(*it)->useAs('a'+i);
							bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, true);
							if(temp && adjacentWord){
								if(temp->inSet)
									moves.insert(move+letter+blankLetter);
							}
						}
					}
					else {
						TrieNode* temp = node->children[letter - 'a'];
						if(temp){
							if(temp->inSet)
								moves.insert(move+letter);
						}
					}
				}
				return;
			}
		}
		//sees if there is still room for words, continues if so
		else{
			for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
				char letter = (*it)->getLetter();
				if(!node->children[letter - 'a'])
					prefixCount++;
			}
			if(prefixCount == hand.size())
				moves.insert(move);
			}
	}
	Square* currLocation = board.getSquare(x, y);
	//adds letter on square to word
	if(currLocation->isOccupied()){
		char letter = currLocation->getLetter();
		int index = letter - 'a';
		findHorizontalMovesHelper(hand, x+1, y, trie,board, node->children[index], move, moves);
	}
	//adds one of the player's tiles to word and move list
	else {
		for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
			char letter = (*it)->getLetter();
			set<Tile*> tempHand = hand;
			if(letter == '?'){
				for(int i = 0; i < 26; i++){
					char blankLetter = (char)('a' + i);
					(*it)->useAs(blankLetter);
					bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, true);
					if(adjacentWord){
						tempHand.erase(*it);
						findHorizontalMovesHelper(tempHand, x+1, y, trie,board, node->children[i], move+letter+blankLetter, moves);
					}
				}
			}
			else{
				int index = letter - 'a';
				bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, true);
				if(adjacentWord){
					tempHand.erase(*it);
					findHorizontalMovesHelper(tempHand, x+1, y, trie,board, node->children[index], move+letter, moves);
				}
			}
		}
	}
}
void findVerticalMovesHelper(set<Tile*>hand, size_t x, size_t y, TrieSet& trie, Board& board, TrieNode* node, string move, set<string>& moves){
	if(node == nullptr || x > board.getColumns() || y > board.getRows())
		return;
	//checks if word is in the trie and either the ai is on a blank square or at the edge of the board
	if(node->inSet && (y <= board.getRows() || !board.getSquare(x, y)->isOccupied())){
		unsigned int prefixCount = 0;
		if(y == board.getRows()){
			//adds last letter to the move, checks it for legality
			if(board.getSquare(x, y)->isOccupied()){
				char letter = board.getSquare(x, y)->getLetter();
				if(node->children[letter - 'a'] != nullptr){
					if(node->inSet)
						moves.insert(move);
				}
				return;
			}
			else {
				//cycles through tiles to see if a word can be made
				for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
					char letter = (*it)->getLetter();
					if(letter == '?'){
						for(int i = 0; i < 26; i++){
							char blankLetter = 'a' + i;
							TrieNode* temp = node->children[i];
							(*it)->useAs('a'+i);
							bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, false);
							if(temp && adjacentWord){
								if(temp->inSet)
									moves.insert(move+letter+blankLetter);
							}
						}
					}
					else {
						TrieNode* temp = node->children[letter - 'a'];
						if(temp){
							if(temp->inSet)
								moves.insert(move+letter);
						}
					}
				}
				return;
			}
		}
		//sees if there is still room for words, continues if so
		else{
			for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
				char letter = (*it)->getLetter();
				if(!node->children[letter - 'a'])
					prefixCount++;
			}
			if(prefixCount == hand.size())
				moves.insert(move);
		}
	}
	Square* currLocation = board.getSquare(x, y);
	//adds letter on square to word
	if(currLocation->isOccupied()){
		char letter = currLocation->getLetter();
		int index = letter - 'a';
		findVerticalMovesHelper(hand, x, y+1, trie,board, node->children[index], move, moves);
	}
	//adds one of the player's tiles to word and move list
	else {
		for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
			char letter = (*it)->getLetter();
			set<Tile*> tempHand = hand;
			if(letter == '?'){
				for(int i = 0; i < 26; i++){
					char blankLetter = (char)('a' + i);
					(*it)->useAs(blankLetter);
					bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, false);
					if(adjacentWord){
						tempHand.erase(*it);
						findVerticalMovesHelper(tempHand, x+1, y, trie,board, node->children[i], move+letter+blankLetter, moves);
					}
				}
			}
			else {
				int index = letter - 'a';
				bool adjacentWord = validAdjacentWord(x, y, *it, board, trie, false);
				if(adjacentWord){
					tempHand.erase(*it);
					findVerticalMovesHelper(tempHand, x, y+1, trie,board, node->children[index], move+letter, moves);
				}
			}
		}
	}
}

size_t findStartLocation(size_t x, size_t y, Board& board, bool horizontal){
	int i = 0;
	if(horizontal){
		while(x-i -1 >0){
			if(!board.getSquare(x - i - 1, y)->isOccupied())
				break;
			i++;
		}
		return x - i;
	}
	else {
		while(y-i -1 >0){
			if(!board.getSquare(x, y- i - 1)->isOccupied())
				break;
			i++;
		}
		return y - i;
	}
}

bool validAdjacentWord(size_t x, size_t y, Tile* tile, Board& board, TrieSet& trie, bool horizontal){
	if(horizontal){
		size_t newY = findStartLocation(x, y, board, !horizontal);
		bool hasTilesUnder = false;
		if(y+1 <= board.getRows()){
			if(board.getSquare(x, y+1)->isOccupied())
				hasTilesUnder = true;
		}
		//traverse upwards and downwards to see if there's an adjacent word
		if(newY != y || hasTilesUnder){
			TrieNode* node = trie.prefix("");
			int i = 0;
			bool usedTile = false;
			while(newY + i <= board.getRows() && node != nullptr){
				if(board.getSquare(x, newY + i)->isOccupied()){
					char letter = board.getSquare(x, newY + i)->getLetter();
					node = node->children[letter - 'a'];
				}
				else{
					if(!usedTile){
						char letter = tile->getUse();
						node = node->children[letter - 'a'];
						usedTile = true;
					}
					else
						break;
				}
				i++;
			}
			//returns true if adjacent word is in the trie
			if(node){
				if(node->inSet)
					return true;
			}
			return false;
		}
		else
			return true;
	}
	else {
		size_t newX = findStartLocation(x, y, board, !horizontal);
		bool hasTilesUnder = false;
		if(x+1 <= board.getRows()){
			if(board.getSquare(x+1, y)->isOccupied())
				hasTilesUnder = true;
		}
		//traverse left and right to see if there's an adjacent word
		if(newX != x || hasTilesUnder){
			TrieNode* node = trie.prefix("");
			int i = 0;
			bool usedTile = false;
			while(newX + i <= board.getColumns() && node != nullptr){
				if(board.getSquare(newX + i, y)->isOccupied()){
					char letter = board.getSquare(newX+i, y)->getLetter();
					node = node->children[letter - 'a'];
				}
				else{
					if(!usedTile){
						char letter = tile->getUse();
						node = node->children[letter - 'a'];
						usedTile = true;
					}
					else
						break;
				}
				i++;
			}
			//returns true if adjacent word is in the trie
			if(node){
				if(node->inSet)
					return true;
			}
			return false;
		}
		else
			return true;
	}
}

string returnMaxPointMove(set<string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie){
	size_t maxPoints = 0;
	Move* temp = nullptr;
	string finalMove = "";
	//iterates over collected moves to find best move
	for(set<string>::iterator it = moves.begin(); it != moves.end(); it++){
		stringstream ss(*it);
		string buff;
		for(int i = 0; i < 6; i++)
			ss >> buff;
		//checks move length
		size_t tilesPlaced = buff.length() - count(buff.begin(), buff.end(), '?');
		if(tilesPlaced == 0)
			continue;
		temp = Move::parseMove(*it, player);
		size_t points = 0;
		//check move placement
		bool validMove = board.validPlaceMove(*dynamic_cast<PlaceMove*>(temp), firstmove);
		if(!validMove){
			returnCPUTiles(player, temp);
			continue;
		}
		//checks words formed
		vector<pair<string, unsigned int>> words = board.getPlaceMoveResults(*dynamic_cast<PlaceMove*>(temp));
		if(!words.size()){
			returnCPUTiles(player, temp);
			continue;
		}
		bool legalWord = false;
		for (vector<pair<string, unsigned int>>::iterator it = words.begin(); it != words.end(); it++){
			legalWord = trie.inTrie(it->first);
			if(legalWord){
				points += it->second;
			}
			else
				break;
		}
		if(!legalWord){
			returnCPUTiles(player, temp);
			continue;
		}
		returnCPUTiles(player, temp);
		if(tilesPlaced == player.getMaxTiles())
			points+= 50;
		//checks points vs maxPoints
		if(points >= maxPoints && validMove && legalWord){
			maxPoints = points;
			finalMove = *it;
		}
		returnCPUTiles(player, temp);
		delete temp;
	}
	return finalMove;
}
string returnMaxLengthMove(set<string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie){
	size_t maxlen = 0;
	Move* temp = nullptr;
	string finalMove = "";
	//iterates over collected moves
	for(set<string>::iterator it = moves.begin(); it != moves.end(); it++){
		stringstream ss(*it);
		string buff;
		for(int i = 0; i < 6; i++)
			ss >> buff;
		size_t tilesPlaced = buff.length() - count(buff.begin(), buff.end(), '?');
		if(tilesPlaced < maxlen)
			continue;
		//checks move length
		if(tilesPlaced == 0)
			continue;
		temp = Move::parseMove(*it, player);
		//checks move placement
		bool validMove = board.validPlaceMove(*dynamic_cast<PlaceMove*>(temp), firstmove);
		if(!validMove){
			returnCPUTiles(player, temp);
			continue;
		}
		//checks word legality
		vector<pair<string, unsigned int>> words = board.getPlaceMoveResults(*dynamic_cast<PlaceMove*>(temp));
		if(!words.size()){
			returnCPUTiles(player, temp);
			continue;
		}
		bool legalWord = false;
		for (vector<pair<string, unsigned int>>::iterator it = words.begin(); it != words.end(); it++){
			legalWord = trie.inTrie(it->first);
			if(!legalWord){
				break;
			}
		}
		//checks length against maxLength
		if(tilesPlaced >= maxlen && legalWord && validMove){
			maxlen = tilesPlaced;
			finalMove = *it;
			returnCPUTiles(player, temp);
		}
		else
			returnCPUTiles(player, temp);
		delete temp;
	}
	return finalMove;
}

void returnCPUTiles(Player& player, Move* m){
	player.addTiles(m->getPlayerTiles());
}
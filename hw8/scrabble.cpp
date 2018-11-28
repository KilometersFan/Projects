#include "scrabble.h"
using namespace std;
string returnMaxPointMove(set<string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie);
void returnCPUTiles(Player& player, Move* m);
string returnMaxLengthMove(set<string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie);
void blankTileStrings(string str, int pos, set<pair<size_t, size_t>>& horCoordinateBank, set<pair<size_t, size_t>>& vertCoordinateBank, Board& board, vector<pair<string, int>>& moves, TrieSet& trie, bool &firstmove);
bool hasValidAdjacentWords(size_t x, size_t y, Board& board, bool horizontal, char letter, TrieSet& trie);
void getStartOfWord(size_t &x, size_t &y, Board& board, bool horizontal);
int main(int argc, char const *argv[])
{
	try{
		// Retrieve config file
		// if(argc < 2){
		// 	throw FileException("No file specified.");
		// }
		ifstream ifile("config.txt");
		if(ifile.fail()){
			throw FileException("Unable to open file.");
		}
		//Read in config file information to set up game
		size_t maxTiles = 0;
		string boardFile = "";
		string bagFile = "";
		string dictionaryFile = "";
		string initFile = "";
		uint32_t seed = 0;
		string buff;
		while (ifile >> buff){
			if(buff == "HANDSIZE:")
				ifile >> maxTiles;
			else if (buff == "TILES:")
				ifile >> bagFile;
			else if (buff == "DICTIONARY:")
				ifile >> dictionaryFile;
			else if (buff == "BOARD:")
				ifile >> boardFile;
			else if(buff == "SEED:")
				ifile >> seed;
			else if (buff == "INIT:")
				ifile >> initFile;
		}	
		ifile.close();
		Dictionary dictionary(dictionaryFile);
		TrieSet trie;
		for(set<string>::iterator it = dictionary.dictionary.begin(); it != dictionary.dictionary.end(); it++)
			trie.insert(*it);
		Board board(boardFile);
		Bag bag = Bag(bagFile, seed);
		ConsolePrinter console;
		set<pair<size_t, size_t>> occupiedCoords;
		bool firstmove = initializeBoard(initFile, board, occupiedCoords);
		//Add payers and starting hands
		cout << "Hello! Welcome to Scrabble. Please enter the number of players in the game (max 8): ";
		int numPlayers;
		cin >> numPlayers;
		cin.ignore(10000, '\n');
		cin.clear();
		while ( numPlayers < 1){
			cout << "Please enter a number >= 1" << endl;
			cin >> numPlayers;
			cin.ignore(10000, '\n');
			cin.clear();
		}
		vector<Player*> players;
		for (int i = 0; i < numPlayers; i++){
			string name;
			cout << "Please tell me your name: ";
			getline(cin, name);
			for(int i = 0; i < name.length(); i++)
				name[i] = toupper(name[i]);
			Player* player = NULL;
			if(name.find("CPUS") != string::npos)
				player = new BotPlayer(name, "cpus", maxTiles);
			else if (name.find("CPUL") != string::npos)
				player = new BotPlayer(name, "cpul" ,maxTiles);
			else 
				player = new Player(name, maxTiles);
			players.push_back(player);
			vector <Tile*> initialHand = bag.drawTiles(maxTiles);
			player->addTiles(initialHand);
		}
		//start game
		scrabble(players, board, occupiedCoords, bag, dictionary, console, firstmove, trie);
		for(vector<Player*>::iterator it = players.begin(); it != players.end(); it++){
			delete *it;
		}
	}
	catch (FileException &e){
		cout << e.getMessage() << endl;
	}	

	return 0;
	
}
void scrabble(vector<Player*> &players, Board &board, set<pair<size_t, size_t>>& occupiedCoords, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie){
	bool gameOver = false;
	int pos = -1;
	while (!gameOver){
		unsigned int passCount = 0;
		bool emptyHand = false;
		set<pair<size_t, size_t>> horCoordinateBank;
		set<pair<size_t, size_t>> vertCoordinateBank;
		//This loop represents a round for each player 1 to k
		for (unsigned int i = 0; i < players.size(); i++){
			//prints out scores, boad, and the player's hand
			console.printBoard(board);
			console.printHand(*(players[i]));
			cout << "SCORES:\n";
			for(unsigned int j = 0; j < players.size(); j++){
				cout << (players[j])->getName() + ": " << (players[j]->getScore()) << endl;
			}
			cout << endl;
			Move* m = nullptr;
			//Bot moves
			if(players[i]->getType() != "human"){
				string handLetters = players[i]->getLetters();
				if(occupiedCoords.size() > 0){
					size_t size = players[i]->getHandTiles().size();
					getHorizontalStartCoords(size, board, occupiedCoords, horCoordinateBank);
					getVerticalStartCoords(size, board, occupiedCoords, vertCoordinateBank);
				}
				else {
					pair<size_t, size_t> sc = board.getStartCoords();
					horCoordinateBank.insert(sc);
					vertCoordinateBank.insert(sc);
				}
				string move = "";
				TrieNode* root = trie.prefix("");
				cout << root << endl;
				set<string> moves;
				for(set<pair<size_t, size_t>>::iterator it = horCoordinateBank.begin(); it != horCoordinateBank.end(); it++){
					size_t x = it->first; size_t y = it->second;
					string move = "place - " + to_string(y) + " " + to_string(x) + " ";
					getStartOfWord( x,  y, board, true);
					getValidHorizontalMoves(players[i]->getHandTiles(), x, y, board, "", move, moves, trie, firstmove, root);
				}
				for(set<pair<size_t, size_t>>::iterator it = vertCoordinateBank.begin(); it != vertCoordinateBank.end(); it++){
					size_t x = it->first; size_t y = it->second;
					string move = "place | " + to_string(y) + " " + to_string(x) + " ";
					getStartOfWord( x,  y, board, false);
					getValidVerticalMoves(players[i]->getHandTiles(), x, y, board, "", move, moves, trie, firstmove, root);
				}
				// retrieve final move
				string finalMove = "";
				if(players[i]->getType() == "cpus")
					finalMove = returnMaxPointMove(moves, *(players[i]), board, firstmove, trie);
				else
					finalMove = returnMaxLengthMove(moves, *(players[i]), board, firstmove, trie);
				cout << "FINAL MOVE: " << finalMove << endl;
				if(!finalMove.length())
					m = Move::parseMove("pass", *(players[i]));
				else
					m = Move::parseMove(finalMove, *(players[i]));
				m->execute(board, bag, dictionary, firstmove);
			}
			//human moves
			else {
				string move;
				cout << "Hello " + players[i]->getName() + " what would you like to do?" << endl;
				cout << "Examples: \nPASS\nEXCHANGE <tile string>\nPLACE <dir> <row> <col> <tile string>" << endl;
				getline(cin, move);
				//creates move based on player input. If malformed, asks again until well formed 
				m = Move::parseMove(move, *(players[i]));
				while(!m->isValidMove()){
					validMoveCheck(move);
					m = Move::parseMove(move, *(players[i]));
					move = "";
				}
				m->execute(board, bag, dictionary, firstmove);
				while(!m->isValidMove()){
					validMoveCheck(move);
					delete m;
					m = nullptr;
					m = Move::parseMove(move, *(players[i]));
					m->execute(board, bag, dictionary, firstmove);
					move = "";
				}
			} 
			console.printBoard(board);
			if(m->isPass())
				passCount++;
			//adds occupied coords to a list for use in AI turns
			if(m->isWord()){
				if((dynamic_cast<PlaceMove*>(m))->isHorizontal()){
					for(unsigned int i = 0; i < (dynamic_cast<PlaceMove*>(m))->getStringLength(); i++){
						pair<size_t, size_t> temp = make_pair((dynamic_cast<PlaceMove*>(m))->getX() + i, (dynamic_cast<PlaceMove*>(m))->getY());
						occupiedCoords.insert(temp);
						if(horCoordinateBank.find(temp)!= horCoordinateBank.end())
							horCoordinateBank.erase(temp);
						if(vertCoordinateBank.find(temp)!= vertCoordinateBank.end())
							vertCoordinateBank.erase(temp);
					}
				}
				else {
					for(unsigned int i = 0; i < (dynamic_cast<PlaceMove*>(m))->getStringLength(); i++){
						pair<size_t, size_t> temp = make_pair((dynamic_cast<PlaceMove*>(m))->getX(), (dynamic_cast<PlaceMove*>(m))->getY() + i);
						occupiedCoords.insert(temp);
						if(horCoordinateBank.find(temp)!= horCoordinateBank.end())
							horCoordinateBank.erase(temp);
						if(vertCoordinateBank.find(temp)!= vertCoordinateBank.end())
							vertCoordinateBank.erase(temp);
					}
				}
			}
			if(players[i]->getHandTiles().size() == 0){
				emptyHand = true;
				pos = i;
			}
			cout << "Please press enter to end your turn." << endl;
			cin.ignore(10000, '\n');
			delete m;
			firstmove = false;
		}
		if(passCount == players.size() || emptyHand)
			gameOver = true;
	}
	cout << "Thanks for playing!" << endl;
	int tileSum = 0;
	//subtracts the sum of points for each tile a player has at the end of the game.
	for (unsigned int i = 0; i < players.size(); i++){
		set<Tile*> playerHand =  players[i]->getHandTiles();
		for(set<Tile*>::iterator it=playerHand.begin(); it != playerHand.end(); it++){
			int playerScore = static_cast<int>(players[i]->getScore());
			int tileScore = static_cast<int>((*it)->getPoints());
			tileSum += tileScore;
			if(playerScore - tileScore <= 0)
				players[i]->setScore(-1 * playerScore);
			else 
				players[i]->setScore(-1 * tileScore);
		}
	}
	//adds the sum of subtracted points to a player who has no tiles in their hand
	if(pos >= 0)
		players[pos]->setScore(tileSum);
	//finds the winner(s) from the new scores
	size_t max = players[0]->getScore();
	vector<Player*> winners;
	for (unsigned int i = 0; i < players.size(); i++){
		cout << players[i]->getName() << " " << players[i]->getScore() << endl;
		if(players[i]->getScore() >= max)
			max = players[i]->getScore();
	}
	for (unsigned int i = 0; i < players.size(); i++){
		if(players[i]->getScore() == max)
			winners.push_back(players[i]);
	}

	if(winners.size() > 1)
		cout << "Winners: ";
	else 
		cout << "Winner: ";
	for (unsigned int i = 0; i < winners.size(); i++){
		cout << winners[i]->getName() + " ";
	}
	cout << endl;

}

void validMoveCheck(string &str){
	cout << "Please enter a valid move: ";
	getline(cin, str);
	cin.clear();
}
bool initializeBoard(string &initFile, Board &board, set<pair<size_t, size_t>>& occupiedCoords){
	if(initFile.length() == 0)
		return false;
	ifstream ifile(initFile);
	size_t row = 1;
	string line;
	while (getline(ifile,line)){
		for(size_t i = 0; i < line.length(); i+=3){
			if(isalpha(line[i])){
				line[i] = tolower(line[i]);
				string _points = "";
				_points += line[i+1];
				_points +=  line[i+2];
				int points = stoi(_points);
				Square* square = board.getSquare(i/3 +1, row);
				Tile* tile = new Tile(line[i], points);
				square->placeTile(tile);
				occupiedCoords.insert(make_pair(i/3+1, row));
			}
		}
		row++;
	}
	return true;
}

void getHorizontalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords, set<pair<size_t, size_t>>& startCoords) {
	for (set<pair<size_t, size_t>>::iterator it = occupiedCoords.begin(); it != occupiedCoords.end(); it++){
		size_t _length = length;
		size_t x = it->first;
		size_t y = it->second;
		//go left until you hit the edge of board or an occupied square
		size_t i = 0;
		while(x - i - 1 > 0 && _length > 0){
			if(board.getSquare(x - i - 1, y)->isOccupied())
				break;
			i++;
			_length--;
		}			
		//add each empty square to start coord set (dupes will not be added)
		while(i != 0){
			startCoords.insert(make_pair(x-i, y));
			i--;
		}
		_length = length;
		size_t j = 0;
		while(x + j + 1 <= board.getColumns() && _length > 0){
			if(!board.getSquare(x + j + 1, y)->isOccupied())
				break;
			j++;
			_length--;
		}
		if(y-1 > 0){
			if(!board.getSquare(x, y-1)->isOccupied()){
				startCoords.insert(make_pair(x, y-1));
			}
		}
		else if(y+1 <= board.getRows()){
			if(!board.getSquare(x, y+1)->isOccupied()){
				startCoords.insert(make_pair(x, y+1));
			}
		}
		if(x + j + 1 <= board.getColumns()){
			if(!board.getSquare(x+j+1, y)->isOccupied())
				startCoords.insert(make_pair(x + j + 1, y));
		}
	}
}
void getVerticalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords, set<pair<size_t, size_t>>& startCoords) {
	for(set<pair<size_t, size_t>>::iterator it = occupiedCoords.begin(); it != occupiedCoords.end(); it++){
		size_t x = it->first;
		size_t y = it->second;
		size_t _length = length;
		size_t i = 0;
		while(y - i - 1 > 0 && _length > 0){
			if(board.getSquare(x, y - i - 1)->isOccupied())
				break;
			i++;
			_length--;
		}			
		while(i != 0){
			startCoords.insert(make_pair(x, y-i));
			i--;
		}
		_length = length;
		size_t j = 0;
		while(y + j + 1 <= board.getRows() && _length > 0){
			if(!board.getSquare(x, y + j + 1)->isOccupied())
				break;
			j++;
			_length--;
		}
		if(x-1 > 0){
			if(!board.getSquare(x-1, y)->isOccupied()){
				startCoords.insert(make_pair(x-1, y));
			}
		}
		else if(x+1 <= board.getColumns()){
			if(!board.getSquare(x+1, y)->isOccupied()){
				startCoords.insert(make_pair(x+1, y));
			}
		}
		if(y + j + 1 <= board.getRows()){
			if(!board.getSquare(x, y+j+1)->isOccupied())
				startCoords.insert(make_pair(x, y + j + 1));
		}
	}
}

void getValidHorizontalMoves(set<Tile*> hand, size_t x, size_t y, Board& board, string word, string move, set<string>& validMoves, TrieSet& trie, bool connected, TrieNode* node){
	if((node == nullptr || x > board.getColumns() || y > board.getRows()) || x < 0 || y < 0)
		return;
	if(node->inSet && connected && x <= board.getColumns() && y <= board.getRows() && word.length() > 1){
		if(x+1 <= board.getColumns()){
			Square* temp = board.getSquare(x+1, y);
			if(!temp->isOccupied()){
				cout << move << endl;
				validMoves.insert(move);
			}
		}
		else{
			if(word.length())
				validMoves.insert(move);
			cout << move << endl;
		}
	}
	Square* s = board.getSquare(x,y);
	char letter;
	int points = 0;
	if(s->isOccupied()){
		letter = s->getLetter();
		node = node->children[letter-'a'];
		connected = true;
		getValidHorizontalMoves(hand, x+1, y, board, word+letter, move, validMoves,trie, connected, node);
	}
	else {
		if(s->isStart())
			connected = true;
		for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
			letter = (*it)->getLetter();
			set<Tile*> temp = hand;
			temp.erase(*it);
			if(letter == '?'){
				for(int i = 0; i < 26; i++){
					TrieNode* child = node->children[i];
					if(y-1 > 0){
						if(board.getSquare(x, y-1)->isOccupied()&& !connected)
							connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
					}
					else if (y + 1 <= board.getColumns()){
						if(board.getSquare(x, y+1)->isOccupied()&& !connected)
							connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
					}
					getValidHorizontalMoves(temp, x+1, y, board, word+(char)('a'+i), move + letter+(char)('a'+i), validMoves, trie, connected, child);
				}
			}
			else {
				TrieNode* child = node->children[letter-'a'];
				if(y-1 > 0){
					if(board.getSquare(x, y-1)->isOccupied() && !connected)
						connected = hasValidAdjacentWords(x, y, board, true, letter, trie);
				}
				else if (y + 1 <= board.getRows()){
					if(board.getSquare(x, y+1)->isOccupied()&& !connected)
						connected = hasValidAdjacentWords(x, y, board, true, letter, trie);
				}
				getValidHorizontalMoves(temp, x+1, y, board, word+letter, move + letter, validMoves, trie, connected, child);
			}

		}
	}
}

void getValidVerticalMoves(set<Tile*> hand, size_t x, size_t y, Board& board, string word, string move, set<string>& validMoves, TrieSet& trie, bool connected, TrieNode* node){
	// int num = 0;
	if((node == nullptr || x > board.getColumns() || y > board.getRows())|| x < 0 || y < 0)
		return;
	if(node->inSet && connected && x <= board.getColumns() && y <= board.getRows() && word.length() > 1){
		if(y+1 <= board.getRows()){
			Square* temp = board.getSquare(x, y+1);
			if(!temp->isOccupied()){
				cout << move << endl;
				validMoves.insert(move);
			}
		}
		else{
			if(word.length())
				validMoves.insert(move);
		}
	}
	Square* s = board.getSquare(x,y);
	char letter;
	int points = 0;
	if(s->isOccupied()){
		letter = s->getLetter();
		node = node->children[letter-'a'];
		connected = true;
		getValidVerticalMoves(hand, x, y+1, board, word+letter, move, validMoves,trie, connected, node);
	}
	else {
		if(s->isStart())
			connected = true;
		for(set<Tile*>::iterator it = hand.begin(); it != hand.end(); it++){
			letter = (*it)->getLetter();
			set<Tile*> temp = hand;
			temp.erase(*it);
			if(letter == '?'){
				for(int i = 0; i < 26; i++){
					TrieNode* child = node->children[i];
					if(x-1 > 0){
						if(board.getSquare(x-1, y)->isOccupied()&& !connected)
							connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
					}
					else if (x + 1 <= board.getColumns()){
						if(board.getSquare(x+1, y)->isOccupied()&& !connected)
							connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
					}
					getValidVerticalMoves(temp, x, y+1, board, word+(char)('a'+i), move + letter+(char)('a'+i), validMoves, trie, connected, child);
				}
			}
			else {
				TrieNode* child = node->children[letter-'a'];
				if(x-1 > 0){
					if(board.getSquare(x-1, y)->isOccupied()&& !connected)
						connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
				}
				else if (x + 1 <= board.getColumns()){
					if(board.getSquare(x+1, y)->isOccupied()&& !connected)
						connected = hasValidAdjacentWords(x, y, board, false, letter, trie);
				}
				getValidVerticalMoves(temp, x, y+1, board, word+letter, move + letter, validMoves, trie, connected, child);
			}
		}
	}
}

bool hasValidAdjacentWords(size_t x, size_t y, Board& board, bool horizontal, char letter, TrieSet& trie){
	string word = "";
	TrieNode* node = trie.prefix(word);
	int i = 0;
	if(horizontal){
		while(y - i- 1 > 0){
			if(!board.getSquare(x, y-1 -i)->isOccupied())
				break;
			i++;
		}
		y = y-i;
		size_t letterUsed = 1;
		while(y<= board.getRows() && (letterUsed != 0 || board.getSquare(x, y)->isOccupied()) && node){
			Square* s = board.getSquare(x, y);
			if(s->isOccupied())
				word+= s->getLetter();
			else{
				word+= letter;
				letterUsed--;
			}
			node = node->children[word[word.length()-1]-'a'];
			if(y >= board.getRows())
				break;
			y++;
		}
		if(node){
			if(node->inSet && word.length() > 1)
				return true;
		}
	}
	else{
		while(x - i -1 > 0){
			if(!board.getSquare(x-i-1, y)->isOccupied())
				break;
			x--;
		}
		x = x - i;
		size_t letterUsed = 1;
		while(y<= board.getRows() && (letterUsed != 0 || board.getSquare(x, y)->isOccupied()) && node){
			Square* s = board.getSquare(x, y);
			if(s->isOccupied())
				word+= s->getLetter();
			else{
				word+= letter;
				letterUsed--;
			}
			node = node->children[word[word.length()-1]-'a'];
			if(x >= board.getColumns())
				break;
			x++;
		}
		if(node){
			if(node->inSet&& word.length() > 1)
				return true;
		}
	}
	return false;
}

void getStartOfWord(size_t &x, size_t &y, Board& board, bool horizontal){
	if(horizontal){
		int i = 0;
		while(x - i -1 > 0){
			if(board.getSquare(x - i -1, y)->isOccupied()){
				i++;
			}
			else 
				break;
		}
		x = x - i ;
	}
	else{
		int i = 0;
		while(y - i -1 > 0){
			if(board.getSquare(x, y- i -1)->isOccupied()){
				i++;
			}
			else 
				break;
		}
		y = y - i;
	}
}

string returnMaxPointMove(set<string>& moves, Player& player, Board& board, bool firstmove, TrieSet& trie){
	size_t maxPoints = 0;
	Move* temp = nullptr;
	string finalMove = "";
	for(set<string>::iterator it = moves.begin(); it != moves.end(); it++){
		stringstream ss(*it);
		string buff;
		for(int i = 0; i < 6; i++)
			ss >> buff;
		size_t tilesPlaced = buff.length() - count(buff.begin(), buff.end(), '?');
		temp = Move::parseMove(*it, player);
		size_t points = 0;
		bool validMove = board.validPlaceMove(*dynamic_cast<PlaceMove*>(temp), firstmove);
		if(!validMove){
			returnCPUTiles(player, temp);
			continue;
		}
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
		if(tilesPlaced == player.getHandTiles().size())
			points+= 50;
		if(points > maxPoints){
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
	for(set<string>::iterator it = moves.begin(); it != moves.end(); it++){
		stringstream ss(*it);
		string buff;
		for(int i = 0; i < 6; i++)
			ss >> buff;
		size_t tilesPlaced = buff.length() - count(buff.begin(), buff.end(), '?');
		temp = Move::parseMove(*it, player);
		bool validMove = board.validPlaceMove(*dynamic_cast<PlaceMove*>(temp), firstmove);
		if(!validMove){
			returnCPUTiles(player, temp);
			continue;
		}
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
		if(tilesPlaced > maxlen && legalWord && validMove){
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
#include "scrabble.h"
#include <thread>
using namespace std;

void testHorizontalThread(set<pair<size_t, size_t>> horCoordinateBank, set<Tile*> hand,TrieSet& trie, Board& board, set<string>& moves);
void testVerticalThread(set<pair<size_t, size_t>> vertCoordinateBank, set<Tile*> hand, TrieSet& trie, Board& board, set<string>& moves);

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
		bool firstmove = initializeBoard(initFile, board);
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
			for(unsigned int i = 0; i < name.length(); i++)
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
		scrabble(players, board, bag, dictionary, console, firstmove, trie);
		for(vector<Player*>::iterator it = players.begin(); it != players.end(); it++){
			delete *it;
		}
	}
	catch (FileException &e){
		cout << e.getMessage() << endl;
	}	

	return 0;
	
}
void scrabble(vector<Player*> &players, Board &board, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie){
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
			cout << "Hello " + players[i]->getName() + " what would you like to do?" << endl;
			cout << "Examples: \nPASS\nEXCHANGE <tile string>\nPLACE <dir> <row> <col> <tile string>" << endl;
			Move* m = nullptr;
			//Bot moves
			if(players[i]->getType() != "human"){
				string handLetters = players[i]->getLetters();
				size_t size = players[i]->getHandTiles().size();
				//gets all possible horizontal and vertical start coordinates based on tiles on the board
				getHorizontalStartCoords(size, board, horCoordinateBank);
				getVerticalStartCoords(size, board, vertCoordinateBank);
				if(horCoordinateBank.size() == 0 && vertCoordinateBank.size() == 0){
					pair<size_t, size_t> sc = board.getStartCoords();
					horCoordinateBank.insert(sc);
					vertCoordinateBank.insert(sc);
				}
				//for every coordinate, the bot stores valid moves
				set<string> moves;
				//thread 1
				thread th1(testHorizontalThread, horCoordinateBank, players[i]->getHandTiles(), std::ref(trie), std::ref(board), std::ref(moves));
				//thread2
				thread th2(testVerticalThread, vertCoordinateBank, players[i]->getHandTiles(), std::ref(trie), std::ref(board), std::ref(moves));
				th1.join(); 
				th2.join();
				// retrieve final move based on bot type
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
			// adds occupied coords to a list for use in AI turns
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
bool initializeBoard(string &initFile, Board &board){
	//if no file provided, proceed as normal
	if(initFile.length() == 0)
		return false;
	ifstream ifile(initFile);
	size_t row = 1;
	string line;
	//adds tiles onto board if they exist, also allows player to place on any tile following placement of the tile(s)
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
			}
		}
		row++;
	}
	return true;
}
void findHorizontalMoves(set<Tile*> hand, size_t x, size_t y, TrieSet& trie, Board& board, set<string>& moves){
	string move = "place - " + to_string(y) + " " + to_string(x) + " ";
	//find start of word
	if(x-1 > 0){
		if(board.getSquare(x-1, y)->isOccupied())
			x = findStartLocation(x,y, board, true);
	}
	TrieNode* root = trie.prefix("");
	findHorizontalMovesHelper(hand, x, y, trie, board, root, move, moves);
}
void findVerticalMoves(set<Tile*> hand, size_t x, size_t y, TrieSet& trie, Board& board, set<string>& moves){
	string move = "place | " + to_string(y) + " " + to_string(x) + " ";
	//finds start coords of word to be built
	if(y-1 > 0){
		if(board.getSquare(x, y-1)->isOccupied())
			y = findStartLocation(x,y, board, false);
	}
	TrieNode* root = trie.prefix("");
	findVerticalMovesHelper(hand, x, y, trie, board, root, move, moves);
}

void testHorizontalThread(set<pair<size_t, size_t>> horCoordinateBank, set<Tile*> hand,TrieSet& trie, Board& board, set<string>& moves){
	for(set<pair<size_t, size_t>>::iterator it = horCoordinateBank.begin(); it != horCoordinateBank.end(); it++){
		size_t x = it->first; size_t y = it->second;
		findHorizontalMoves(hand, x, y, trie, board, moves);
	}
}

void testVerticalThread(set<pair<size_t, size_t>> vertCoordinateBank, set<Tile*> hand, TrieSet& trie, Board& board, set<string>& moves){
	for(set<pair<size_t, size_t>>::iterator it = vertCoordinateBank.begin(); it != vertCoordinateBank.end(); it++){
		size_t x = it->first; size_t y = it->second;
		findVerticalMoves(hand, x, y, trie, board, moves);
	}
}
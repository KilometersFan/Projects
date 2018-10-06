#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include <iostream>
#include <fstream>
using namespace std;

void scrabble(vector<Player*> &players, Board &board, Bag &bag, Dictionary &dictionary, ConsolePrinter &console);
void validMoveCheck(string &str);
int main(int argc, char const *argv[])
{
	try{
		// if(argc < 2){
		// 	throw FileException("No file specified.");
		// }
		ifstream ifile("config.txt");
		if(ifile.fail()){
			throw FileException("Unable to open file.");
		}
		size_t maxTiles = 0;
		string boardFile = "";
		string bagFile = "";
		string dictionaryFile = "";
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
		}	
		ifile.close();
		Dictionary dictionary(dictionaryFile);
		Board board(boardFile);
		Bag bag = Bag(bagFile, seed);
		ConsolePrinter console;
		cout << "Hello! Welcome to Scrabble. Please enter the number of players in the game (max 8): ";
		int numPlayers;
		cin >> numPlayers;
		cin.ignore(10000, '\n');
		cin.clear();
		vector<Player*> players;
		for (int i = 0; i < numPlayers; i++){
			string name;
			cout << "Please tell me your name: ";
			getline(cin, name);
			Player* player = new Player(name, maxTiles);
			players.push_back(player);
			vector <Tile*> initialHand = bag.drawTiles(maxTiles);
			player->addTiles(initialHand);
		}
		scrabble(players, board, bag, dictionary, console);
		for(vector<Player*>::iterator it = players.begin(); it != players.end(); it++){
			delete *it;
		}
	}
	catch (FileException &e){
		cout << e.getMessage() << endl;
	}	

	return 0;
	
}
void scrabble(vector<Player*> &players, Board &board, Bag &bag, Dictionary &dictionary, ConsolePrinter &console){
	bool gameOver = false;
	while (!gameOver){
		unsigned int passCount = 0;
		for (unsigned int i = 0; i < players.size(); i++){
			console.printBoard(board);
			console.printHand(*(players[i]));
			cout << "SCORES:\n";
			for(unsigned int j = 0; j < players.size(); j++){
				cout << (players[j])->getName() + ": " << (players[j]->getScore()) << endl;
			}
			cout << endl;
			string move;
			cout << "Hello " + players[i]->getName() + " what would you like to do?" << endl;
			cout << "Examples: \nPASS\nEXCHANGE <tile string>\nPLACE <dir> <row> <col> <tile string>" << endl;
			getline(cin, move);
			Move* m = Move::parseMove(move, *(players[i]));
			while(!m->isValidMove()){
				validMoveCheck(move);
				m = Move::parseMove(move, *(players[i]));
			}
			m->execute(board, bag, dictionary);
			while(!m->isValidMove()){
				validMoveCheck(move);
				delete m;
				m = Move::parseMove(move, *(players[i]));
				m->execute(board, bag, dictionary);
			}
			if(m->isPass())
				passCount++;
			cout << "Please press enter to end your turn." << endl;
			cin.ignore(10000, '\n');
			delete m;
		}
		if(passCount == players.size() || !bag.tilesRemaining())
			gameOver = true;
	}
	
	cout << "Thanks for playing!" << endl;
	for (unsigned int i = 0; i < players.size(); i++){
		set<Tile*> playerHand =  players[i]->getHandTiles();
		for(set<Tile*>::iterator it=playerHand.begin(); it != playerHand.end(); it++){
			int playerScore = static_cast<int>(players[i]->getScore());
			int tileScore = static_cast<int>((*it)->getPoints());
			if(playerScore - tileScore <= 0)
				tileScore = 0;
			else 
				tileScore *= -1;
			players[i]->setScore(tileScore);
		}
		cout << players[i]->getName() + "\'s final score: " << players[i]->getScore() << endl;
			
	}
	size_t max = players[0]->getScore();
	vector<Player*> winners;
	for (unsigned int i = 0; i < players.size(); i++){
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


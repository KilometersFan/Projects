#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include <iostream>
#include <fstream>
using namespace std;

void scrabble(vector<Player*> &players, Board &board, Bag &bag, Dictionary &dictionary, ConsolePrinter &console);

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
		vector<Player*> players;
		for (int i = 0; i < numPlayers; i++){
			string name;
			cout << "Please tell me your name: ";
			cin >> name;
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
		for (unsigned int i = 0; i < players.size(); i++){
			console.printBoard(board);
			console.printHand(*(players[i]));
			string move;
			cout << "Hello " + players[i]->getName() + " what would you like to do?: ";
			cin.ignore();
			getline(cin, move);
			Move* m = Move::parseMove(move, *(players[i]));
			m->execute(board, bag, dictionary);
			console.printBoard(board);
			console.printHand(*(players[i]));
		}
	}
}
#include "duck_duck_goose.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
using namespace std;

void simulateDDGRound(GameData * gameData, std::ostream & output) {
	size_t num = gameData->playerList.size();
	size_t gooseID = 0;
	while(num > 0){
		//cout << "num " << num << endl;
		size_t m = rand() % (4*num);
		//cout << "m " << m << endl;
		size_t i = 0;
		while (i < m){
			output << gameData->playerList.get(i) << " is Duck!" << endl;
			i++;
		}
		// cerr << endl;
		output << gameData->playerList.get(i) << " is Goose!" << endl << endl;
		gooseID = gameData->playerList.get(i);
		size_t itNum = (rand() % 49) + 1;
		size_t gooseNum = (rand() % 49) + 1;
		while(itNum == gooseNum) {
			itNum = (rand() % 49) + 1;
			gooseNum = (rand() % 49) + 1;
		}
		if(itNum > gooseNum){
			size_t temp = gameData->itPlayerID;
			gameData->itPlayerID = gooseID;
			gameData->playerList.set(i, temp);
		}
		else {
			size_t r = rand() % (m-1);
			size_t newGooseID = gameData->playerList.get(r);
			gameData->playerList.remove(r);
			output << gameData->itPlayerID << " is Out!" << endl<< endl;
			gameData->itPlayerID = newGooseID;
			output << gameData->itPlayerID << " was chosen as the new it."<< endl << endl;
			num--;
		}
		// for (int j = 0; j < num; j++){
		// 	cerr << gameData->playerList.get(j) << endl;
		// }
	}
	output << "Winner is: " << gooseID << endl;
}

int main(int argc, char *argv[])	{
	if(argc < 3){
		cout << "Invalid number of inputs" << endl;
	}
	ifstream ifile(argv[1]);
	if(ifile.fail()){
		cout << "Couldn't open input file" << endl;
		return 1;
	}
	ofstream ofile(argv[2]);
	if(ofile.fail()){
		cout << "Couldn't open output file" << endl;
		return 1;
	}
	int seed = 0;
	ifile >> seed;
	srand(seed);
	cerr << "seed: " << seed << endl;
	unsigned int num_players = 0;
	ifile >> num_players;
	cerr << "num players: " << num_players << endl;
	unsigned int itID = 0; 
	ifile >> itID;
	cerr << "itID: " << itID << endl;
	unsigned int buff = 0;
	CircularListInt* list = new CircularListInt();
	while (ifile >> buff){
		list->push_back(buff);
		cerr << "player ids: " << buff << endl;
	}
	cerr << endl;
	GameData* game = new GameData();
	game->playerList = *list;
	game->itPlayerID = itID;
	simulateDDGRound(game, ofile);
	ifile.close();
	ofile.close();
	delete game;
	delete list;
	return 0;
}
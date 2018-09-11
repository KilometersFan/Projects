#include "duck_duck_goose.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
using namespace std;

void simulateDDGRound(GameData* gameData, std::ostream & output) {
	size_t num = gameData->playerList.size();
	size_t gooseID = 0;
	while(num > 0){
		size_t m = rand() % (4*num);
		size_t i = 0;
		while (i < m){
			// cerr << gameData->playerList.get(i) << " is a Duck!"<< endl;
			output << gameData->playerList.get(i) << " is a Duck!"<< endl;
			i++;
		}
		output << gameData->playerList.get(i) << " is a Goose!"<< endl;
		// cerr << gameData->playerList.get(i) << " is a Goose!"<< endl;
		gooseID = gameData->playerList.get(i);
		size_t itNum = (rand() % 49) + 1;
		size_t gooseNum = (rand() % 49) + 1;
		while(itNum == gooseNum) {
			itNum = (rand() % 49) + 1;
			gooseNum = (rand() % 49) + 1;
		}
		if(itNum > gooseNum){
			output << gameData->itPlayerID << " took " << gameData->playerList.get(i) << "'s spot!" << endl;
			// cerr << gameData->itPlayerID << " took " << gameData->playerList.get(i) << "'s spot!" << endl;
			size_t temp = gameData->itPlayerID;
			gameData->itPlayerID = gooseID;
			gameData->playerList.set(i, temp);
		}
		else {
			size_t r = rand() % (m-1);
			size_t newGooseID = gameData->playerList.get(r);
			output << gameData->itPlayerID << " is out!"<< endl;
			// cerr << gameData->itPlayerID << " is out!"<< endl;
			gameData->itPlayerID = newGooseID;
			output << gameData->itPlayerID << " was chosen as the new it."<< endl; 
			// cerr << gameData->itPlayerID << " was chosen as the new it."<< endl;
			if(num == 1){
				break;
			}
			else{
				gameData->playerList.remove(r);
			}
			num--;
		}
		cerr << endl;
	}
	output << "Winner is " << gameData->itPlayerID << "!"<< endl;
	// cerr << "Winner is " << gameData->itPlayerID << "!" << endl;
}

int main(int argc, char *argv[])	{
	if(argc < 3){
		cout << "Invalid number of inputs" << endl;
		return 1;
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
	unsigned int num_players = 0;
	ifile >> num_players;
	unsigned int itID = 0; 
	ifile >> itID;
	unsigned int buff = 0;
	GameData game;
	game.itPlayerID = itID;
	while (ifile >> buff){
		game.playerList.push_back(buff);
	}
	cerr << endl;
	simulateDDGRound(&game, ofile);
	ifile.close();
	ofile.close();
	return 0;
}
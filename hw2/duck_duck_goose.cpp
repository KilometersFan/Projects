#include "duck_duck_goose.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
using namespace std;

void simulateDDGRound(GameData* gameData, std::ostream & output) {
	//get original size of list
	size_t num_list = gameData->playerList.size();
	//create gooseID
	size_t gooseID = 0;
	//play until no players in list remain
	while(num_list > 0){
		//get random number for duck calls
		size_t m = rand() % (4*num_list);
		//loop until a goose is found
		size_t i = 0;
		while (i < m){
			//write to output
			output << gameData->playerList.get(i) << " is a Duck!"<< endl;
			i++;
		}
		//write to output
		output << gameData->playerList.get(i) << " is a Goose!"<< endl;
		//set gooseID
		gooseID = gameData->playerList.get(i);
		//calculate numbers for goose and itplayer
		size_t itNum = (rand() % 49) + 1;
		size_t gooseNum = (rand() % 49) + 1;
		//if the numbers are equal, make new ones until they are not
		while(itNum == gooseNum) {
			itNum = (rand() % 49) + 1;
			gooseNum = (rand() % 49) + 1;
		}
		//if it player wins swap it and goose IDs
		if(itNum > gooseNum){
			//write to output
			output << gameData->itPlayerID << " took " << 
			gameData->playerList.get(i) << "'s spot!" << endl;
			//swap IDs
			size_t temp = gameData->itPlayerID;
			gameData->itPlayerID = gooseID;
			gameData->playerList.set(i, temp);
		}
		//if goose wins, remove it player from game and choose a new one
		else {
			//if there is only one player apart from it, set itplayer to 0
			// and wite that the goose has won
			if(num_list == 1){
				output << gameData->itPlayerID << " is out!"<< endl;
				gameData->itPlayerID = 0;
			}
			else{
				//randomly choose new it player from list
				size_t r = rand() % (m-1);
				size_t newGooseID = gameData->playerList.get(r);
				//write to output
				output << gameData->itPlayerID << " is out!"<< endl;
				//set new itplayer and remove them from playerlist
				gameData->itPlayerID = newGooseID;
				gameData->playerList.remove(r);
				output << gameData->itPlayerID << " was chosen as the new it."<< endl; 
			}
			//decrement playerlist size
			num_list--;
		}
	}
	//write to output
	output << "Winner is " << gooseID << "!"<< endl;
}

int main(int argc, char *argv[])	{
	//invalid command line arguments
	if(argc < 3){
		cout << "Invalid number of inputs" << endl;
		return 1;
	}
	//invalid input/output files
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
	//get info from config file
	int seed = 0;
	ifile >> seed;
	srand(seed);
	unsigned int num_players = 0;
	ifile >> num_players;
	unsigned int itID = 0; 
	ifile >> itID;
	unsigned int buff = 0;
	//create game
	GameData game;
	//set game settings from config file
	game.itPlayerID = itID;
	while (ifile >> buff){
		game.playerList.push_back(buff);
	}
	//start game
	simulateDDGRound(&game, ofile);
	//close files
	ifile.close();
	ofile.close();
	return 0;
}
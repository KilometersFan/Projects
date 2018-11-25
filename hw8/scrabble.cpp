#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include "Trie.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include <set>
#include <bits/stdc++.h> 
using namespace std;

void scrabble(vector<Player*> &players, Board &board, set<pair<size_t, size_t>>& occupiedCoords, Bag &bag, Dictionary &dictionary, ConsolePrinter &console, bool firstmove, TrieSet& trie);
void validMoveCheck(string &str);
bool initializeBoard(string &initFile, Board& board, set<pair<size_t, size_t>> &occupiedCoords);
void permute(string str, string out, set<string>& combos);
void findSubsets(string str, string out, size_t count, set<string>& sets);
set<pair<size_t, size_t>> getVerticalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords);
set<pair<size_t, size_t>> getHorizontalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords);
void getValidHorizontalMoves(string str, set<pair<size_t, size_t>> &startCoords, Board& board, set<string> &moves, TrieSet& trie);

void getValidVerticalMoves(string str, set<pair<size_t, size_t>> &startCoords, Board& board, set<string> &moves, TrieSet& trie);
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
			if(players[i]->getType() != "human"){
				string handLetters = players[i]->getLetters();
				set<string> permutations;
				permute(handLetters, "", permutations);
				cerr << "created permutations" << endl;
				set<string> subsets;
				for(set<string>::iterator it = permutations.begin(); it != permutations.end(); ++it)
					findSubsets(*it, "", 0, subsets);
				cerr << "created subsets" << endl;
				//subsets now has all permutations of every subset of the cpu's hand
				//retirieve all possible start coords for every sub set length
				map<int, set<pair<size_t, size_t>>> horCoordinateBank;
				map<int, set<pair<size_t, size_t>>> vertCoordinateBank;
				size_t size = players[i]->getLetters().length();
				for(int i = 1; i <= size; i++){
					horCoordinateBank[i] = getHorizontalStartCoords(i, board, occupiedCoords);
					vertCoordinateBank[i] = getVerticalStartCoords(i, board, occupiedCoords);
				}
				cerr << "created start coordinate banks" << endl;
				set<string> moves;
				for(set<string>::iterator it = subsets.begin(); it != subsets.end(); it++){
					getValidHorizontalMoves(*it, horCoordinateBank[it->length()], board, moves, trie);
					getValidVerticalMoves(*it, vertCoordinateBank[it->length()], board, moves, trie);
				}
				// for(set<string>::iterator it = moves.begin(); it != moves.end(); it++)
				// 	cout << *it << endl;
				console.printBoard(board);
			}
			// set<pair<size_t, size_t>> temp = getHorizontalStartCoords(1, board, occupiedCoords);
			// for (set<pair<size_t, size_t>>::iterator it = temp.begin(); it != temp.end(); it++)
			// 	cout << it->first << " " << it->second << endl;
			string move;
			cout << "Hello " + players[i]->getName() + " what would you like to do?" << endl;
			cout << "Examples: \nPASS\nEXCHANGE <tile string>\nPLACE <dir> <row> <col> <tile string>" << endl;
			getline(cin, move);
			//creates move based on player input. If malformed, asks again until well formed 
			Move* m = Move::parseMove(move, *(players[i]));
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
			console.printHand(*(players[i]));
			if(m->isPass())
				passCount++;
			//adds occupied coords to a list for use in AI turns
			if(m->isWord()){
				if((dynamic_cast<PlaceMove*>(m))->isHorizontal()){
					for(unsigned int i = 0; i < (dynamic_cast<PlaceMove*>(m))->getStringLength(); i++){
						occupiedCoords.insert(make_pair((dynamic_cast<PlaceMove*>(m))->getY(), (dynamic_cast<PlaceMove*>(m))->getX() + i));
					}
				}
				else {
					for(unsigned int i = 0; i < (dynamic_cast<PlaceMove*>(m))->getStringLength(); i++){
						occupiedCoords.insert(make_pair((dynamic_cast<PlaceMove*>(m))->getY() + i, (dynamic_cast<PlaceMove*>(m))->getX()));
					}
				}
			}
			for (set<pair<size_t,size_t>>::iterator it = occupiedCoords.begin(); it != occupiedCoords.end(); it++){
				cout << it->first << " " << it->second << endl;
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

void permute(string str, string out, set<string>& combos) 
{ 
    // When size of str becomes 0, out has a 
    // permutation (length of out is n) 
    if (str.size() == 0 && out != "") 
    { 
        combos.insert(out); 
        return; 
    } 
  
    // One be one move all characters at 
    // the beginning of out (or result) 
    for (unsigned int i = 0; i < str.size(); i++) 
    { 
        // Remove first character from str and 
        // add it to out 
        permute(str.substr(1), out + str[0], combos); 
  
        // Rotate string in a way second character 
        // moves to the beginning. 
        rotate(str.begin(), str.begin() + 1, str.end()); 
    } 
}

void findSubsets(string str, string out, size_t count, set<string>& sets){
	if(count == (str.length())){
		if(out != "")
			sets.insert(out);
		return;
	}
	findSubsets(str, out + "", count + 1, sets);
	findSubsets(str, out + str[count], count + 1, sets);
}

set<pair<size_t, size_t>> getHorizontalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords) {
	set<pair<size_t, size_t>> startCoords;
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
		if(x + j + 1 <= board.getColumns()){
			if(!board.getSquare(x+j+1, y)->isOccupied())
				startCoords.insert(make_pair(x + j + 1, y));
		}
	}
	return startCoords;
}
set<pair<size_t, size_t>> getVerticalStartCoords(size_t length, Board &board, set<pair<size_t, size_t>>& occupiedCoords) {
	set<pair<size_t, size_t>> startCoords;
	for (set<pair<size_t, size_t>>::iterator it = occupiedCoords.begin(); it != occupiedCoords.end(); it++){
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
		if(y + j + 1 <= board.getRows()){
			if(!board.getSquare(x, y+j+1)->isOccupied())
				startCoords.insert(make_pair(x, y + j + 1));
		}
	}
	return startCoords;
}

void getValidHorizontalMoves(string str, set<pair<size_t, size_t>> &startCoords, Board& board, set<string> &moves, TrieSet& trie){
	for(set<pair<size_t, size_t>>::iterator it = startCoords.begin(); it != startCoords.end(); it++){
		size_t x = it->first;
		size_t y = it->second;
		size_t i = 0;
		bool inTrie = true;
		while(x - i - 1 > 0){
			if(!board.getSquare(x - i - 1, y)->isOccupied())
				break;
			i++;
		}
		size_t newX = x - i;
		size_t count = 0;
		string word = "";
		while(count != str.length() || board.getSquare(newX, y)->isOccupied()){
			Square* s = board.getSquare(newX, y);
			if(s->isOccupied())
				word += s->getLetter();
			else{
				word += str[count];
				count++;
			}
			if(!trie.prefix(word)){
				inTrie = false;
				break;
			}
			newX++;
			if(newX > board.getColumns())
				break;
		}
		// cout << word << " " << trie.inTrie(word) << endl;
		if(inTrie && trie.inTrie(word)){
			// cout << word << endl;
			string move = "place - " + to_string(y) + " " + to_string(x) + " " + str;
			// cout << move << endl;
			moves.insert(move);
		}
	}
}

void getValidVerticalMoves(string str, set<pair<size_t, size_t>> &startCoords, Board& board, set<string> &moves, TrieSet& trie){
	for(set<pair<size_t, size_t>>::iterator it = startCoords.begin(); it != startCoords.end(); it++){
		size_t x = it->first;
		size_t y = it->second;
		size_t i = 0;
		bool inTrie = true;
		while(y - i - 1 > 0){
			if(!board.getSquare(x, y - i - 1)->isOccupied())
				break;
			i++;
		}
		size_t newY = y - i;
		size_t count = 0;
		string word = "";
		while(count != str.length() || board.getSquare(x, newY)->isOccupied()){
			Square* s = board.getSquare(x, newY);
			if(s->isOccupied())
				word += s->getLetter();
			else{
				word += str[count];
				count++;
			}
			if(!trie.prefix(word)){
				inTrie = false;
				break;
			}
			newY++;
			if(newY > board.getRows())
				break;
		}
		// cout << word << " " << trie.inTrie(word) << endl;
		if(inTrie && trie.inTrie(word)){
			cout << word << endl;
			string move = "place | " + to_string(y) + " " + to_string(x) + " " + str;
			cout << move << endl;
			moves.insert(move);
		}
	}
}
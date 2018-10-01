#include "Player.h"
#include "Move.h"
#include "Board.h"
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	Dictionary dic("dictionary.txt");
	Board board("board.txt");
	for (size_t i = 1; i <= board.getRows(); i++){
		for(size_t j = 1; j <= board.getColumns(); j++){
			cout << board.getSquare(i, j)->getLMult();
		}
		cout << endl;
	}
	cout << endl;
	for (size_t i = 1; i <= board.getRows(); i++){
		for(size_t j = 1; j <= board.getColumns(); j++){
			cout << board.getSquare(i, j)->getWMult();
		}
		cout << endl;
	}
	return 0;
} 
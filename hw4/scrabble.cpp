#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	Dictionary dic("dictionary.txt");
	Board board("board.txt");
	Player player1 = Player("Miles", 8);
	Bag bag = Bag("bag.txt", 99977);
	ConsolePrinter console;
	console.printBoard(board);

	vector<Tile*> player1Hand = bag.drawTiles(8);
	player1.addTiles(player1Hand);
	console.printHand(player1);
	Move* m = Move::parseMove("EXCHANGE eeikz", player1);
	m->execute(board, bag, dic);
	console.printHand(player1);
	player1Hand = bag.drawTiles(5);
	player1.addTiles(player1Hand);
	console.printHand(player1);
	delete m;
	return 0;
}
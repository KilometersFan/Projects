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
	Bag bag = Bag("bag.txt", 99978);
	ConsolePrinter console;
	console.printBoard(board);
	vector<Tile*> player1Hand = bag.drawTiles(8);
	player1.addTiles(player1Hand);
	console.printHand(player1);

	Move* n = Move::parseMove("EXCHANGE aaeeei", player1);
	console.printHand(player1);
	n->execute(board, bag, dic);
	console.printHand(player1);
	Move* m = Move::parseMove("PLACE | 8 8 VINE", player1);
	console.printHand(player1);
	m->execute(board, bag, dic);
	console.printBoard(board);
	console.printHand(player1);
	Move* t = Move::parseMove("PLACE - 11 6 RY", player1);
	console.printHand(player1);
	t->execute(board, bag, dic);
	console.printBoard(board);
	console.printHand(player1);
	delete m;
	delete n;
	delete t;
	return 0;
}
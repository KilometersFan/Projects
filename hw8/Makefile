CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: scrabble test

scrabble: scrabble.cpp Player.o Move.o Dictionary.o Bag.o Board.o ConsolePrinter.o Square.h Tile.h 
	$(CXX) $(CPPFLAGS) $^ -o $@

Player.o: Player.cpp Player.h Tile.h Exceptions.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Move.o: Move.cpp Move.h Tile.h Bag.h Dictionary.h Player.h Board.h Exceptions.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Dictionary.o: Dictionary.cpp Dictionary.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Bag.o: Bag.cpp Bag.h Tile.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Board.o: Board.cpp Board.h Tile.h Square.h Move.h Dictionary.h Exceptions.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

ConsolePrinter.o: ConsolePrinter.cpp ConsolePrinter.h rang.h Board.h Player.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Trie.o: Trie.cpp Trie.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

test: test.cpp Trie.o
	$(CXX) $(CPPFLAGS) $^ -o $@
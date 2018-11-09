CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: test

test: test.cpp bst.h rotateBST.h
	$(CXX) $(CPPFLAGS) $^ -o $@

clean:
	rm -f *o test
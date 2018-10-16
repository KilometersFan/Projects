CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: cave functor test

test: test.cpp msort.h functor.h

cave: cave.cpp 
	$(CXX) $(CPPFLAGS) $^ -o $@

functor: functor.cpp functor.h
	$(CXX) $(CPPFLAGS) $^ -o $@	

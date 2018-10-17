CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: cave functor test fatalist

test: test.cpp msort.h functor.h

cave: cave.cpp 
	$(CXX) $(CPPFLAGS) $^ -o $@

functor: functor.cpp functor.h
	$(CXX) $(CPPFLAGS) $^ -o $@

fatalist: fatalist.cpp msort.h functor.h
	$(CXX) $(CPPFLAGS) $^ -o $@	
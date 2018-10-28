CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: search

search: search.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

coloring: coloring.cpp coloring.h
	$(CXX) $(CPPFLAGS) $^ -o $@

test: test.cpp MinHeap.h
	$(CXX) $(CPPFLAGS) $^ -o $@
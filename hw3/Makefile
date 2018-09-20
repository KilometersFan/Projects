CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: parser

parser: parser.cpp stackint.o circular_list_int.o
	$(CXX) $(CPPFLAGS) $^ -o $@

stackint.o: stackint.cpp stackint.h circular_list_int.o
	$(CXX) $(CPPFLAGS) -c $< -o $@

circular_list_int.o: circular_list_int.cpp circular_list_int.h
	$(CXX) $(CPPFLAGS) -c $< -o $@


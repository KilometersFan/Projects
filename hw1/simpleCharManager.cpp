#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include "simpleCharManager.h"
using namespace std;
void simpleCharManager::printbuff(){
  cerr << "Buffer contents:" << endl;
  for (int i = 0; i < 20; i++){
    cerr << buffer[i] << endl;
  }
}
simpleCharManager::simpleCharManager(){
	for (int i = 0; i < BUF_SIZE; i++){
		buffer[i] = '\0';
	}
	free_place = &buffer[0];
}

simpleCharManager::~simpleCharManager(){
	for (int i = 0; i < BUF_SIZE; i++){
		buffer[i] = '\0';
	}
	free_place = &buffer[0];
}
             
char* simpleCharManager::alloc_chars(int n){
	char* prev_place = free_place;
	if(prev_place == &buffer[BUF_SIZE-1])
		return NULL;
	for(int i = 0; i < n; i++)
		*(free_place + i) = '\0';
	free_place += n; 
	return prev_place;
}

void simpleCharManager::free_chars(char* p){
	free_place = p;
	int i = 0;
	while(true){
		*(free_place + i) = '\0';
		if(free_place+i == &buffer[BUF_SIZE-1])
			break;
		i++;
	}
}         





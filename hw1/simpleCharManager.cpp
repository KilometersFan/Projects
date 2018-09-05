#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include "simpleCharManager.h"
using namespace std;
//prints the contents of buffer
void simpleCharManager::printbuff(){
  cerr << "Buffer contents:" << endl;
  for (int i = 0; i < 20; i++){
    cerr << buffer[i] << endl;
  }
}
//constructor that sets buffer elements to 
//null characters
simpleCharManager::simpleCharManager(){
	for (int i = 0; i < BUF_SIZE; i++){
		buffer[i] = '\0';
	}
	free_place = &buffer[0];
}
//destructor that reverts buffer to original 
//state
simpleCharManager::~simpleCharManager(){
	for (int i = 0; i < BUF_SIZE; i++){
		buffer[i] = '\0';
	}
	free_place = &buffer[0];
}
//allocates elements in buffer to act like
// "new" statement             
char* simpleCharManager::alloc_chars(int n){
	char* prev_place = free_place;
	//returns NULL if no free space
	if(prev_place == &buffer[BUF_SIZE-1])
		return NULL;
	for(int i = 0; i < n; i++)
		*(free_place + i) = '\0';
	free_place += n; 
	return prev_place;
}
//Frees memory at a particular location until 
//the end of buffer
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





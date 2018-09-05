#include <iostream>
#include <stdlib.h>
#include "flexCharManager.h"
using namespace std;
//constructor that sets used memory to array of Mem_block pointers 
//and sets default size of used_memory
flexCharManager::flexCharManager(){
 	used_memory = new Mem_Block*[2];
 	used_memory[0] = NULL;
 	used_memory[1] = NULL;
 	free_mem = BUF_SIZE;
 	active_requests = 0;
 	used_mem_size = 2;
}
//delete all allocated memory for memblocks and 
//delete used_memory
flexCharManager::~flexCharManager(){
	for (int i = 0; i < active_requests; i++)
		delete used_memory[i];
	delete [] used_memory;
}
char* flexCharManager::flex_alloc_chars(int n){
	//if buffer is full, return NUll
	if(free_mem <= 0)
		return NULL;
	int len = 0;
	int diff = 0;
	char* mem_pos = NULL;
	//sort used_memory before accessing it
	sort();
	//increase used_mem suize if necessay
	if(active_requests == used_mem_size){
		Mem_Block** temp = used_memory;
		if(!used_mem_size)
			used_mem_size = 1;
		used_memory = new Mem_Block*[used_mem_size*2];
		for (int i = 0; i < used_mem_size; i++){
			used_memory[i] = temp[i];
		}
		int old_mem_size = used_mem_size;
		used_mem_size *= 2;
		for (int j = old_mem_size; j < used_mem_size; j++){
			used_memory[j] = NULL;
		}
		delete [] temp;
	}
	//upon initialization of flexcharmanager set 
	//mem_pos to buffer[0] address
	if (active_requests == 0){
		mem_pos = buffer;
	}
	else {
		//loop through used_memory for space
		for (int i = 0; i < active_requests; i++){
			//check to see if there's space before first mem block
			if(i == 0){
				diff = (int)(used_memory[i]->physical_location - buffer);
				if(diff >= n){
					mem_pos = buffer;
					break;
				}
				//check to see if there is space after first memblock
				if(active_requests == 1){
					len = used_memory[i]->size;
					diff = (int)(&buffer[9999] - 
					(used_memory[i]->physical_location + len));
					if(diff >= n){
						mem_pos = used_memory[i]->physical_location + len;
						break;
					}
				}

			}
			//compare mem blocks to see if there is space between them
			else if (i < active_requests-1){
				len = used_memory[i-1]->size;
				diff = (int)((used_memory[i]->physical_location)-
				(used_memory[i-1]->physical_location+len));
				if(diff >= n){
					mem_pos = used_memory[i-1]->physical_location 
					+ len;
					break;
				}
			}
			//see if there's space after last mem_block
			else if (i == active_requests-1){
				len = used_memory[i]->size;
				diff = (int)(&buffer[9999]-(used_memory[i]->physical_location + len));
				if(diff >= n){
					mem_pos = used_memory[i]->physical_location + len;
					break;
				}
			}
		}
	}
	if(!mem_pos)
		return NULL;
	//create mem block if mem pos exists
	Mem_Block* new_block = new Mem_Block(n, mem_pos);
	used_memory[active_requests] = new_block;
	free_mem -= n;
	active_requests++;
	//Sort used_memory array;
	sort();
	return mem_pos;
}
//frees memory in buffer and removes memblock in used_memory
void flexCharManager::flex_free_chars(char* p){
	int len = 0;
	//sort array before accessing it
	sort();
	//decrease used memory size if needed
	if(active_requests <= used_mem_size/2){
		Mem_Block** temp = used_memory;
		used_mem_size /= 2;
		if(!used_mem_size)
			used_mem_size = 1;
		used_memory = new Mem_Block*[used_mem_size];
		for (int i = 0; i < used_mem_size; i++){
			used_memory[i] = temp[i];
		}
		delete [] temp;
	}
	//loop through mem blocks to find matching case with p
	for (int i = 0; i < active_requests; i++){
		if(p == used_memory[i]->physical_location){
			len = used_memory[i]->size;
			//set spaces in buffer to null character
			for (int j = 0; j < len; j++){
				*(used_memory[i]->physical_location + j) = '\0';
			}
			free_mem += used_memory[i]->size;
			Mem_Block* temp_mem = used_memory[i];
			active_requests--;
			//remove and shift elements in used memory to the left
			for (int j = i; j < active_requests; ++j){
				used_memory[j] = used_memory[j+1];
			}
			used_memory[active_requests] = NULL;
			delete used_memory[active_requests];
			delete temp_mem;
			sort();
			break;
		}
	}
}     
//sorts used_memory via insertion sort    
void flexCharManager::sort() {
	for (int j = 0; j < active_requests; j++){
		for (int k = j + 1; k < active_requests; k++){
			Mem_Block* temp = used_memory[j];
			if(used_memory[j]->physical_location - 
				used_memory[k]->physical_location > 0){
				used_memory[j] = used_memory[k];
				used_memory[k] = temp;
			}
		}
	}
}
//prints the contents of buffer
void flexCharManager::print_buff(){
  cerr << "Buffer contents:" << endl;
  for (int i = 0; i < 25; i++){
    cerr << buffer[i] << endl;
  }
}
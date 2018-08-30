#ifndef FLEXCHARMANAGER_H
#define FLEXCHARMANAGER_H


#include <cstddef>
#include "simpleCharManager.cpp"
#include <iostream>
#include <stdlib.h>
typedef struct Mem_Block {
      int size;
      char* physical_location;
      Mem_Block(){size = 0; physical_location = NULL;};
      Mem_Block(int s,char *p){size = s; physical_location = p;};
} Mem_Block;


class flexCharManager: public simpleCharManager
{
	public:
            flexCharManager();
            ~flexCharManager();
            
            char* alloc_chars(int n);
            void free_chars(char* p);
            void sort();
            void print_buff();
            void print_used_mem();

	protected:
/*Dynamically maintain an array of pointers to Mem_Blocks sorted by physical address which they manage in order to keep
track of active requests */
            Mem_Block** used_memory;
/* memory available in the buffer */
            int free_mem;
/* memory blocks in use */
            int active_requests;
/* memory blocks available in array of memory blocks*/
            int used_mem_size;

};





#endif

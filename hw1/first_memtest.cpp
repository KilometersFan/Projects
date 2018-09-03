#include <iostream>
#include <stdlib.h>
#include "simpleCharManager.cpp"
using namespace std;
void simpleCharManager::printbuff(){
  cerr << "Buffer contents:" << endl;
  for (int i = 0; i < 20; i++){
    cerr << buffer[i] << endl;
  }
}
int main(int argc, char *argv[])
{
  simpleCharManager simplest_mem_manager;
  
  /*write driver code as described in the assignment to replace this */
  char* c1 = simplest_mem_manager.alloc_chars(1);
  char* c2 = simplest_mem_manager.alloc_chars(1);
  char* c3 = simplest_mem_manager.alloc_chars(1);
  char* c4 = simplest_mem_manager.alloc_chars(1);
  char* c5 = simplest_mem_manager.alloc_chars(1);
  char* c6 = simplest_mem_manager.alloc_chars(1);
  char* c7 = simplest_mem_manager.alloc_chars(1);
  char* c8 = simplest_mem_manager.alloc_chars(1);
  char* c9 = simplest_mem_manager.alloc_chars(1);
  char* c10 = simplest_mem_manager.alloc_chars(1);
  char* c11 = simplest_mem_manager.alloc_chars(1);
  char* c12 = simplest_mem_manager.alloc_chars(1);
  char* c13 = simplest_mem_manager.alloc_chars(1);
  c1[0] = 'H';
  c2[0] = 'e';
  c3[0] = 'l';
  c4[0] = 'l';
  c5[0] = 'o';
  c6[0] = ' ';
  c7[0] = 'w';
  c8[0] = 'o';
  c9[0] = 'r';
  c10[0] = 'l';
  c11[0] = 'd';
  c12[0] = '!';
  c13[0] = '\n';
  simplest_mem_manager.printbuff();
  simplest_mem_manager.free_chars(c7);
  simplest_mem_manager.printbuff();
  char *c14 = simplest_mem_manager.alloc_chars(11);
  c14[0] = 'm';
  c14[1] = 'o';
  c14[2] = 'o';
  c14[3] = 'n';
  c14[4] = '!';
  c14[5] = ' ';
  c14[6] = 'B';
  c14[7] = 'y';
  c14[8] = 'e';
  c14[9] = '.';
  c14[10] = '\n';
  simplest_mem_manager.printbuff();
  return 0;
}


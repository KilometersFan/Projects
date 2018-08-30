#include <iostream>
#include <stdlib.h>
#include "flexCharManager.cpp"
using namespace std;

int main(int argc, char *argv[])
{
  flexCharManager simplest_mem_manager;
  char* c1 = simplest_mem_manager.alloc_chars(3);
  c1[0] = 'i';
  c1[1] = 'n';
  c1[2] = ' ';
  simplest_mem_manager.print_buff();
  char* c2 = simplest_mem_manager.alloc_chars(7);
  c2[0] = 'F';
  c2[1] = 'r';
  c2[2] = 'e';
  c2[3] = 'n';
  c2[4] = 'c';
  c2[5] = 'h';
  c2[6] = ' ';
  simplest_mem_manager.print_buff();
  char* c3 = simplest_mem_manager.alloc_chars(7);
  c3[0] = 'c';
  c3[1] = 'h';
  c3[2] = 'a';
  c3[3] = 'p';
  c3[4] = 'e';
  c3[5] = 'a';
  c3[6] = 'u';
  simplest_mem_manager.print_buff();
  simplest_mem_manager.free_chars(c1);
  simplest_mem_manager.print_buff();
  char* c4 = simplest_mem_manager.alloc_chars(3);
  c4[0] = 't';
  c4[1] = 'o';
  c4[2] = 'p';
  simplest_mem_manager.print_buff();
  simplest_mem_manager.free_chars(c2);
  simplest_mem_manager.print_buff();
  char* c5 = simplest_mem_manager.alloc_chars(8);
  c5[0] = 's';
  c5[1] = 'o';
  c5[2] = 'm';
  c5[3] = 'b';
  c5[4] = 'r';
  c5[5] = 'e';
  c5[6] = 'r';
  c5[7] = 'o';
  simplest_mem_manager.print_buff();
  char* c6 = simplest_mem_manager.alloc_chars(3);
  c6[0] = 'h';
  c6[1] = 'a';
  c6[2] = 't';
  simplest_mem_manager.print_buff();
  
  return 0;
}

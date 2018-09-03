#include <iostream>
#include <stdlib.h>
#include "flexCharManager.h"
using namespace std;
void flexCharManager::print_buff(){
  cerr << "Buffer contents:" << endl;
  for (int i = 0; i < 25; i++){
    cerr << buffer[i] << endl;
  }
}
int main(int argc, char *argv[])
{
  flexCharManager flex_mem_manager;
  char* c1 = flex_mem_manager.flex_alloc_chars(3);
  c1[0] = 'i';
  c1[1] = 'n';
  c1[2] = ' ';
  flex_mem_manager.print_buff();
  char* c2 = flex_mem_manager.flex_alloc_chars(7);
  c2[0] = 'F';
  c2[1] = 'r';
  c2[2] = 'e';
  c2[3] = 'n';
  c2[4] = 'c';
  c2[5] = 'h';
  c2[6] = ' ';
  flex_mem_manager.print_buff();
  char* c3 = flex_mem_manager.flex_alloc_chars(7);
  c3[0] = 'c';
  c3[1] = 'h';
  c3[2] = 'a';
  c3[3] = 'p';
  c3[4] = 'e';
  c3[5] = 'a';
  c3[6] = 'u';
  flex_mem_manager.print_buff();
  flex_mem_manager.flex_free_chars(c1);
  flex_mem_manager.print_buff();
  char* c4 = flex_mem_manager.flex_alloc_chars(3);
  c4[0] = 't';
  c4[1] = 'o';
  c4[2] = 'p';
  flex_mem_manager.print_buff();
  flex_mem_manager.flex_free_chars(c2);
  flex_mem_manager.print_buff();
  char* c5 = flex_mem_manager.flex_alloc_chars(8);
  c5[0] = 's';
  c5[1] = 'o';
  c5[2] = 'm';
  c5[3] = 'b';
  c5[4] = 'r';
  c5[5] = 'e';
  c5[6] = 'r';
  c5[7] = 'o';
  flex_mem_manager.print_buff();
  char* c6 = flex_mem_manager.flex_alloc_chars(3);
  c6[0] = 'h';
  c6[1] = 'a';
  c6[2] = 't';  
  flex_mem_manager.print_buff();
  return 0;
}

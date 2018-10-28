#include <vector>
#include <utility>
#include <exception>
#include <stdexcept>
#include <iostream>

class invalidAccessException : public std::exception{
  public:
    invalidAccessException (std::string message) { _message = message; }
    ~invalidAccessException () throw () {}
    std::string getMessage () const { return _message; }
  private:
    std::string _message;
};

template <class T>
  class MinHeap {
     public:
       MinHeap (int d){
          _dary = d;
       };

       ~MinHeap (){

       };

       void add (T item, int priority){
         /* adds the item to the heap, with the given priority. */
          std::pair<T, int> temp = std::make_pair(item,priority);
          heap.push_back(temp);
          // std::cout << heap.size() << std::endl;
          bubbleUp(heap.size() - 1);
       };

       const T & peek () const{
          if(isEmpty()){
            throw invalidAccessException("Heap is empty.");
          }
          return heap[0].first;
       };

       void remove (){
         /* removes the element with smallest priority.
            Break ties however you wish.
            Throws an exception if the heap is empty. */
          if(isEmpty()){
            throw invalidAccessException("Heap is empty.");
          }
          std::swap(heap[0], heap[heap.size()-1]);
          heap.pop_back();
          trickleDown(0);
       };

       const bool isEmpty ()const{
          if(heap.size() == 0)
            return true;
          return false;
       };

       void trickleDown(int pos){
        unsigned int child = _dary*pos;
        if(child < heap.size()){
          int counter = 0;
          for(int i = 1; i <= _dary; i++){
            if(child + i < heap.size()){
              if(heap[child+i].second < heap[child + counter].second)
                counter = i;
            }
            else 
              break;
          }
          child += counter;
          if(heap[child].second < heap[pos].second){
            std::swap(heap[child], heap[pos]);
            trickleDown(child);
          }
        }
      };
      void bubbleUp(int pos){
        // std::cout << (pos-1)/_dary << std::endl;
        if(pos > 0 && heap[pos].second < heap[(pos-1)/_dary].second){
          std::swap(heap[pos], heap[(pos-1)/_dary]);
          bubbleUp((pos-1)/_dary);
        }
      };
         
   private:
      std::vector<std::pair<T, int>> heap;
      int _dary;
  };
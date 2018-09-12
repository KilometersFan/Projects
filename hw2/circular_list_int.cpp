#include "circular_list_int.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
// standard no-argument constructor

CircularListInt::CircularListInt(){

}
// Destructor. Should delete all data allocated by the list. 
CircularListInt::~CircularListInt(){
	while(!empty()){
		remove(0);
	}
}

// Gets item at an index.
// If an index is passed that is larger then the number of items in the list,
// it should "wrap around" back to the first element.
// If there are no elements in the list, returns 0.
int CircularListInt::get(size_t index) const {
	if(!count){
		return 0;
	}
	else {
		size_t true_count = (index % count);
		Item* temp = head;
		for (size_t i = 0; i < true_count; i++){
			temp = temp->next;
		}
		return temp->value;
	}

}

// get length of list.
size_t CircularListInt::size()const {
	return count;
}

// returns true iff the list is empty.
bool CircularListInt::empty()const {
	if(!count)
		return true;
	return false;
}

// Inserts (a copy of) the given item at the end of list.
void CircularListInt::push_back(int value){
	if (head == nullptr){
		head = new Item(value);
		head->next = head;
		head->prev = head;
	}
	else{
		Item* newItem = new Item(value);
		Item* temp = head->prev;
		newItem->prev = temp;
		newItem->next = head;
		temp->next = newItem;
		head->prev = newItem;
	}
	count++;
}

// Sets the item at the given index to have the given value.
// If an index is passed that is >= the number of items in the list, 
//it should "wrap around" back to the first element.
void CircularListInt::set(size_t index, int value){
	if(!count){
		return;
	}
	size_t true_count = (index % count);
	Item* temp = head;
	for (size_t i = 0; i < true_count; i++){
		temp = temp->next;
	}
	temp->value = value;
	return;
}

// Removes the item at the given index from the list.
// List elements after the removed element are pulled forward, so 
//their indicies decrease by one.
// If an index is passed that is larger then the number of items
// in the list, it should "wrap around" back to the first element.
void CircularListInt::remove(size_t index){
	if(empty() || index < 0){
		return;
	}
	else if(count == 1){
		delete head;
		head = nullptr;
		count--;
	}
	else {
		size_t true_count = index % count;
		Item* temp = head;
		for (size_t i = 0; i < true_count; i++){
			temp = temp->next;
		}
		if(temp == head){
			head = head->next;
		}
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
		count--;
	}
}
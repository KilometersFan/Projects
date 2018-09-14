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
	//if nothing in list return 0
	if(!count){
		return 0;
	}
	else {
		//returns a size_t from 0 to count-1
		size_t true_count = (index % count);
		//moves through the list to the desired index
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
	//if empty list, set head to mem address and set its value
	if (head == nullptr){
		head = new Item(value);
		head->next = head;
		head->prev = head;
	}
	else{
		//create new item and append to end of list
		Item* newItem = new Item(value);
		Item* temp = head->prev;
		//fix pointers so list remains intact
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
	//if empty list do nothing
	if(!count){
		return;
	}
	//returns size_t from 0 to count-1
	size_t true_index = (index % count);
	Item* temp = head;
	//move through list to desired index
	for (size_t i = 0; i < true_index; i++){
		temp = temp->next;
	}
	//set new value
	temp->value = value;
	return;
}

// Removes the item at the given index from the list.
// List elements after the removed element are pulled forward, so 
//their indicies decrease by one.
// If an index is passed that is larger then the number of items
// in the list, it should "wrap around" back to the first element.
void CircularListInt::remove(size_t index){
	//if empty list or invalid index given do nothing
	if(empty() || index < 0){
		return;
	}
	//base case for list of size 1
	else if(count == 1){
		delete head;
		head = nullptr;
		count--;
	}
	else {
		//find true index
		size_t true_index = index % count;
		Item* temp = head;
		//move through list till desired index
		for (size_t i = 0; i < true_index; i++){
			temp = temp->next;
		}
		//if temp is head, set head to next item
		if(temp == head){
			head = head->next;
		}
		//fix pointers so list remains intact
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
		count--;
	}
}
CircularListInt::Item* CircularListInt::findItem(size_t index) const {
	size_t true_count = (index % count);
	//moves through the list to the desired index
	Item* temp = head;
	for (size_t i = 0; i < true_count; i++){
		temp = temp->next;
	}
	return temp;
}
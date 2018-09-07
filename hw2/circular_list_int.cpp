#include "circular_list_int.h"
// #include <iostream>
// #include <stdlib.h>
// using namespace std;
// standard no-argument constructor

CircularListInt::CircularListInt(){

}
// Destructor. Should delete all data allocated by the list. 
CircularListInt::~CircularListInt(){
	for (size_t i = 0; i < count; i++){
		remove(i);
	}
}

// Gets item at an index.
// If an index is passed that is larger then the number of items in the list, it should "wrap around" back to the first element.
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
	if (!head){
		head = new Item(value);
		head->next = head;
		head->prev = head;
	}
	else{
		Item* newItem = new Item(value);
		Item* temp = head;
		while(temp->next != head){
			temp = temp->next;
		}
		newItem->prev = temp;
		newItem->next = head;
		temp->next = newItem;
		head->prev= newItem;
	}
	count++;
}

// Sets the item at the given index to have the given value.
// If an index is passed that is >= the number of items in the list, it should "wrap around" back to the first element.
void CircularListInt::set(size_t index, int value){
	if(!count){
		return;
	}
	else {
		size_t true_count = (index % count);
		Item* temp = head;
		for (size_t i = 0; i < true_count; i++){
			temp = temp->next;
		}
		temp->value = value;
		return;
	}
}

// Removes the item at the given index from the list.
// List elements after the removed element are pulled forward, so their indicies decrease by one.
// If an index is passed that is larger then the number of items in the list, it should "wrap around" back to the first element.
void CircularListInt::remove(size_t index){
	if(!count){
		return;
	}
	else {
		size_t true_count = (index % count);
		Item* temp = head;
		for (size_t i = 0; i < true_count; i++){
			temp = temp->next;
		}
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		if(temp == head){
			head = head->next;
		}
		temp = nullptr;
		delete temp;
	}
	count--;
}
// int main(){
// // 	CircularListInt* list = new CircularListInt();
// // 	list->push_back(10);
// // 	list->push_back(11);
// // 	list->push_back(12);
// // 	list->push_back(13);
// // 	for (size_t i = 0; i < list->size(); i++){
// // 		cout << list->get(i) << endl;
// // 	}
// // 	cout<< endl;
// // 	list->remove(7);
// // 	for (size_t i = 0; i < list->size(); i++){
// // 		cout << list->get(i) << endl;
// // 	}
// // 	list->push_back(15);
// // 	list->set(7, 90);
// // 	// list->remove(1);
// // 	// list->remove(12);
// // 	for (size_t i = 0; i < list->size(); i++){
// // 		cout << list->get(i) << endl;
// // 	}
// 	return 0; 
// }
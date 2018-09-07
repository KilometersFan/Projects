#include "circular_list_int.h"
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
		while(temp->next != nullptr){
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
		delete temp;
		return;
	}
}
int main(){
	Item* item1 = new Item(5);
	Item* item2 = new Item(2);
	Item* item3 = new Item(7);
	Item* item4 = new Item(0);
	cout << "item1: " << item1 << endl << "item2: " << item2 << endl << "item3: " << item3 << endl<< "item4: " << item4 << endl;
	item1->next = item2;
	item1->prev = nullptr;
	item2->next = item3;
	item2->prev = item1;
	item3->next = item4;
	item3->prev = item2;
	item4->next = nullptr;
	item4->prev = item3;
	Item* temp = LLSelectionSort(item1);
	int c = 0;
	while (temp != nullptr){
		cerr << c << " " << temp << endl;
		temp = temp->next;
		c++;
	}
	delete item1;
	delete item2;
	delete item3;
	delete item4;
	return 0; 
}
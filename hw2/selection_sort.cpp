#include <iostream>
#include <stdlib.h>
#include "selection_sort.h"
using namespace std;

Item* findMin(Item* head){
	if (!head) 
		return nullptr;
	if (!head->next) 
	{
		return head;
	}
	// cout << "Here is the counter " + counter++ << endl;
	Item* temp = findMin(head->next);
	if(head->getValue() < temp->getValue()) 
		return head;
	return temp;
}

Item* LLSelectionSort(Item* head){
	if (!head)
		return nullptr;
	if(!head->next)
		return head;
	Item* min = findMin(head);
	if(head == min)
		return min;
	if(min->next){
		min->next->prev = min->prev;
	}
	min->prev->next = min->next;
	min->prev = head->prev;
	head->prev = min;
	min->next = LLSelectionSort(head);
	return min;
}
<<<<<<< HEAD:hw2/selection_sort.cpp
=======

// int main() {
	// Item* item1 = new Item(5);
	// Item* item2 = new Item(2);
	// Item* item3 = new Item(7);
	// Item* item4 = new Item(2);
	// cout << "item1: " << item1 << endl << "item2: " << item2 << endl << "item3: " << item3 << endl<< "item4: " << item4 << endl;
	// item1->next = item2;
	// item1->prev = nullptr;
	// item2->next = item3;
	// item2->prev = item1;
	// item3->next = item4;
	// item3->prev = item2;
	// item4->next = nullptr;
	// item4->prev = item3;
	// Item* temp = LLSelectionSort(item1);
	// int c = 0;
	// while (temp != nullptr){
	// 	cerr << c << " " << temp << endl;
	// 	temp = temp->next;
	// 	c++;
	// }
	// delete item1;
	// delete item2;
	// delete item3;
	// delete item4;
// 	return 0; 
// }
>>>>>>> 2264d3a08b63caa8631ff297b0716eb56f8af059:hw2/selecton_sort.cpp

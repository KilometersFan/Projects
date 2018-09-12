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

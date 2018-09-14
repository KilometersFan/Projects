#include <iostream>
#include <stdlib.h>
#include "selection_sort.h"
using namespace std;

Item* findMin(Item* head){
	//if empty list return null
	if (!head) 
		return nullptr;
	//if list has one item return that item
	if (!head->next) 
	{
		return head;
	}
	// recursive loop to find min value from rest of list
	Item* temp = findMin(head->next);
	// compare front of list with min, swap if min < head
	if(head->getValue() < temp->getValue()) 
		return head;
	return temp;
}
Item* LLSelectionSort(Item* head){
	//if empty list return null
	if (!head)
		return nullptr;
	//if list has one item return that item
	if(!head->next)
		return head;
	//get smallest item
	Item* min = findMin(head);
	//base case
	//rearrange pointers to fix the list
	// if you're not at the last item in the list 
	// you fix min->next
	if(head != min){
		if(min->next){
			min->next->prev = min->prev;
		}
		min->prev->next = min->next;
		min->prev = head->prev;
		head->prev = min;
	}
	// if head = min, then shift head to the next item in the list
	else {
		head = head->next;
	}
	//calculate next smallest item using recursive call
	min->next = LLSelectionSort(head);
	//after the recursive call is done, returns the smallest
	//item
	return min;
}


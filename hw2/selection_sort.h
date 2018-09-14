#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

// ----------------------------------------
// CS104 HW2 Problem 1 skeleton header
// ----------------------------------------

// STUDENTS: DO NOT MODIFY THIS HEADER -- 
//YOUR FUNCTIONS ARE REQUIRED TO HAVE THESE SIGNATURES


struct Item 
{
public:

	// constructor taking a value (you don't have to code this)
	Item(int value): _value(value) {}

	// default copy constructor (you don't have to code this)
	Item(Item const & other) = default;

	// get the int stored in the item
	int getValue() const { return _value; }

	// pointers to next and previous items in the list.
	Item *next = nullptr;
	Item *prev = nullptr;

private:
	int _value;
};

// Given a list, returns a pointer to the smallest item.
// Returns nullptr if and only if head is nullptr.
Item* findMin(Item * head);

// Sorts the given list in ascending order and returns the new head.
// Returns nullptr iff head is nullptr.
Item* LLSelectionSort(Item * head);

#endif
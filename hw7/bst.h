#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <cmath>
#include <algorithm>

/**
* A templated class for a Node in a search tree. The getters for parent/left/right are virtual so that they
* can be overridden for future kinds of search trees, such as Red Black trees, Splay trees, and AVL trees.
*/
template <typename Key, typename Value>
class Node
{
public:
	Node(const Key& key, const Value& value, Node<Key, Value>* parent);
	virtual ~Node();

	const std::pair<Key, Value>& getItem() const;
	std::pair<Key, Value>& getItem();
	const Key& getKey() const;
	const Value& getValue() const;
	Key& getKey();
	Value& getValue();

	virtual Node<Key, Value>* getParent() const;
	virtual Node<Key, Value>* getLeft() const;
	virtual Node<Key, Value>* getRight() const;

	void setParent(Node<Key, Value>* parent);
	void setLeft(Node<Key, Value>* left);
	void setRight(Node<Key, Value>* right);
	void setValue(const Value &value);

protected:
	std::pair<Key, Value> mItem;
	Node<Key, Value>* mParent;
	Node<Key, Value>* mLeft;
	Node<Key, Value>* mRight;
};

/*
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
	: mItem(key, value)
	, mParent(parent)
	, mLeft(NULL)
	, mRight(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<Key, Value>& Node<Key, Value>::getItem() const
{
	return mItem;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& Node<Key, Value>::getItem()
{
	return mItem;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
	return mItem.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
	return mItem.second;
}

/**
* A non-const getter for the key.
*/
template<typename Key, typename Value>
Key& Node<Key, Value>::getKey()
{
	return mItem.first;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
	return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
	return mParent;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
	return mLeft;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
	return mRight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
	mParent = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
	mLeft = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
	mRight = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
	mItem.second = value;
}

/*
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
	public:
		BinarySearchTree(); //TODO
		virtual ~BinarySearchTree(); //TODO
  		virtual void insert(const std::pair<Key, Value>& keyValuePair); //TODO
        virtual void remove(const Key& key); //TODO
  		void clear(); //TODO
  		void print() const;
  		bool isBalanced() const; //TODO

	public:
		/**
		* An internal iterator class for traversing the contents of the BST.
		*/
		class iterator
		{
			public:
				iterator(Node<Key,Value>* ptr);
				iterator();

				std::pair<Key,Value>& operator*() const;
				std::pair<Key,Value>* operator->() const;

				bool operator==(const iterator& rhs) const;
				bool operator!=(const iterator& rhs) const;
				iterator& operator=(const iterator& rhs);

				iterator& operator++();

			protected:
				Node<Key, Value>* mCurrent;

				friend class BinarySearchTree<Key, Value>;
		};

	public:
		iterator begin() const;
		iterator end() const;
		iterator find(const Key& key) const;

	protected:
		Node<Key, Value>* internalFind(const Key& key) const; //TODO
		Node<Key, Value>* getSmallestNode() const; //TODO
		void printRoot (Node<Key, Value>* root) const;

	protected:
		Node<Key, Value>* mRoot;

	public:
		void print() {this->printRoot(this->mRoot);}
	private: 
		int checkHeight(Node<Key, Value>*node) const;
		int size;

};

/*
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
	: mCurrent(ptr)
{

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
	: mCurrent(NULL)
{

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const
{
	return mCurrent->getItem();
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const
{
	return &(mCurrent->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key, Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
	this->mCurrent = rhs.mCurrent;
	return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
	if(mCurrent->getRight() != NULL)
	{
		mCurrent = mCurrent->getRight();
		while(mCurrent->getLeft() != NULL)
		{
			mCurrent = mCurrent->getLeft();
		}
	}
	else if(mCurrent->getRight() == NULL)
	{
		Node<Key, Value>* parent = mCurrent->getParent();
		while(parent != NULL && mCurrent == parent->getRight())
		{
			mCurrent = parent;
			parent = parent->getParent();
		}
		mCurrent = parent;
	}
	return *this;
}

/*
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/*
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
	// TODO
	mRoot = NULL;
	size = 0;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
	// TODO
	clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& key) const
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/**
* An insert method to insert into a Binary Search Tree. The tree will not remain balanced when
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	// TODO
	if(mRoot == NULL){
		mRoot = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
		size++;
	}
	else{
		Node<Key, Value>* temp = mRoot;
		bool done = false;
		while(!done){
			if(keyValuePair.first == temp->getKey()){
				temp->setValue(keyValuePair.second);
				done = true;
			}
			else if(keyValuePair.first > temp->getKey()){
				if(temp->getRight() != NULL)
					temp = temp->getRight();
				else{
					Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
					temp->setRight(newNode);
					done = true;
					size++;
				}
			}
			else{
				if(temp->getLeft() != NULL)
					temp = temp->getLeft();
				else{
					Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
					temp->setLeft(newNode);
					done = true;
					size++;
				}
			}
		}
	}
}

/**
* An remove method to remove a specific key from a Binary Search Tree. The tree may not remain balanced after
* removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
	// TODO
	Node<Key, Value>* temp = internalFind(key);
	// std::cout << temp->getKey() << std::endl;
	if(temp == NULL)
		return;
	else{
		Node<Key, Value>* parent = temp->getParent();
		// if(parent != NULL)
			// std::cout << "PARENT" << parent->getKey() << std::endl;
		if(temp->getLeft() == NULL && temp->getRight() == NULL){
			if(parent != NULL){
				if(parent->getLeft() == temp)
					parent->setLeft(NULL);
				else
					parent->setRight(NULL);	
			}
			else {
				mRoot = NULL;
			}
		}
		else if(temp->getLeft() == NULL && temp->getRight() != NULL){
			if(parent != NULL){
				if(parent->getLeft() == temp){
					parent->setLeft(temp->getRight());
					temp->getRight()->setParent(parent);
				}
				else{
					parent->setRight(temp->getRight());
					temp->getRight()->setParent(parent);
				}
			}
			else {
				temp->getRight()->setParent(parent);
				mRoot = temp->getRight();
			}
			
		}
		else if(temp->getLeft() != NULL && temp->getRight() == NULL){
			if(parent != NULL){
				if(parent->getLeft() == temp){
					parent->setLeft(temp->getLeft());
					temp->getLeft()->setParent(parent);
				}
				else{
					parent->setRight(temp->getLeft());
					temp->getLeft()->setParent(parent);
				}
			}
			else {
				temp->getLeft()->setParent(parent);
				mRoot = temp->getLeft();
			}
		}
		else if(temp->getLeft() != NULL && temp->getRight() != NULL){
			Node<Key, Value>* predecessor = temp->getLeft();
			while(predecessor->getRight() != NULL){
				predecessor= predecessor->getRight();
			}
			// std::cout <<"PRED:" <<predecessor->getKey() << std::endl;
			Node<Key, Value>* predLeft = predecessor->getLeft();
			Node<Key, Value>* predParent = predecessor->getParent();
			if(parent != NULL){
				//fix predecessors area
				if(predParent != temp){
					predParent->setRight(predLeft);
					if(predLeft != NULL)
						predLeft->setParent(predParent);
				}
				//fix temps right and predecessor
				predecessor->setRight(temp->getRight());
				temp->getRight()->setParent(predecessor);

				//fix temps parent and predcessor
				if(predParent != temp){
					predecessor->setLeft(temp->getLeft());
					temp->getLeft()->setParent(predecessor);
				}
				predecessor->setParent(parent);
				if(parent->getLeft() == temp)
					parent->setLeft(predecessor);
				else
					parent->setRight(predecessor);
			}
			else {
				if(predParent != temp){
					predParent->setRight(predLeft);
					if(predLeft != NULL)
						predLeft->setParent(predParent);
				}
				//fix temps area
				predecessor->setParent(parent);
				predecessor->setRight(temp->getRight());
				temp->getRight()->setParent(predecessor);
				if(predParent != temp){
					predecessor->setLeft(temp->getLeft());
					temp->getLeft()->setParent(predecessor);
				}
				mRoot = predecessor;
			}
		}
		delete temp;
		size--;
	}
}

/**
* A method to remove all contents of the tree and reset the values in the tree
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
	// TODO
	const int _size = size;
	for(int i = 0; i < _size; i++){
		remove(mRoot->getKey());
	}
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
	// TODO
	Node<Key, Value>* temp = mRoot;
	if(temp == NULL)
		return temp;
	while(temp->getLeft() != NULL){
		temp = temp->getLeft();
	}
	return temp;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
	// TODO
	Node<Key, Value>* temp = mRoot;
	while(temp != NULL){
		if(key == temp->getKey())
			break;
		else{
			if(key > temp->getKey()){
				temp = temp->getRight();
			}
			else{
				temp = temp->getLeft();
			}
		}
	}
	return temp;
}

/**
 * Return true iff the BST is an AVL Tree.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
	// TODO
	int height = checkHeight(mRoot);
	if(height == -1)
		return false;
	return true;
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::checkHeight(Node<Key, Value>* node) const{
	if(node == NULL)
		return 0;
	int leftHeight = checkHeight(node->getLeft());
	int rightHeight = checkHeight(node->getRight());
	if(std::abs(leftHeight - rightHeight) > 1 || leftHeight == -1 || rightHeight == -1)
		return -1;
	int newHeight = std::max(leftHeight, rightHeight) + 1;
	return newHeight;
}


/**
 * Lastly, we are providing you with a print function, BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->mRoot)

   It will print up to 5 levels of the tree rooted at the passed node, in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif

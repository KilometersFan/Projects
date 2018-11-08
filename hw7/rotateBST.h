#include "bst.h"

template<typename Key, typename Value>
class rotateBST: public BinarySearchTree<Key, Value> {
	friend class rotateBST<Key, Value>;
	protected:
		void leftRotate(Node<Key, Value>* r);
		void rightRotate(Node<Key, Value>* r);
	public: 
		rotateBST();
		~rotateBST();
		bool sameKeys(const rotateBST& t2) const;
		void transform(rotateBST& t2) const;
};

template<typename Key, typename Value>
rotateBST<Key, Value>::rotateBST() : BinarySearchTree<Key, Value>() {

};
template<typename Key, typename Value>
rotateBST<Key, Value>::~rotateBST(){

};

template<typename Key, typename Value>
void rotateBST<Key, Value>::leftRotate(Node<Key, Value>* r){
	Node<Key, Value> rRight = r->getRight();
	Node<Key, Value> rParent = r->getParent();
	Node<Key, Value> leftTree = rRight->getLeft();
	if(rParent != NULL){
		if(rParent->getLeft() == r)
			rParent->setLeft(rRight);
		else
			rParent->setRight(rRight);
		rRight->setParent(rParent);
		rRight->setLeft(r);
		r->setParent(rRight);
		r->setRight(leftTree);
	}
	else{
		rRight->setParent(rParent);
		rRight->setLeft(r);
		r->setParent(rRight);
		r->setRight(leftTree);
		mRoot = rRight;
	}
};
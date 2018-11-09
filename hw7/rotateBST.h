#include "bst.h"

template<typename Key, typename Value>
class rotateBST: public BinarySearchTree<Key, Value> {
	protected:
		void leftRotate(Node<Key, Value>* r);
		void rightRotate(Node<Key, Value>* r);
	public: 
		rotateBST();
		~rotateBST();
		bool sameKeys(const rotateBST& t2) const;
		void transform(rotateBST& t2) const;
	private:
		void transformRightHelper(Node<Key,Value>* node);
		void transformLeftHelper(Node<Key, Value>* node, Node<Key, Value>* trueNode);
};

template<typename Key, typename Value>
rotateBST<Key, Value>::rotateBST() : BinarySearchTree<Key, Value>() {

};
template<typename Key, typename Value>
rotateBST<Key, Value>::~rotateBST(){

};

template<typename Key, typename Value>
void rotateBST<Key, Value>::leftRotate(Node<Key, Value>* r){
	Node<Key, Value>* rRight = r->getRight();
	if(rRight == NULL)
		return;
	Node<Key, Value>* rParent = r->getParent();
	Node<Key, Value>* leftTree = rRight->getLeft();
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
		this->mRoot = rRight;
	}
};
template<typename Key, typename Value>
void rotateBST<Key, Value>::rightRotate(Node<Key, Value>* r){
	Node<Key, Value>* rLeft = r->getLeft();
	if(rLeft == NULL)
		return;
	Node<Key, Value>* rParent = r->getParent();
	Node<Key, Value>* rightTree = rLeft->getRight();
	if(rParent != NULL){
		if(rParent->getLeft() == r)
			rParent->setLeft(rLeft);
		else
			rParent->setRight(rLeft);
		rLeft->setParent(rParent);
		rLeft->setRight(r);
		r->setParent(rLeft);
		r->setLeft(rightTree);
	}
	else {
		rLeft->setParent(rParent);
		rLeft->setRight(r);
		r->setParent(rLeft);
		r->setLeft(rightTree);
		this->mRoot = rLeft;
	}
};
template<typename Key, typename Value>
bool rotateBST<Key, Value>::sameKeys(const rotateBST& t2) const {
	for (typename rotateBST<Key, Value>::iterator it = this->begin(); it != this->end(); ++it){
		if(t2.internalFind(it->first) == NULL)
			return false;
	}
	for (typename rotateBST<Key, Value>::iterator it = t2.begin(); it != t2.end(); ++it){
		if(this->internalFind(it->first) == NULL)
			return false;
	}
	return true;

};

template<typename Key, typename Value>
void rotateBST<Key, Value>::transformRightHelper(Node<Key, Value>* node){
	if(node == NULL)
		return;
	Node<Key, Value>* right = node->getRight();
	while(node->getLeft() != NULL){
		rightRotate(node);
		std::cout << "ROTATING RIGHT" << std::endl;
	}
	transformRightHelper(right);
}
template<typename Key, typename Value>
void rotateBST<Key, Value>::transformLeftHelper(Node<Key, Value>* node, Node<Key, Value>* trueNode){
	if(node == NULL)
		return;
	else if(node->getKey() == trueNode->getKey()){
		return;
	}
	if(node->getKey() != trueNode->getKey()){
		leftRotate(node);
		transformLeftHelper(node->getRight(), trueNode);
	}
	if(node->getKey() != trueNode->getKey()){
		rightRotate(node);
		transformLeftHelper(node->getLeft(), trueNode);
	}
	transformLeftHelper(node->getLeft(), trueNode->getLeft());
	transformLeftHelper(node->getRight(), trueNode->getRight());
}

template<typename Key, typename Value>
void rotateBST<Key, Value>::transform(rotateBST& t2) const {
	if(!this->sameKeys(t2))
		return;
	while(t2.mRoot->getLeft() != NULL){
		t2.rightRotate(t2.mRoot);
	}
	t2.transformRightHelper(t2.mRoot->getRight());
	t2.print();
	while(t2.mRoot->getKey() != this->mRoot->getKey()){
		t2.leftRotate(t2.mRoot);
	}
	t2.transformLeftHelper(t2.mRoot, this->mRoot);
	t2.print();
}
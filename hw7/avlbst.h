#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "rotateBST.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they 
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent)
    , mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
    return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public rotateBST<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    void remove(const Key& key);
    AVLTree();
    ~AVLTree();

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
    void fixHeights(AVLNode<Key, Value>* node);
    int setHeights(AVLNode<Key, Value>* root);
    AVLNode<Key, Value>* getUnbalancedNode(AVLNode<Key, Value>* node) const;
    AVLNode<Key, Value>* getUnbalancedNodeChild(AVLNode<Key, Value>* newNode, AVLNode<Key, Value>* z) const;
    AVLNode<Key, Value>* getUnbalancedNodeHeavyChild(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y = NULL);
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
AVLTree<Key, Value>::AVLTree(){

};

template<typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree(){
    // this->clear();
    while(this->mRoot != NULL){
        AVLTree<Key, Value>::remove(this->mRoot->getKey());
        // this->print();
    }
};


template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // TODO
    AVLNode<Key, Value>* newNode = dynamic_cast<AVLNode<Key, Value>*>(this->internalFind(keyValuePair.first));
    // std::cout << newNode->getKey() << std::endl;
    if(this->mRoot == NULL){
        this->mRoot = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        newNode = dynamic_cast<AVLNode<Key,Value>*>(this->mRoot);
    }
    else{
        AVLNode<Key, Value>* temp = dynamic_cast<AVLNode<Key,Value>*>(this->mRoot);
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
                    newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
                    temp->setRight(newNode);
                    done = true;
                }
            }
            else{
                if(temp->getLeft() != NULL)
                    temp = temp->getLeft();
                else{
                    newNode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, temp);
                    temp->setLeft(newNode);
                    done = true;
                }
            }
        }
    }
    fixHeights(newNode);
    if(!(this->isBalanced())){
        AVLNode<Key, Value>* z = getUnbalancedNode(newNode);
        AVLNode<Key, Value>* y = getUnbalancedNodeChild(newNode, z);
        AVLNode<Key, Value>* x = getUnbalancedNodeChild(newNode, y);
        if(y == z->getLeft() && x == y->getLeft()){
            this->rightRotate(z);
        }
        else if(y == z->getLeft() && x == y->getRight()){
            this->leftRotate(y);
            setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
            this->rightRotate(z);
        }
        else if(y == z->getRight() && x == y->getRight()){
            this->leftRotate(z);
        }
        else if(y == z->getRight() && x == y->getLeft()){
            this->rightRotate(y);
            setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
            this->leftRotate(z);
        }
        setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
    }
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
   // TODO
    AVLNode<Key, Value>* node = (dynamic_cast<AVLNode<Key, Value>*>(this->internalFind(key)));
    AVLNode<Key, Value>* parent = node->getParent();
    if(node->getLeft() != NULL && node->getRight() != NULL){
        parent = node->getLeft();
        if(parent->getRight() != NULL){
            while(parent->getRight() != NULL){
                parent = parent->getRight();
            }
            parent = parent->getParent();
        }
    }
    else if(parent == NULL && node->getLeft() == NULL && node->getRight() != NULL)
        parent = node->getRight();
    else if (parent == NULL && node->getLeft() != NULL && node->getRight() == NULL)
        parent = node->getLeft();
    rotateBST<Key, Value>::remove(key);
    fixHeights(parent);
    setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
    // this->print();
    while(!(this->isBalanced())){
        // std::cout << "Not balanced!" << std::endl;
        AVLNode<Key, Value>* z = getUnbalancedNode(parent);
        AVLNode<Key, Value>* y = getUnbalancedNodeHeavyChild(z);
        AVLNode<Key, Value>* x = getUnbalancedNodeHeavyChild(z, y);
        if(y == z->getLeft() && x == y->getLeft()){
                this->rightRotate(z);
        }
        else if(y == z->getLeft() && x == y->getRight()){
            this->leftRotate(y);
            setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
            this->rightRotate(z);
        }
        else if(y == z->getRight() && x == y->getRight()){
            this->leftRotate(z);
        }
        else if(y == z->getRight() && x == y->getLeft()){
            this->rightRotate(y);
            setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
            this->leftRotate(z);
        }
    }
    setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::fixHeights(AVLNode<Key, Value>* node){
    if(node == NULL)
        return;
    if(node->getLeft() == NULL && node->getRight() != NULL){
        node->setHeight(node->getRight()->getHeight() + 1);
    }
    else if(node->getLeft() != NULL && node->getRight() == NULL){
        node->setHeight(node->getLeft()->getHeight() + 1);
    }
    else if(node->getLeft() != NULL && node->getRight() != NULL){
        node->setHeight(std::max(node->getLeft()->getHeight(), node->getRight()->getHeight()) + 1);
    }
    else{
        node->setHeight(1);
    }
    fixHeights(node->getParent());
}

template<typename Key, typename Value>
int AVLTree<Key, Value>::setHeights(AVLNode<Key, Value>* root){
    if(root == NULL)
        return 0;
    int leftHeight = setHeights(root->getLeft());
    int rightHeight = setHeights(root->getRight());
    int newHeight = std::max(leftHeight, rightHeight) + 1;
    root->setHeight(newHeight);
    // std::cout << "height of " << root->getKey() << ": " << root->getHeight() << std::endl;
    return newHeight;
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getUnbalancedNode(AVLNode<Key, Value>* node) const
{
    if(node == NULL)
        return NULL;
    if(node->getLeft() != NULL && node->getRight() != NULL){
        int diff = std::abs(node->getLeft()->getHeight() - node->getRight()->getHeight());
        if(diff > 1)
            return node;
    }
    else if(node->getLeft() != NULL && node->getRight() == NULL){
        if(node->getLeft()->getHeight() > 1)
            return node;
    }
    else if(node->getLeft() == NULL && node->getRight() != NULL){
        if(node->getRight()->getHeight() > 1)
            return node;
    }
    return getUnbalancedNode(node->getParent());
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getUnbalancedNodeChild(AVLNode<Key, Value>* node, AVLNode<Key, Value>* z) const
{
    if(node == NULL)
        return NULL;
    if(node->getParent() == z)
        return node;
    return getUnbalancedNodeChild(node->getParent(), z);
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getUnbalancedNodeHeavyChild(AVLNode<Key, Value>* z, AVLNode<Key, Value>* y) {
    if(z->getLeft() != NULL && z->getRight() == NULL && y == NULL)
        return z->getLeft();
    else if(z->getLeft() == NULL && z->getRight() != NULL && y == NULL)
        return z->getRight();
    else if(z->getLeft() != NULL && z->getRight() != NULL && y == NULL){
        if(z->getLeft()->getHeight() > z->getRight()->getHeight())
            return z->getLeft();
        return z->getRight();
    }
    else{
        if(y->getLeft() != NULL && y->getRight() == NULL)
            return y->getLeft();
        else if (y->getLeft() == NULL && y->getRight() != NULL)
            return y->getRight();
        else {
            if(y->getLeft()->getHeight() > y->getRight()->getHeight())
                return y->getLeft();
            else if (y->getLeft()->getHeight() < y->getRight()->getHeight())
                return y->getRight();
            else {
                if(y == z->getLeft())
                    return y->getLeft();
                else
                    return y->getRight();
            }
        }
    }
}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif

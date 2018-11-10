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
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // TODO
    AVLNode<Key, Value>* newNode;
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
    this->print();
    fixHeights(newNode);
    if(!(this->isBalanced())){
        std::cout << "Not balanced!" << std::endl;
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
        std::cout << "Balanced Tree:" << std::endl;
        this->print();
    }
    else 
        std::cout << "Balanced!" << std::endl;
    setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
   // TODO
    AVLNode<Key, Value>* temp = dynamic_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    // std::cout << temp->getKey() << std::endl;
    if(temp == NULL)
        return;
    else{
        AVLNode<Key, Value>* parent = temp->getParent();
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
                // mRoot->setHeight(0);
                this->mRoot = NULL;
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
                this->mRoot = temp->getRight();
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
                this->mRoot = temp->getLeft();
            }
        }
        else if(temp->getLeft() != NULL && temp->getRight() != NULL){
            AVLNode<Key, Value>* predecessor = temp->getLeft();
            while(predecessor->getRight() != NULL){
                predecessor= predecessor->getRight();
            }
            // std::cout <<"PRED:" <<predecessor->getKey() << std::endl;
            AVLNode<Key, Value>* predLeft = predecessor->getLeft();
            AVLNode<Key, Value>* predParent = predecessor->getParent();
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
                this->mRoot = predecessor;
            }
        }
        delete temp;
        setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
        this->print();
        if(!(this->isBalanced())){
            std::cout << "Not balanced!" << std::endl;
            AVLNode<Key, Value>* z = getUnbalancedNode(parent);
            AVLNode<Key, Value>* y = getUnbalancedNodeHeavyChild(z);
            AVLNode<Key, Value>* x = getUnbalancedNodeChild(z, y);
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
            std::cout << "Balanced Tree:" << std::endl;
            this->print();
            setHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
        }

    }
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
    std::cout << "height of " << root->getKey() << ": " << root->getHeight() << std::endl;
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
    if(z->getLeft() != NULL && z->getRight() == NULL)
        return z->getLeft();
    else if(z->getLeft() == NULL && z->getRight() != NULL)
        return z->getRight();
    else{
        if(z->getRight()->getHeight() > z->getLeft()->getHeight())
            return z->getRight();
        else if (z->getRight()->getHeight() < z->getLeft()->getHeight())
            return z->getLeft();
        else {
            if(y == z->getLeft())
                return y->getLeft();
            else
                return y->getRight();
        }
    } 

}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif

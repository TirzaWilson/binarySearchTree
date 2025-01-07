// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

// This file includes all of the pieces of the BST implementation

// Include the node implementation
#include "BSTNode.h"

// Include the dictionary ADT
#include "dictionary.h"
#include <string>

#ifndef BST_H
#define BST_H

using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <typename Key, typename E>
class BST : public Dictionary<Key, E> {
private:
    BSTNode<Key, E>* root;   // Root of the BST

    BSTNode<Key, E>* current;

    int nodecount;         // Number of nodes in the BST

    // Private "helper" functions
    void clearhelp(BSTNode<Key, E>*);
    
    BSTNode<Key, E>* inserthelp(BSTNode<Key, E>*,
        const Key&, const E&);
    
    BSTNode<Key, E>* deletemin(BSTNode<Key, E>*);
    
    BSTNode<Key, E>* getmin(BSTNode<Key, E>*);
    
    BSTNode<Key, E>* removehelp(BSTNode<Key, E>*, const Key&);
    
    E* findhelp(BSTNode<Key, E>*, const Key&) const;
    
    void printhelp(BSTNode<Key, E>*, int) const;
    
    void visit(BSTNode<Key, E>*) const;


public:
    BST() { root = NULL; nodecount = 0; }  // Constructor

    //Note from Prof Sipantzi -- I've commented out the destructor
    //since you would have to change clearhelp() to make it work with
    //doubly-threaded trees and that is not part of the assignment.
    //~BST() { clearhelp(root); }            // Destructor

    void clear()   // Reinitialize tree
    {
        clearhelp(root); root = NULL; nodecount = 0;
    }

    // Insert a record into the tree.
    // k Key value of the record.
    // e The record to insert.
    void insert(const Key& k, const E& e) {
        if (nodecount == 0) {
            root = inserthelp(root, k, e);
        } else {
            current = inserthelp(root, k, e);
        }
        nodecount++;
    }
    void printInorder() const;
    
    void printReverse() const;
    
    // Remove a record from the tree.
    // k Key value of record to remove.
    // Return: The record removed, or NULL if there is none.
    E* remove(const Key& k) {
        E* temp = findhelp(root, k);   // First find it
        if (temp != NULL) {
            root = removehelp(root, k);
            nodecount--;
        }
        return temp;
    }
    // Remove and return the root node from the dictionary.
    // Return: The record removed, null if tree is empty.
    E* removeAny() {  // Delete min value
        if (root != NULL) {
            E* temp = new E;
            *temp = root->element();
            root = removehelp(root, root->key());
            nodecount--;
            return temp;
        }
        else return NULL;
    }

    // Return Record with key value k, NULL if none exist.
    // k: The key value to find. */
    // Return some record matching "k".
    // Return true if such exists, false otherwise. If
    // multiple records match "k", return an arbitrary one.
    E* find(const Key& k) const { return findhelp(root, k); }

    // Return the number of records in the dictionary.
    int size() { return nodecount; }

    void print() const { // Print the contents of the BST
        if (root == NULL) cout << "The BST is empty.\n";
        else printhelp(root, 0);
    }

};

// Visit -- prints out root
template <typename Key, typename E>
void BST<Key, E>::visit(BSTNode<Key, E>* r) const {
    cout << "Node - " << r->element() << endl;
}

// Clean up BST by releasing space back free store
template <typename Key, typename E>
void BST<Key, E>::
clearhelp(BSTNode<Key, E>* root) {
    if (root == NULL) return;
    clearhelp(root->left());
    clearhelp(root->right());
    delete root;
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(
    BSTNode<Key, E>* root, const Key& k, const E& it) {

    BSTNode<Key, E>* node = new BSTNode<Key, E>(k, it, NULL, NULL); // Create a new node to insert into the tree with the given key and element

    if (root == NULL) { // Base case: If the root is NULL (tree is empty)
        return new BSTNode<Key, E>(k, it, NULL, NULL); }

    BSTNode<Key, E>* current = root, * previous = NULL; // Pointers for traversal: 'current' for the current node, 'previous' for the parent node

    while (current != NULL) { // Traverse the tree to find the correct location to insert the new node

        previous = current;	 // Save the current node as the previous node

        if (k < previous->key()) { // If the key is smaller than the current node's key, go left
           
            if (current->returnLThread() == 1)
                break; // Stop if the left thread indicates no left child
            else { current = current->left(); }  // Otherwise, continue traversing the left child
        } else {

            if (current->returnRThread() == 1) // If the key is greater than or equal to current node's key, go right
                break; // Stop if the right thread indicates no right child
            else { current = current->right(); } // Otherwise, continue traversing the right child
        }
    }
        if (current == NULL) { // If 'current' is NULL, this is the correct position for the new node

        if (k < previous->key()) { // Insert the new node as the left or right child of the previous node based on the key
            previous->setLeft(node);		
            node->setRight(previous);		
            node->rightThread(1);			
        } else {
            previous->setRight(node);		
            node->setLeft(previous);		
            node->leftThread(1); }
    } else { // If 'current' is not NULL, the new node is inserted at the same level as 'current'
        
        if (k < current->key()) { // Insert the new node as the child of 'current' (left or right depending on comparison)
            node->setLeft(current->left());	
            node->leftThread(1);			
            node->setRight(current);		
            node->rightThread(1);			
            current->setLeft(node);			
            current->leftThread(0);			
        } else { // Insert the new node as the right child of 'current'
            node->setRight(current->right());	
            node->rightThread(1);				
            node->setLeft(current);				
            node->leftThread(1);				
            current->setRight(node);			
            current->rightThread(0);			
        }
    }
    return node;
}
// Delete the minimum value from the BST, returning the revised BST
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::
getmin(BSTNode<Key, E>* rt) {
    if (rt->left() == NULL)
        return rt;
    else return getmin(rt->left());
}
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::
deletemin(BSTNode<Key, E>* rt) {
    if (rt->left() == NULL) // Found min
        return rt->right();
    else {                      // Continue left
        rt->setLeft(deletemin(rt->left()));
        return rt;
    }
}

// Remove a node with key value k
// Return: The tree with the node removed
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::
removehelp(BSTNode<Key, E>* rt, const Key& k) {
    if (rt == NULL) return NULL;    // k is not in tree
    else if (k < rt->key())
        rt->setLeft(removehelp(rt->left(), k));
    else if (k > rt->key())
        rt->setRight(removehelp(rt->right(), k));
    else {                            // Found: remove it
        BSTNode<Key, E>* temp = rt;
        if (rt->left() == NULL) {     // Only a right child
            rt = rt->right();         //  so point to right
            delete temp;
        }
        else if (rt->right() == NULL) { // Only a left child
            rt = rt->left();          //  so point to left
            delete temp;
        }
        else {                    // Both children are non-empty
            BSTNode<Key, E>* temp = getmin(rt->right());
            rt->setElement(temp->element());
            rt->setKey(temp->key());
            rt->setRight(deletemin(rt->right()));
            delete temp;
        }
    }
    return rt;
}
// Find a node with the given key value
template <typename Key, typename E>
E* BST<Key, E>::findhelp(BSTNode<Key, E>* root,
    const Key& k) const {
    if (root == NULL) return NULL;          // Empty tree
    if (k < root->key())
        return findhelp(root->left(), k);   // Check left
    else if (k > root->key())
        return findhelp(root->right(), k);  // Check right
    else {
        E* temp = new E;
        *temp = root->element();
        return temp;  // Found it
    }
}
// Print out a BST
template <typename Key, typename E>
void BST<Key, E>::printhelp(BSTNode<Key, E>* root, int level) const {
    
    if (root == NULL) return; // Base case: if the node is NULL, return

    if (root->returnLThread() == 1) { // If the left thread flag is set to 1
        printhelp(NULL, level); // Skip left recursion and move to the right child
    } else {
        printhelp(root->left(), level + 1); // Recursively print the left subtree (indentation)
    }
    for (int i = 0; i < level; i++) cout << "  "; 

    cout << root->key() << "\n";  // Print the current node's key

    if (root->returnRThread() == 1) { // If the right thread flag is set to 1
        printhelp(NULL, level); // Skip right recursion
    } else {
        printhelp(root->right(), level + 1); // Recursively print the right subtree (indentation)
    }
}
// Prints the BST in ascending order
template <typename Key, typename E>
void BST<Key, E>::printInorder() const {
    if (root == NULL) {
        cout << "The Tree is Empty" << endl;
        return; 
    }
        BSTNode<Key, E> *current, *previous;
        previous = NULL;
        current = root;		
    do {
        while (current != NULL) {
            previous = current;	// Mark the current node as the last visited node		

            if (current->returnLThread() == 1) // If there's no left child, break
                break; // The current node is ready to be printed
            else		
                current = current->left(); // Move to the left child			
        }
        cout << previous->element() << endl;		
        current = previous->right(); // Move to the right child (or the in-order successor)				

        while ((previous->returnRThread() == 1) && (current != NULL)) {
            cout << current->element() << endl;	// Print the current node's element	
            previous = current;	// Update previous to current node					
            current = previous->right(); // Move to the right child (or the in-order successor)			
        }
    } while (current != NULL);
}
// Prints the BST in descending order
template <typename Key, typename E>
void BST<Key, E>::printReverse() const {
        if (root == NULL) {
            cout << "The Tree is Empty" << endl;
            return; 
        }
        BSTNode<Key, E>* current, * previous;
        previous = NULL;
        current = root;			
        do { 
            while (current != NULL) { // Continue until all nodes are visited
                previous = current;	// Set previous to current node					

                if (current->returnRThread() == 1) // If the current node has a right thread
                    break;
                else
                    current = current->right();	// Move to the right child		
            }
            cout << previous->element() << endl; // Print the element of the previous node		
            current = previous->left();	// Move to the left child (or the in-order predecessor)				

            while ((previous->returnLThread() == 1) && (current != NULL)) {
                cout << current->element() << endl;	// Print the element of the current node
                previous = current;	// Set previous to current node					
                current = previous->left();	// Move to the left child or in-order predecessor			
            }
        } while (current != NULL);
}

#endif

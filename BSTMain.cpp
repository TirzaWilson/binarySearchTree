// This is an implementation of a binary search tree derived from the course work.
// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

#include <iostream>
#include "BST.h"
#include "BSTNode.h"

using namespace std;

int main() {

	cout << " <Tirza Wilson> -- CSIS 215 Programming Assignment 2 -- Threaded Binary Tree " << endl << endl; 

	BST<int, string> Node;

	Node.insert(77,"seventy-seven");
	Node.insert(70, "seventy");
	Node.insert(75, "seventy-five");
	Node.insert(66, "sixty-six");
	Node.insert(79, "seventy-nine");
	Node.insert(68, "sixty-eight");
	Node.insert(67, "sixty-seven");
	Node.insert(69, "sixty-nine");
	Node.insert(90, "ninety");
	Node.insert(85, "eighty-five");
	Node.insert(83, "eighty-three");
	Node.insert(87, "eighty-seven");
	Node.insert(65, "sixty-five");
	
	cout << "My tree size is " << Node.size() << endl << endl;
	
	Node.print();
	
	cout << endl << "Inorder printing of my tree" << endl;
	
	Node.printInorder();

	cout << endl << "Reverse order printing of my tree" << endl;

	Node.printReverse();

} 
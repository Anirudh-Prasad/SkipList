/**
 * SkipList https://en.wikipedia.org/wiki/Skip_list
 *
 * SkipList with a depth of 1 is similar to a doubly-linked list
 * Each item has a p percent chance of being at the next level up
 * For our implementation p = 50%
 * All elements are inserted at the lowest Depth (1)
 * 50% of the elements inserted in Depth = 2
 * 50% of 50%, or 25% of the elements inserted in Depth = 3
 * and so on
 *
 * If a Skip List has only 1 level, such as p = 0%
 * Insert O(n), Search O(n)
 * For Depth > 1
 * Insert O(log n), Search O(log n)
 * Modifying p allows us to trade off search speed and storage cost
 */
/*
*Author: Anirudh Prasad
*File: skiplist.h
*Date: 11/13/2019
*Description: Header file for skiplist. Implementation found in skiplist.cpp
*/

#ifndef ASS4_SKIP_LIST_SKIPLIST_H
#define ASS4_SKIP_LIST_SKIPLIST_H

#include <iostream>

using namespace std;

class SkipList {
	// display data at each level
	friend ostream& operator<<(ostream& Out, const SkipList& SkipL);

private:
	// private SNode
	// defined in .cpp as SkipList::SNode::SNode(int Data) ...
	struct SNode {
		explicit SNode(int Dta);
		~SNode() { //NOLINT
			//cout << "Deleting SNode " << this; //output
		 }
		int Data;
		// link to Next SNode
		SNode* Next;
		// link to Prev SNode
		SNode* Prev;
		// link to up one level
		SNode* UpLevel;
		// link to down one level
		SNode* DownLevel;
	};

	using Snode = struct Snode;

	// Depth of SkipList
	int Depth;

	//Member variable that tracks current depth location in list
	int LevelCtr{ 0 };

	// array of Depth SNode* objects as FrontGuards linking levels
	SNode** FrontGuards;

	// array of Depth SNode* objects as RearGuards linking levels
	SNode** RearGuards;

	// given a SNode, place it before the given NextNode
	void addBefore(SNode* NewNode, SNode* NextNode); 

	//Recursive function that adds to upper list level, 50% loop chance
	void AddUpper(SNode* Curr, int LevelCtr); //NOLINT

	//Recursive function for removing an item from the list
	void RecRemove(int Dta, int Level); //NOLINT

	// return true 50% of time,
	// each node has a 50% chance of being at higher level
	bool AlsoHigher() const; //NOLINT

public:
	// default SkipList has Depth of 1, just one doubly-linked list
	explicit SkipList(int Dpth = 1);

	// destructor
	virtual ~SkipList();

	// return true if successfully added, no duplicates
	bool add(int Dta);

	// return true if successfully removed
	bool remove(int Dta);

	// return true if found in SkipList
	bool contains(int Dta);
};

#endif // ASS4_SKIP_LIST_SKIPLIST_H


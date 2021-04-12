/* 
* Author: Anirudh Prasad
* File: skiplist.cpp
* Date: 11/13/2019
* Description: A SkipList is a data structure that stores all data
* on the lowest level of the structure. Each ata item has a 50% chance
* of being passed a level up until the max level is reached. This implementation
* reduces search times by potentially having to parse through less data.
*/

#include <climits>
#include <cstdlib>
#include <iostream>

#include "skiplist.h"

using namespace std;

//Overloads << operator to output the contents of SkipList from each level
ostream& operator<<(ostream& Out, const SkipList& SkipL) {
	for (int I = SkipL.Depth - 1; I >= 0; I--) {
		Out << "Level: " << I << " -- ";
		SkipList::SNode* Curr = SkipL.RearGuards[I];
		while (Curr != nullptr /*&& Curr != SkipL.FrontGuards[I]*/) {
			//cout << "Curr: " << Curr->Data;
			Out << Curr->Data << ", ";
			Curr = Curr->Next;
		}
		Out << endl;
	}

	return Out;
}

/*constructor for SNode structure, initializes Data to int param and sets pointers
to nullptr*/
SkipList::SNode::SNode(int Dta) {
	this->Data = Dta;
	Next = nullptr;
	Prev = nullptr;
	UpLevel = nullptr;
	DownLevel = nullptr;
}

/*Constructor for SkipList. Initializes FrontGuards and RearGuards dbl pointers
as a means to provide bounds for the array. Sets param value to depth and initializes
LevelCtr to 0. Also links depth up and down.*/
SkipList::SkipList(int Dpth){
	this->Depth = Dpth;
	FrontGuards = new SNode * [Depth]; //How do I initialize to INT_MAX?
	RearGuards = new SNode * [Depth]; //How do I initialize to INT_MIN?

	for (int I = 0; I < Depth; I++) {
		FrontGuards[I] = new SNode(INT_MAX);
		RearGuards[I] = new SNode(INT_MIN);

		FrontGuards[I]->Prev = RearGuards[I];
		RearGuards[I]->Next = FrontGuards[I];

		FrontGuards[I]->Next = nullptr; //beyond max
		RearGuards[I]->Prev = nullptr; //before min
	}

	for (int I = 0; I < Depth; I++) { //up-down
		if (Depth == 1) {
			FrontGuards[I]->DownLevel = nullptr;
			RearGuards[I]->DownLevel = nullptr;
			FrontGuards[I]->UpLevel = nullptr;
			RearGuards[I]->UpLevel = nullptr;
		}
		else if ((Depth != 1) && (I == 0)) { //lower bound
			FrontGuards[I]->DownLevel = nullptr;
			RearGuards[I]->DownLevel = nullptr;
		}
		else if ((Depth != 1) && (I + 1 == Depth)) { //upper bound
			FrontGuards[I]->UpLevel = nullptr;
			FrontGuards[I]->DownLevel = FrontGuards[I - 1];
			RearGuards[I]->UpLevel = nullptr;
			RearGuards[I]->DownLevel = RearGuards[I - 1];
		}
		else { //middle links
			FrontGuards[I]->UpLevel = FrontGuards[I + 1];
			FrontGuards[I]->DownLevel = FrontGuards[I - 1];
			RearGuards[I]->UpLevel = RearGuards[I + 1];
			RearGuards[I]->DownLevel = RearGuards[I - 1];
		}
	}
}

//returns result of probability test 
bool SkipList::AlsoHigher() const { //NOLINT
	bool RetVal = true;
	if (rand() % 2 == 1) { //NOLINT
		RetVal = false;
	}
	return RetVal;
}

/*Adds Snodes to the first level of the SkipList. 
Has a 50% chance of calling AddUpper() to add higher in the list.*/
bool SkipList::add(int Dta) {
	LevelCtr = 0;
	if (contains(Dta)) {
		return false;
	}
	if ((Dta == INT_MAX) || (Dta == INT_MIN)) {
		return false;
	}

	auto ToAdd = new SNode(Dta);
	SNode* Head = RearGuards[0];

	while (Dta > Head->Data) {
		Head = Head->Next;
	}
	addBefore(ToAdd, Head);

	if ((Depth > 1) && (AlsoHigher())) {
		AddUpper(ToAdd, ++LevelCtr);
	}

	return true;
}

//Adds a new Snode before the Next Snode. Links accordingly.
void SkipList::addBefore(SNode* NewNode, SNode* NextNode) {
	NewNode->Next = NextNode;
	NewNode->Prev = NextNode->Prev;
	NextNode->Prev->Next = NewNode;
	NextNode->Prev = NewNode;

}

//Destructor for SkipList. Deletes every SNode to prevent memory leaks
SkipList::~SkipList() {

	for (int I = 0; I < Depth; I++) {
		SNode* Parser = RearGuards[I];
		while (Parser != nullptr) {
			SNode* NxtNode = Parser->Next;
			delete Parser;
			Parser = NxtNode;
		}
		delete Parser;
	}
	delete[] FrontGuards;
	delete[] RearGuards;
}

/*Recursive function that adds item to the upper list level based
on a 50% chance*/
void SkipList::AddUpper(SNode* Curr, int LevelCtr) { //NOLINT
	auto UpperNode = new SNode(Curr->Data);
	Curr->UpLevel = UpperNode;
	UpperNode->DownLevel = Curr;

	SNode* Head = RearGuards[LevelCtr];

	while (UpperNode->Data > Head->Data) {
		Head = Head->Next;
	}
	addBefore(UpperNode, Head);

	if ((LevelCtr + 1 < Depth) && AlsoHigher()) {
		AddUpper(UpperNode, ++LevelCtr);
	}

}

/*Baseline Remove method. Provides basic invariant checking before calling 
recursive Remove function. Returns true if removed, false if invariant checks failed.*/
bool SkipList::remove(int Dta) {
	LevelCtr = 0;
	if (!contains(Dta)) return false;
	if ((Dta == RearGuards[LevelCtr]->Data) || (Dta == FrontGuards[LevelCtr]->Data)) {
		cout << "ERROR: Cannot Remove Guards" << endl;
		return false;
	}

	RecRemove(Dta, LevelCtr);
	LevelCtr = 0;
	return true;
}

//Recursively removes data item from every level of list, if possible.
void SkipList::RecRemove(int Dta, int Level) { //NOLINT
	SNode* Head = RearGuards[Level];
	while (Head->Data != Dta) {
		Head = Head->Next;
	}

	bool RetLoop = false;
	if (Head->UpLevel != nullptr) {
		++LevelCtr;
		RetLoop = true;
	}

	Head->Prev->Next = Head->Next;
	Head->Next->Prev = Head->Prev;
	delete Head;
	if(RetLoop) RecRemove(Dta, LevelCtr);
}

/*Performs a log n search of SkipList for data item
Uses head and tail pointers to search 2 locations at once*/
bool SkipList::contains(int Dta) {
	SNode* Head;
	SNode* Tail;

	for (int I = Depth - 1; I >= 0; I--) {
		Head = RearGuards[I]->Next;
		Tail = FrontGuards[I]->Prev;

		if ((Head->Data == Dta) || (Tail->Data == Dta)) {
			return true;
		}

		while (((Head->Data != INT_MAX) && (Head->Data < Dta)) ||
			((Tail->Data != INT_MIN) && (Tail->Data > Dta))) {

			if ((Head->Data == Dta) || (Tail->Data == Dta)) {
				return true;
			}
			Head = Head->Next;
			Tail = Tail->Prev;
		}
	}
	return false;
}


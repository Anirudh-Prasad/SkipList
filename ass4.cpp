//
// Created by Yusuf Pisan on 4/26/18.
// Further tests added by Anirudh Prasad on 11/17/19
// This file tests the functionality of the SkipList

#include "skiplist.h"
#include <iostream> //NOLINT
#include <cstdlib>
#include <sstream>
#include <cassert>
#include <climits>

using namespace std;

int TotalPassed = 0;
int TotalTests = 0;

//Tests if T objects match each other
template <typename T> string isOK(const T& got, const T& expected) {
	++TotalTests;
	if (got == expected) {
		++TotalPassed;
		return "OK: "; //NOLINT
	}
	else { //NOLINT
		cout << "    Got   " << got << "\n expected " << expected << endl;
		return "ERR: ";
	}
}

//Tests the add and contains function
void test02() {
	SkipList Skip(3);
	stringstream Ss;
	Ss << Skip;
	cout << isOK(Ss.str(), "Level: 2 -- -2147483648, 2147483647, \n"s +
		"Level: 1 -- -2147483648, 2147483647, \n"s +
		"Level: 0 -- -2147483648, 2147483647, \n"s)
		<< "Empty SkipList of Depth=3" << endl;
	srand(100); //NOLINT
	Skip.add(10);
	Skip.add(30);
	Skip.add(5);
	Skip.add(25);
	Ss.str("");
	Ss << Skip;
	cout << isOK(Ss.str(),
		"Level: 2 -- -2147483648, 25, 2147483647, \n"s +
		"Level: 1 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s +
		"Level: 0 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s)
		<< "SkipList of Depth=3 with 10, 30, 5, 25" << endl;
	cout << isOK(Skip.contains(10), true) << "Contains 10" << endl;
	cout << isOK(Skip.contains(30), true) << "Contains 30" << endl;
	cout << isOK(Skip.contains(71), false) << "Does not contain 71" << endl;
}

//Tests the add function
void test01() {
	SkipList Skip;
	stringstream Ss;
	Ss << Skip;
	cout << isOK(Ss.str(), "Level: 0 -- -2147483648, 2147483647, \n"s)
		<< "Empty SkipList" << endl;
	Skip.add(10);
	Skip.add(30);
	Skip.add(5);
	Skip.add(25);
	Ss.str("");
	Ss << Skip;
	cout << isOK(Ss.str(),
		"Level: 0 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s)
		<< "Added 10, 30, 5, 25," << endl;
}

//Tests add, contains, remove, and all their invariants with asserts
void TestCustom1() { //NOLINT
	cout << "Custom Test 1: " << endl;
	SkipList Skip(3);
	Skip.add(-2000);
	Skip.add(24);
	Skip.add(49);
	Skip.add(204);
	Skip.add(1);
	Skip.add(4);
	Skip.add(3);
	Skip.add(-9);
	Skip.add(4);
	Skip.remove(3);
	Skip.remove(INT_MIN); //error cout
	Skip.remove(INT_MAX); //error cout

	assert(Skip.contains(-2000));
	assert(Skip.contains(24));
	assert(Skip.contains(49));
	assert(Skip.contains(204));
	assert(Skip.contains(1));
	assert(Skip.contains(-9));
	assert(!Skip.contains(3));
	assert(!Skip.contains(9));
	assert(!Skip.add(INT_MAX));
	assert(!Skip.add(INT_MIN));
	assert(!Skip.remove(2900));
	assert(!Skip.remove(INT_MAX));
	assert(!Skip.remove(INT_MIN));

	cout << "All Asserts passed!" << endl;

	stringstream Ss;
	Ss << Skip;
	cout << Ss.str() << endl;
	Ss.str("");
}

//Adds random numbers to a larger list to view list level population
void TestCustom2() { //NOLINT
	cout << "Custom Test 2: " << endl;
	SkipList Skip(5);
	for (int I = 0; I < 20; I++) {
		Skip.add((rand() % 100) + 1); //NOLINT
	}
	stringstream Ss;
	Ss << Skip;
	cout << Ss.str() << endl;
}

//runs all tests
int main() {
	cout << "Because of the random nature of SkipList, "
		<< "the skip list levels may not match" << endl;
	// Set the random seed for easy debugging
	// NOLINTNEXTLINE
	srand(424242);
	test01();
	test02();
	cout << "Passed: " << TotalPassed << "/" << TotalTests << endl;
	TestCustom1();
	TestCustom2();
	return 0;
}
#pragma once
#include <iostream>
#include <string>

using namespace std;

class stack {
private:
	class node {//node of the stack
	public:
		string info;
		string type;
		node* prev;
		node(string info, string type) { //constructor with the parameter
			this->info = info;
			this->type = type;
			prev = NULL;
		}
	};
	node* tail;
	int size;

public:
	stack() {// constructor
		tail = NULL;
		size = 0;
	}

	~stack() { // destructor
		while (!isEmpty())
			take_last();
	}

	string get_info() {
		node* cur = tail;
		return cur->info;
	}

	string get_type() {
		node* cur = tail;
		return cur->type;
	}

	void add(string info, string type) { //adding an item to the end of the stack
		node* cur = tail;
		tail = new node(info, type);
		tail->prev = cur;
		size++;
	}

	string take_last() {//return the last item in the stack and delete them
		if (isEmpty())
			throw "The stack is empty";
		else {
			string last_info = tail->info;
			node* buff = tail->prev;
			delete tail;
			tail = buff;
			size--;
			return last_info;
		}
	}

	int get_size() { //return the stack size
		return size;
	}

	bool isEmpty() { //is the stack empty?
		if (size == 0) return true;
		else return false;
	}
};

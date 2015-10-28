#ifndef HEADERS_TREE_H_
#define HEADERS_TREE_H_

#include "Node.h"

class Tree
{
public:
	Tree();
	~Tree();

	string toString(bool precise = false);

	Node* getNode(uint* route, uint routeSize);
	Node* getNode(vector<uint> vec);

	Node* root;
private:
};

#endif
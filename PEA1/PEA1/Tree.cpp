#include "stdafx.h"
#include "Tree.h"

Tree::Tree()
{
	root = new Node();
}

Tree::~Tree()
{
	delete root;
}

string Tree::toString(bool precise)
{
	return root->printSelfAndSons(precise);
}

Node* Tree::getNode(uint* route, uint routeSize)
{
	Node* ret = root;

	for (uint i = 1; i < routeSize; i++)
	{
		ret = ret->getSon(route[i]);
	}

	return ret;
}

Node* Tree::getNode(vector<uint> vec)
{
	Node* ret = root;

	for (uint i = 1; i < vec.size(); i++)
	{
		ret = ret->getSon(vec[i]);
	}

	return ret;
}
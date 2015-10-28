#ifndef HEADERS_NODE_H_
#define HEADERS_NODE_H_

#include "MatrixGraph.h"
#include <vector>

class Node
{
public:
	Node(Node* father = nullptr);
	~Node();

	string toString(bool precise = false);
	string printSelfAndSons(bool precise = false);

	Node* getSon(uint son);
	uint getSonsNumber();
	Node* getFather();
	MatrixGraph& getGraph();
	uint getLowerBound();

	void addSon(Node* son);
	void setGraph(MatrixGraph& mg);
private:
	Node *father;
	vector<Node*> sons;
	MatrixGraph mg;
	uint lowerBound;
	bool isLowerBoundGet;
	string printSelfAndSons(bool precise, int depth);
};

#endif
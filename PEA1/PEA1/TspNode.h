#ifndef HEADERS_TSPNODE_H_
#define HEADERS_TSPNODE_H_

#include "MatrixGraph.h"

class TspNode
{
public:
	TspNode();
	TspNode(const TspNode &tsp);
	TspNode(MatrixGraph &mg);
	TspNode(MatrixGraph &mg, uint index);

	void minimizeCost();

	MatrixGraph mg;
	uint index, lowerBound;

	friend ostream & operator<<(ostream& os, const TspNode& obj);
};

#endif
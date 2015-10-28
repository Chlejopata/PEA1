#include "stdafx.h"
#include "TspNode.h"

TspNode::TspNode()
{
	;
}

TspNode::TspNode(const TspNode &tsp)
{
	mg = tsp.mg;
	index = tsp.index;
	lowerBound = tsp.lowerBound;
}

TspNode::TspNode(MatrixGraph &mg)
{
	this->mg = mg;
}

TspNode::TspNode(MatrixGraph &mg, uint index)
{
	this->mg = mg;
	this->index = index;
}

void TspNode::minimizeCost()
{
	lowerBound = mg.minimizeCost();
}

ostream & operator<<(ostream& os, const TspNode& obj)
{
	os << "[#" << obj.index << ", " << obj.lowerBound << "]";
	return os;
}

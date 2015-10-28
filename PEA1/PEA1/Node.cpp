#include "stdafx.h"
#include "Node.h"

//PUBLIC

Node::Node(Node* father)
{
	this->father = father;
	lowerBound = 0;
	isLowerBoundGet = false;
}

Node::~Node()
{
	for (uint i = 0; i < sons.size(); i++)
	{
		delete sons[i];
	}
}

string Node::toString(bool precise)
{
	stringstream retVal;
	if (precise)
	{
		retVal << "father: " + (int)father;
		for (uint i = 0; i < sons.size(); i++)
		{
			retVal << i + "# son: " + (int)sons[i];
			retVal << ", ";
		}
		retVal << "lowerBound: ";
	}

	retVal << lowerBound;
	return retVal.str();
}

string Node::printSelfAndSons(bool precise)
{
	return printSelfAndSons(precise, 0);
}

Node* Node::getSon(uint son)
{
	if (son < sons.size())
		return sons[son];
	return nullptr;
}

uint Node::getSonsNumber()
{
	return sons.size();
}

Node* Node::getFather()
{
	return father;
}

MatrixGraph& Node::getGraph()
{
	return mg;
}

uint Node::getLowerBound()
{
	if (!isLowerBoundGet)
	{
		lowerBound = mg.minimizeCost();
		isLowerBoundGet = true;
	}

	return lowerBound;
}

void Node::addSon(Node* son)
{
	if (son)
	{
		son->father = this;
		sons.push_back(son);
	}
}

void Node::setGraph(MatrixGraph& mg)
{
	this->mg = mg;
	isLowerBoundGet = false;
	lowerBound = 0;
}

//PRIVATE
string Node::printSelfAndSons(bool precise, int depth)
{
	stringstream retVal;
	retVal << depth;
	retVal << ": " + toString(precise);

	if (sons.size())
		retVal << ", ";

	for (uint i = 0; i < sons.size(); i++)
	{
		retVal << sons[i]->printSelfAndSons(precise, depth + 1) + ", ";
	}
	return retVal.str();
}
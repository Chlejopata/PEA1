#include "stdafx.h"
#include "Pair.h"

Pair::Pair(uint row, uint col)
{
	this->row = row;
	this->col = col;
}

Pair::Pair(const Pair & pair)
{
	row = pair.row;
	col = pair.col;
}

Pair & Pair::operator =(const Pair & pair)
{
	row = pair.row;
	col = pair.col;
	return *this;
}

ostream & operator<<(ostream& os, const Pair& obj)
{
	os << "[" << obj.row << ", " << obj.col << "]";
	return os;
}
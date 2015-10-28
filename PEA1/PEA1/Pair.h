#ifndef HEADERS_PAIR_H_
#define HEADERS_PAIR_H_

class Pair
{
public:
	uint row, col;

	Pair(uint row = 0, uint col = 0);
	Pair(const Pair & pair);
	Pair & operator =(const Pair & pair);
	friend ostream & operator<<(ostream& os, const Pair& obj);
	
};

#endif
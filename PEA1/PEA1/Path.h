#ifndef HEADERS_PATH_H_
#define HEADERS_PATH_H_

#include "Pair.h"

class Path
{
public:
	Path();
	Path(const Path &path);

	void addFirst(Pair &pair);
	void addLast(Pair &pair);
	
	void mergeFirst(Path &path);
	void mergeLast(Path &path);

	Path & operator =(const Path & path);
	const Pair & operator[] (uint n) const;
	friend ostream & operator<<(ostream& os, const Path& obj);

	Pair & front();
	Pair & back();

	void output();

	static vector<Path> makePaths(vector<Pair> &pairs);
	static vector<Path> mergePaths(vector<Path> &paths);
private:
	vector<Pair> path;
};

#endif
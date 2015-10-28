#include "stdafx.h"
#include "Path.h"

Path::Path()
{
	;
}

Path::Path(const Path &path)
{
	this->path = path.path;
}

void Path::addFirst(Pair &pair)
{
	path.insert(path.begin(), pair);
}

void Path::addLast(Pair &pair)
{
	path.push_back(pair);
}

void Path::mergeFirst(Path &path)
{
	this->path.insert(this->path.begin(), path.path.begin(), path.path.end());
}

void Path::mergeLast(Path &path)
{
	this->path.insert(this->path.end(), path.path.begin(), path.path.end());
}

Path & Path::operator =(const Path & path)
{
	this->path = path.path;
	return *this;
}

const Pair & Path::operator[] (uint n) const
{
	return path[n];
}

ostream & operator<<(ostream& os, const Path& obj)
{
	for (uint i = 0; i < obj.path.size(); i++)
	{
		if (i == 0)
		{
			os << obj[i].row << " -> " << obj[i].col << " -> ";
		}
		else if (i < obj.path.size() - 1)
		{
			os << obj[i].col << " -> ";
		}
		else
		{
			os << obj[i].col;
		}
	}

	return os;
}

Pair & Path::front()
{
	return path.front();
}

Pair & Path::back()
{
	return path.back();
}

void Path::output()
{
	for (uint i = 0; i < path.size(); i++)
	{
		cout << path[i];
		if (i < path.size() - 1)
		{
			cout << ", ";
		}
	}
}

vector<Path> Path::makePaths(vector<Pair> &pairs)
{
	vector<bool> used(pairs.size(), false);
	vector<Path> paths;

	for (uint i = 0; i < pairs.size(); i++)
	{
		// Iterowanie po œcie¿kach
		for (uint j = 0; j < paths.size(); j++)
		{
			if (!used[i])
			{
				// Jeœli œcie¿ka jest niepusta
				if (paths[j].path.size())
				{
					// Jeœli ³¹czy siê z pocz¹tkiem lub koñcem œcie¿ki
					if (pairs[i].row == paths[j].back().col)
					{
						paths[j].addLast(pairs[i]);
						used[i] = true;
					}
					else if (pairs[i].col == paths[j].front().row)
					{
						paths[j].addFirst(pairs[i]);
						used[i] = true;
					}
				}
			}
		}

		// Jeœli nie dodano do istniej¹cej œcie¿ki - utwórz now¹ i tam dodaj
		if (!used[i])
		{
			paths.push_back(Path());
			paths.back().addLast(pairs[i]);
			used[i] = true;
		}
	}

	mergePaths(paths);

	return paths;
}

vector<Path> Path::mergePaths(vector<Path> &paths)
{
	bool merged = true;
	while (merged && paths.size() > 1)
	{
		merged = false;
		for (uint i = 0; i < paths.size(); i++)
		{
			for (uint j = i + 1; j < paths.size(); j++)
			{
				if (paths[i].front().row == paths[j].back().col)
				{
					paths[i].mergeFirst(paths[j]);
					paths.erase(paths.begin() + j);
					merged = true;
				}
				else if (paths[i].back().col == paths[j].front().row)
				{
					paths[j].mergeFirst(paths[i]);
					paths.erase(paths.begin() + i);
					merged = true;
				}
			}
		}
	}

	return paths;
}
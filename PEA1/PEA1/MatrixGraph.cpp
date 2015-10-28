#include "stdafx.h"
#include "MatrixGraph.h"
#include "TspNode.h"

bool MatrixGraph::rndSeed = false;
// PUBLIC

using kptree::print_tree_bracketed;

MatrixGraph::MatrixGraph(uint vertexNumber)
{
	// Jeśli random seed nie został zainicjowany
	if (!MatrixGraph::rndSeed)
	{
		srand((unsigned int)time(NULL));
		MatrixGraph::rndSeed = true;
	}

	this->vertexNumber = 0;
	this->matrix = nullptr;

	// Jeśli ilość wierzchołków nie jest zerem
	if (vertexNumber)
		reserve(vertexNumber);
}

MatrixGraph::MatrixGraph(const MatrixGraph &mg)
{
	matrix = nullptr;
	vertexNumber = 0;
	this->clone(mg);
}

MatrixGraph::MatrixGraph(ifstream &input)
{
	this->vertexNumber = 0;
	this->matrix = nullptr;

	readFile(input);
}

MatrixGraph::~MatrixGraph()
{
	purge();
}

MatrixGraph & MatrixGraph::operator =(const MatrixGraph & mg)
{
	clone(mg);
	return *this;
}

void MatrixGraph::clone(const MatrixGraph &mg)
{
	if (vertexNumber != mg.vertexNumber)
		reserve(mg.vertexNumber);

	for (uint row = 0; row < vertexNumber; row++)
	{
		memcpy(matrix[row], mg.matrix[row], sizeof(int) * vertexNumber);
	}
}

void MatrixGraph::readFile(ifstream &input)
{
	uint vNum = 0;
	if (!input.is_open())
		input.open("input.txt", ios_base::in);

	input >> vNum;	// liczba wierzchołków

	if (reserve(vNum))
	{
		for (uint i = 0; i < vertexNumber; ++i)
		{
			for (uint j = 0; j < vertexNumber; j++)
			{
				input >> matrix[i][j];
			}
		}
	}
}

void MatrixGraph::writeFile(ofstream &output)
{
	if (!output.is_open())
		output.open("output.txt", ios_base::out | ios_base::trunc);

	output << vertexNumber << "\n";	// liczba wierzchołków

	for (uint i = 0; i < vertexNumber; ++i)
	{
		for (uint j = 0; j < vertexNumber; ++j)
		{
			output << matrix[i][j] << " ";
		}
		output << "\n";
	}
}

void MatrixGraph::generateGraph(uint vertexNumber, bool symmetrical)
{
	reserve(vertexNumber);
	if (vertexNumber > 1)
	{
		uint* colEnd = &vertexNumber;
		uint row = 0;

		if (symmetrical)
			colEnd = &row;

		for (; row < vertexNumber; row++)
		{
			for (uint col = 0; col <= *colEnd; col++)
			{
				if (row != col)
				{
					matrix[row][col] = rand() % (vertexNumber << 1);
					if (symmetrical)
						matrix[col][row] = matrix[row][col];
				}
				else
					matrix[row][col] = -1;
			}
		}
	}
}

void MatrixGraph::generateToFile(ofstream &output, uint vNum, double percent,
	bool overwrite)
{
	int** tmpMatrix = nullptr;
	int* tmpWeights = nullptr;
	uint tmpVertexNumber = 0, tmpEdgeNumber = 0;

	generateGraph(vNum);
	writeFile(output);
}

void MatrixGraph::output(bool noColor)
{
	const uint columnMaxValue = getColumnMaxValue();
	const uint columnValueWidth = (uint)(ceil(log10(columnMaxValue)) + 1);
	const uint vertIndexWidth = (uint)(ceil(log10(vertexNumber - 1)));


	for (uint row = 0; row < vertexNumber; row++)
	{
		cout.width(vertIndexWidth);
		cout << row << ": ";
		for (uint col = 0; col < vertexNumber; col++)
		{
			cout.width(columnValueWidth);
			cout << matrix[row][col] << " ";
		}
		cout << endl;
	}
}

void MatrixGraph::bruteForce(bool printProgress)
{
	uint townNumber = vertexNumber;
	if (!townNumber)
		return;

	if (townNumber >= 20)
		printProgress = false;

	ulong endStep = 2;
	for (uint i = 3; i <= townNumber; ++i)
		endStep *= i;

	ulong div = endStep / 100;
	if (!div)
		div = 1;
	if (div > 1000000)
		div = 1000000;

	bool newDiv = false;
	uint bestWeight = -1, currStep = 0;
	uint *townArray = new uint[townNumber];
	uint *bestRoute = new uint[townNumber];

	// Pierwsza permutacja
	for (uint i = 0; i < townNumber; ++i)
		townArray[i] = i;

	clock_t firstPerm;
	double perms = 0.0;

	clock_t overallTime = clock();
	clock_t startTime = clock();
	do
	{
		++currStep;
		double onePerm;
		if (currStep <= 1000)
			firstPerm = clock() - startTime;

		uint currWeight = matrix[townArray[townNumber - 1]][townArray[0]];
		for (uint i = 0; i < townNumber - 1; ++i)
			currWeight += matrix[townArray[i]][townArray[i + 1]];

		if (currStep <= 1000)
			onePerm = firstPerm / (double)CLOCKS_PER_SEC;

		if (currWeight < bestWeight)
		{
			bestWeight = currWeight;
			memcpy(bestRoute, townArray, townNumber * sizeof(uint));
		}

		if (currStep <= 1000)
		{
			perms += onePerm;
			if (currStep == 1000)
			{
				onePerm /= 1000.0;
				double factorial = 2;
				for (uint i = 3; i < townNumber; ++i)
					factorial *= i;
				onePerm *= endStep;
				if (printProgress)
					cout << "Przewidywany czas trwania: " << onePerm << " sekund\n";
			}
		}

		if (printProgress)
		{
			if (!(currStep % div))
			{
				clock_t endTime = clock() - startTime;
				double ratio = ((double)currStep / (double)endStep) * 100.0;
				double estTime = endTime / (double)CLOCKS_PER_SEC;
				estTime /= (div / (double)(endStep - currStep));

				cout << "\r";
				for (uint i = 0; i < 60; i++)
					cout << " ";
				cout.flush();

				cout << "\rPostep: " << ratio;
				cout << "%, przewidywany czas: " << estTime << " sekund";
				cout.flush();

				if (!newDiv)
				{
					div = (long)((double)div / (endTime / (double)CLOCKS_PER_SEC));
					newDiv = true;
				}
				startTime = clock();
			}
		}
	} while (nextPermutation(townArray, townNumber));

	double duration = (clock() - overallTime) / (double)CLOCKS_PER_SEC;

	if (printProgress)
	{
		cout << "\nDroga:\n";
		for (uint i = 0; i < townNumber; ++i)
		{
			if (i < townNumber)
				cout << bestRoute[i] << " -> ";
		}
		cout << bestRoute[0] << endl;
		cout << "Koszt drogi: " << bestWeight << endl;
		cout << "Calkowity czas trwania: " << duration << " sekund\n";
	}

	delete[] bestRoute;
	delete[] townArray;
}

void MatrixGraph::branchAndBound()
{
	clock_t overallTime = clock();
	bool finished = false;
	TspNode tmp(*this, 0);
	tmp.minimizeCost();
	tree<TspNode> root(tmp);

	// Początkowa pozycja w drzewie
	tree <TspNode>::iterator currentNode = root.begin();

	/*for (uint i = 0; i < vertexNumber; i++)
	{
		cout << i << " ";
	}
	cout << endl;*/

	bool addedChild = false;
	do
	{
		addedChild = false;
		uint dummy;
		vector<bool> visited = buildVisited(currentNode, vertexNumber);
		vector<uint> path = buildPath(currentNode, dummy);
		for (uint i = 0; i < vertexNumber; i++)
		{
			/*string logic;
			visited[i] ? (logic = "V ") : (logic = "X ");
			cout << logic; */
			
			// Jeśli wierzchołek nie został jeszcze odwiedzony
			if (!visited[i])
			{
				TspNode tmp(currentNode->mg, i);
				tmp.mg.setRow(currentNode->index);
				tmp.mg.setCol(i);

				for (uint j = 0; j < path.size(); j++)
				{
					tmp.mg.setValue(i, path[j]);
				}

				// Minimalizacja kosztu
				tmp.minimizeCost();
				tmp.lowerBound += currentNode->mg.getValue(currentNode->index, i);
				tmp.lowerBound += currentNode->lowerBound;

				// Dodanie grafu do drzewa
				root.append_child(currentNode, tmp);
				addedChild = true;
			}
		}

		//cout << endl<<"Finding minimum in leaves: ";
		// Szukanie najmniejszego rozwiązania
		uint minimum = MAXUINT;
		for (tree<TspNode>::leaf_iterator iter = root.begin_leaf(); iter != root.end_leaf(); iter++)
		{
			//cout << iter->lowerBound << ", ";
			if (iter->lowerBound < minimum)
			{
				minimum = iter->lowerBound;
				currentNode = iter;
			}
		}

		//cout << endl;
	} while (addedChild);

	double duration = (clock() - overallTime) / (double)CLOCKS_PER_SEC;

	/*cout << "\nTree: \n";
	print_tree_bracketed(root);*/

	uint lowerBound;
	vector<uint> path = buildPath(currentNode, lowerBound);
	cout << "Droga: ";
	for (uint i = 0; i < path.size(); i++)
	{
		cout << path[i];
		if (i < path.size() - 1)
		{
			cout<< " -> ";
		}
		else
		{
			cout << endl;
		}
		
	}

	cout << "Koszt drogi: " << lowerBound << endl;
	cout << "Calkowity czas trwania: " << duration << " sekund\n";
	
}

int MatrixGraph::getValue(uint row, uint col)
{
	return matrix[row][col];
}

uint MatrixGraph::getVertexNumber()
{
	return vertexNumber;
}

// PROTECTED

bool MatrixGraph::reserve(uint vNumber)
{
	// Jeśli ilość wierzchołków nie jest zerem
	if (vNumber)
	{
		// Czyszczenie grafu
		purge();

		vertexNumber = vNumber;
		matrix = new int*[vertexNumber];

		int** endLoop = matrix + vertexNumber;
		for (int** i = matrix; i < endLoop; ++i)
		{
			*i = new int[vNumber];
			memset(*i, 0, sizeof(int) * vNumber);
		}

		return true;
	}

	return false;
}

void MatrixGraph::purge()
{
	if (matrix)
	{
		int** endLoop = matrix + vertexNumber;
		for (int** i = matrix; i < endLoop; ++i)
		{
			if (*i)
			{
				delete[] * i;
				*i = nullptr;
			}
		}

		delete[] matrix;
		matrix = nullptr;
	}

	vertexNumber = 0;
}

// PRIVATE
uint MatrixGraph::getMatrixAbsMax()
{
	int retVal = 0;

	int** endOuterLoop = matrix + vertexNumber;
	for (int** i = matrix; i < endOuterLoop; ++i)
	{
		int* endInnerLoop = *i + vertexNumber;
		for (int* j = *i; j < endInnerLoop; ++j)
		{
			if (abs(*j) > retVal)
				retVal = abs(*j);
		}
	}

	return retVal;
}

uint MatrixGraph::getColumnMaxValue()
{
	uint retVal = getMatrixAbsMax() + 1;

	if (retVal < vertexNumber - 1)
		retVal = vertexNumber - 1;

	return retVal;
}

int MatrixGraph::getLowestInRow(uint row)
{
	if (row >= vertexNumber)
	{
		return -1;
	}
	else
	{
		int lowest = MAXINT;
		for (uint col = 0; col < vertexNumber; col++)
		{
			if (matrix[row][col] < lowest && matrix[row][col] >= 0)
				lowest = matrix[row][col];
		}

		if (lowest == MAXINT)
			return -1;
		else
			return lowest;
	}
}

int MatrixGraph::getLowestInCol(uint col)
{
	if (col >= vertexNumber)
	{
		return -1;
	}
	else
	{
		int lowest = MAXINT;
		for (uint row = 0; row < vertexNumber; row++)
		{
			if (matrix[row][col] < lowest && matrix[row][col] >= 0)
				lowest = matrix[row][col];
		}

		if (lowest == MAXINT)
			return -1;
		else
			return lowest;
	}
}

int MatrixGraph::getLowestInRow(uint row, int& secondLowest)
{
	if (row >= vertexNumber)
	{
		secondLowest = -1;
		return -1;
	}
	else
	{
		int lowest = MAXINT;
		secondLowest = MAXINT;
		uint positive = 0;
		for (uint col = 0; col < vertexNumber; col++)
		{
			if (matrix[row][col] >= 0)
			{
				++positive;
				if (matrix[row][col] <= lowest)
				{
					if (lowest < secondLowest)
						secondLowest = lowest;
					lowest = matrix[row][col];
				}
				else
				{
					if (matrix[row][col] < secondLowest)
						secondLowest = matrix[row][col];
				}
			}
		}
		if (secondLowest == MAXINT)
			secondLowest = -1;
		if (positive == 1)
			secondLowest = lowest;

		if (lowest == MAXINT)
			return -1;
		else
			return lowest;
	}
}

int MatrixGraph::getLowestInCol(uint col, int& secondLowest)
{
	if (col >= vertexNumber)
	{
		secondLowest = -1;
		return -1;
	}
	else
	{
		int lowest = MAXINT;
		secondLowest = MAXINT;
		uint positive = 0;
		for (uint row = 0; row < vertexNumber; row++)
		{
			if (matrix[row][col] >= 0)
			{
				++positive;
				if (matrix[row][col] <= lowest)
				{
					if (lowest < secondLowest)
						secondLowest = lowest;
					lowest = matrix[row][col];
				}
				else
				{
					if (matrix[row][col] < secondLowest)
						secondLowest = matrix[row][col];
				}
			}
		}
		if (secondLowest == MAXINT)
			secondLowest = -1;
		if (positive == 1)
			secondLowest = lowest;

		if (lowest == MAXINT)
			return -1;
		else
			return lowest;
	}
}

int MatrixGraph::getSecondLowestInRow(uint row)
{
	if (row >= vertexNumber)
		return -1;
	else
	{
		int lowest = MAXINT;
		uint zerosCount = 0, positive = 0;
		for (uint col = 0; col < vertexNumber; col++)
		{
			if (matrix[row][col] < lowest && matrix[row][col] >= 0)
			{
				if (!matrix[row][col] && zerosCount++)
				{
					lowest = matrix[row][col];
				}
				else if (matrix[row][col])
				{
					lowest = matrix[row][col];
				}
			}
		}
		return lowest;
	}
}

int MatrixGraph::getSecondLowestInCol(uint col)
{
	if (col >= vertexNumber)
		return -1;
	else
	{
		int lowest = MAXINT;
		uint zerosCount = 0;
		for (uint row = 0; row < vertexNumber; row++)
		{
			if (matrix[row][col] < lowest && matrix[row][col] >= 0)
			{
				if (!matrix[row][col] && zerosCount++)
				{
					lowest = matrix[row][col];
				}
				else if (matrix[row][col])
				{
					lowest = matrix[row][col];
				}
			}
		}
		return lowest;
	}
}

void MatrixGraph::substractFormRow(uint row, int sub)
{
	if (sub > 0)
	{
		for (uint col = 0; col < vertexNumber; col++)
		{
			if (matrix[row][col] >= 0)
				matrix[row][col] -= sub;
		}
	}
}

void MatrixGraph::substractFormCol(uint col, int sub)
{
	if (sub > 0)
	{
		for (uint row = 0; row < vertexNumber; row++)
		{
			if (matrix[row][col] >= 0)
				matrix[row][col] -= sub;
		}
	}
}

void MatrixGraph::setRow(uint row, int value)
{
	for (uint col = 0; col < vertexNumber; col++)
	{
		matrix[row][col] = value;
	}
}

void MatrixGraph::setCol(uint col, int value)
{
	for (uint row = 0; row < vertexNumber; row++)
	{
		matrix[row][col] = value;
	}
}

void MatrixGraph::setValue(uint row, uint col, int value)
{
	if (row < vertexNumber && col < vertexNumber)
		matrix[row][col] = value;
}

uint MatrixGraph::minimizeCost()
{
	/*cout << "\n----------------------------------------" << endl;
	cout << "BEFORE minimizeCost():\n";
	output();*/
	uint lowerBound = 0;
	// Minimalizacja kosztów w wierszach
	for (uint row = 0; row < vertexNumber; row++)
	{
		int tmp = getLowestInRow(row);
		if (tmp > 0)
		{
			lowerBound += tmp;
			substractFormRow(row, tmp);
		}
	}

	// Minimalizacja kosztów w kolumnach
	for (uint col = 0; col < vertexNumber; col++)
	{
		int tmp = getLowestInCol(col);
		if (tmp > 0)
		{
			lowerBound += tmp;
			substractFormCol(col, tmp);
		}
	}

	/*cout << "\nAFTER minimizeCost():\n";
	output();
	cout <<"lowerBound:"<<lowerBound<<"\n----------------------------------------" <<endl;*/
	return lowerBound;
}

bool MatrixGraph::reduceMatrix(vector<Pair>& route, uint &lowerBound)
{
	vector<int> rowMinimum(vertexNumber, -1);
	vector<int> colMinimum(vertexNumber, -1);
	bool isReducable = false;

	// Wyszukiwanie drugich najmniejszych wartości
	for (uint i = 0; i < vertexNumber; i++)
	{
		int secondLowest;
		// Jeśli pierwsza jest zerem, wtedy druga jest istotna
		if (!getLowestInRow(i, secondLowest))
		{
			rowMinimum[i] = secondLowest;
			isReducable = true;
		}

		if (!getLowestInCol(i, secondLowest))
		{
			colMinimum[i] = secondLowest;
			isReducable = true;
		}
	}

	if (isReducable)
	{

		cout << "rowMinimum: ";
		for (uint i = 0; i < vertexNumber; i++)
			cout << rowMinimum[i] << " ";

		cout << "\ncolMinimum: ";
		for (uint i = 0; i < vertexNumber; i++)
			cout << colMinimum[i] << " ";
		cout << endl;

		Pair pair;
		int max = INT_MIN;
		// Szukanie pary wartości która po zsumowaniu jest największa
		for (uint row = 0; row < vertexNumber; row++)
		{
			for (uint col = 0; col < vertexNumber; col++)
			{
				if (rowMinimum[row] >= 0 && colMinimum[col] >= 0 && !matrix[row][col])
				{
					int value = rowMinimum[row] + colMinimum[col];
					if (value > max)
					{
						max = value;
						pair.row = row;
						pair.col = col;
					}
				}
			}
		}

		cout << "Pair: " << pair << ", rowMin: " << rowMinimum[pair.row]
			<< ", colMin: " << colMinimum[pair.col] << ", sum: " << max << endl;

		// "Zmniejszanie" rozmiaru macierzy 
		setRow(pair.row);
		setCol(pair.col);
		// Zakazanie powrotu
		setValue(pair.col, pair.row);

		route.push_back(pair);

		vector<Path> paths = Path::makePaths(route);

		for (uint i = 0; i < paths.size(); i++)
		{
			//cout << "\tPath #" << i << ": " << paths[i] << endl;
			// Jeśli któryś koniec równa się któremuś początkowi - zabroń dojścia na krańce
			setValue(paths[i].back().col, paths[i].front().row);
			//cout << "[" << paths[i].back().col << ", " << paths[i].front().row << "] marked as -1\n";
		}
	}
	else
	{
		cout << "NO ZEROS PRESENT - ADDING LAST PAIRS\n";
		lowerBound += addLastPairs(route);
	}

	cout << "Matrix: \n";
	output();

	//cout << "\nRoute: \n";
	/*for (uint i = 0; i < route.size(); i++)
	{
		cout << route[i];
		if (i < route.size() - 1)
			cout << ", ";
	}
	cout << endl;*/
	return isReducable;
}

uint MatrixGraph::addLastPairs(vector<Pair>& route)
{
	uint lowerBound = 0;
	for (uint row = 0; row < vertexNumber; row++)
	{
		for (uint col = 0; col < vertexNumber; col++)
		{
			if (matrix[row][col] >= 0)
			{
				route.push_back(Pair(row, col));
				lowerBound += matrix[row][col];
			}
		}
	}

	return lowerBound;
}

long MatrixGraph::noRepeatDraw(bool* drawn, uint length)
{
	uint draw = 0, notDrawn = 0, retVal = 0;

	// Ile liczb jest niewylosowanych
	for (uint i = 0; i < length; ++i)
	{
		if (!drawn[i])
			++notDrawn;
	}

	// Która liczba z niewylosowanych ma być wylosowana
	if (notDrawn)
		draw = rand() % notDrawn;
	else
		return -1;

	for (uint i = 0; i <= draw; ++retVal)
	{
		if (!drawn[retVal])
			++i;
	}

	drawn[--retVal] = true;
	return retVal;
}

bool MatrixGraph::nextPermutation(uint *array, uint length)
{
	// Znajduje nierosnący ciąg
	if (length == 0)
		return false;
	int i = length - 1;
	while (i > 0 && array[i - 1] >= array[i])
		i--;
	if (i == 0)
		return false;

	// Znajdź następnik pivota
	int j = length - 1;
	while (array[j] <= array[i - 1])
		j--;
	int temp = array[i - 1];
	array[i - 1] = array[j];
	array[j] = temp;

	// Odwróć ciąg
	j = length - 1;
	while (i < j)
	{
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
		i++;
		j--;
	}
	return true;
}

vector<bool> MatrixGraph::buildVisited(tree <TspNode>::iterator currentNode, uint vertexNumber)
{
	vector<bool> visited(vertexNumber, false);
	tree<TspNode>::iterator parent;

	while (currentNode != nullptr)
	{
		parent = tree<TspNode>::parent(currentNode);
		visited[currentNode->index] = true;
		currentNode = parent;
	}

	return visited;
}

vector<uint> MatrixGraph::buildPath(tree <TspNode>::iterator currentNode, uint &lowerBound)
{
	vector<uint> path;
	tree<TspNode>::iterator parent;
	
	if (currentNode != nullptr)
		lowerBound = currentNode->lowerBound;

	//cout << endl << endl << "lowerBounds in path: ";

	while (currentNode != nullptr)
	{
		parent = tree<TspNode>::parent(currentNode);
		path.insert(path.begin(), currentNode->index);
		currentNode = parent;
	}
	
	//cout << "\nPath: ";

	return path;
}

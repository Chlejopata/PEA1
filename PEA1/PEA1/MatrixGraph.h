#ifndef HEADERS_MATRIXGRAPH_H_
#define HEADERS_MATRIXGRAPH_H_

#include "Path.h"

class TspNode;

class MatrixGraph
{
public:
	MatrixGraph(uint vertexNumber = 0);
	MatrixGraph(const MatrixGraph &mg);
	MatrixGraph(ifstream &input);
	virtual ~MatrixGraph();

	MatrixGraph & operator =(const MatrixGraph & mg);
	void clone(const MatrixGraph &mg);
	void readFile(ifstream &input);
	void writeFile(ofstream &output);
	void generateGraph(uint vertexNumber, bool symmetrical = true);
	void generateToFile(ofstream &output, uint vNum, double percent, bool overwrite = false);
	void output(bool noColor = false);
	void bruteForce(bool printProgress);
	void branchAndBound();

	int getValue(uint row, uint col);

	uint getVertexNumber();
	uint minimizeCost();

protected:
	bool reserve(uint vNumber = 0);
	void purge();

private:
	static bool rndSeed;
	// Macierz w postaci [wierzchołki][krawędzie]
	uint vertexNumber;
	int** matrix;	// macierz
	// Zwraca maksymalną wartość w macierzy
	uint getMatrixAbsMax();
	// Zwraca maskymalną wartość jaka jest zapisana w kolumnie
	// (macierz, waga lub numer kolumny)
	uint getColumnMaxValue();

	int getLowestInRow(uint row);
	int getLowestInCol(uint col);

	int getLowestInRow(uint row, int& secondLowest);
	int getLowestInCol(uint col, int& secondLowest);

	int getSecondLowestInRow(uint row);
	int getSecondLowestInCol(uint col);

	void substractFormRow(uint row, int sub);
	void substractFormCol(uint col, int sub);

	void setRow(uint row, int value = -1);
	void setCol(uint col, int value = -1);
	void setValue(uint row, uint col, int value = -1);

	bool reduceMatrix(vector<Pair>& route, uint &lowerBound);
	uint addLastPairs(vector<Pair>& route);

	long noRepeatDraw(bool* drawn, uint length);

	static bool nextPermutation(uint *array, uint length);
	static vector<bool> buildVisited(tree <TspNode>::iterator currentNode, uint vertexNumber);
	static vector<uint> buildPath(tree <TspNode>::iterator currentNode, uint &lowerBound);
};

#endif /* HEADERS_MATRIXGRAPH_H_ */

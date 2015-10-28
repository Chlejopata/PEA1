/*
 * menu.h
 *
 *  Created on: 5 maj 2015
 *      Author: bartosz
 */

#ifndef HEADERS_MENU_H_
#define HEADERS_MENU_H_

#include "MatrixGraph.h"

#define FMENU_SIZE 7

enum Option
{
	LOAD_GRAPH,
	SAVE_GRAPH,
	GENERATE_GRAPH,
	SHOW,
	BRANCH_AND_BOUND,
	BRUTE_FORCE,
	BACK,
	EXIT,
	UNDEFINED
};

void start();
Option showFirstMenu(MatrixGraph* mg, const char* title);
void loadGraph(MatrixGraph* mg);
void saveGraph(MatrixGraph* mg);
void genGraph(MatrixGraph* mg);
void showGraph(MatrixGraph* mg);
void branchAndBound(MatrixGraph* mg);
void bruteForce(MatrixGraph* mg);

#endif /* HEADERS_MENU_H_ */

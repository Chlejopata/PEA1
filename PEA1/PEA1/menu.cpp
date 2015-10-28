/*
 * menu.cpp
 *
 *  Created on: 5 maj 2015
 *      Author: bartosz
 */
#pragma once
#include "stdafx.h"
#include "menu.h"

void start()
{
	setlocale(LC_ALL, "polish");
	MatrixGraph* mg = new MatrixGraph();

	const char titleMain[] =
	{ "Reprezentacje grafów i algorytmy" };
	const char titleMatrix[] =
	{ "Macierz incydencji" };
	const char titleList[] =
	{ "Lista następników" };

	Option op = UNDEFINED;
	do
	{
		op = showFirstMenu(mg, titleMatrix);
	} while (op != EXIT);

	delete mg;
}

Option showFirstMenu(MatrixGraph* mg, const char* title)
{
	Option retVal = UNDEFINED;
	char op = 0;

	const char* firstMenu[] =
	{ "Wczytaj graf z pliku", "Zapisz graf do pliku", "Wygeneruj graf losowo", "Wyswietl", "Algorytm B&B", "Algorytm BruteForce", "Wyjscie" };

	do
	{
		cout << endl;
		system("cls");
		cout << title << endl;
		for (uint i = 0; i < FMENU_SIZE; ++i)
		{
			cout << "\t" <<  i + 1 << ": ";
			cout << firstMenu[i] << endl;
		}

		cout << ">: ";
		op = getchar();
	} while (op < '1' || op > '7');

	switch (op)
	{
	case '1':
		retVal = LOAD_GRAPH;
		loadGraph(mg);
		break;
	case '2':
		retVal = SAVE_GRAPH;
		saveGraph(mg);
		break;
	case '3':
		retVal = GENERATE_GRAPH;
		genGraph(mg);
		break;
	case '4':
		retVal = SHOW;
		showGraph(mg);
		break;
	case '5':
		retVal = BRANCH_AND_BOUND;
		branchAndBound(mg);
		break;
	case '6':
		retVal = BRUTE_FORCE;
		bruteForce(mg);
		break;
	case '7':
		retVal = EXIT;
		break;
	default:
		retVal = UNDEFINED;
		break;
	}
	return retVal;
}

void loadGraph(MatrixGraph* mg)
{
	string path;
	ifstream ifs;
	bool opened = true;
	do
	{
		cout << endl;
		system("cls");
		if (!opened)
			cout << "Nie udalo sie otworzyc" << endl;
		cout << "Wprowadz sciezke (powrot -> 0 + ENTER): ";
		cin >> path;

		if (path == "0")
		{
			opened = false;
			break;
		}

		ifs.open(path.c_str(), ios_base::in);
		opened = ifs.is_open();
	} while (!opened);

	if (opened)
	{
		if (ifs.is_open())
		{
			cout << "Wczytuje macierz..." << endl;
			mg->readFile(ifs);
			cout << "Wczytano" << endl;
		}

		if (ifs.is_open())
			ifs.close();

		while (getchar() != '\n')
			;
		getchar();
	}
}

void saveGraph(MatrixGraph* mg)
{
	string path;
	ofstream ofs;
	bool opened = true;
	do
	{
		cout << endl;
		system("cls");
		if (!opened)
			cout << "Nie udalo sie otworzyc" << endl;
		cout << "Wprowadz sciezke (powrot -> 0 + ENTER): ";
		cin >> path;

		if (path == "0")
		{
			opened = false;
			break;
		}

		ofs.open(path.c_str(), ios_base::out | ios_base::trunc);
		opened = ofs.is_open();
	} while (!opened);

	if (opened)
	{
		if (ofs.is_open())
		{
			cout << "Zapisuje macierz..." << endl;
			mg->writeFile(ofs);
			cout << "Zapisano" << endl;
		}

		if (ofs.is_open())
			ofs.close();

		while (getchar() != '\n')
			;
		getchar();
	}
}

void genGraph(MatrixGraph* mg)
{
	long vert = 0;

	cout << endl;
	system("cls");
	do
	{
		cout << "Podaj ilosc wierzcholkow: ";
		cin >> vert;
	} while (vert <= 0);

	mg->generateGraph(vert);
}

void showGraph(MatrixGraph* mg)
{
	cout << endl;
	system("cls");

	if (mg->getVertexNumber())
		mg->output(true);
	else
		cout << "Graf jest pusty";

	while (getchar() != '\n')
		;
	getchar();
}

void branchAndBound(MatrixGraph* mg)
{
	cout << endl;
	system("cls");

	if (mg->getVertexNumber())
		mg->branchAndBound();
	else
		cout << "Graf jest pusty";

	while (getchar() != '\n')
		;
	getchar();
}

void bruteForce(MatrixGraph* mg)
{
	cout << endl;
	system("cls");

	if (mg->getVertexNumber())
		mg->bruteForce(true);
	else
		cout << "Graf jest pusty";

	while (getchar() != '\n')
		;
	getchar();
}
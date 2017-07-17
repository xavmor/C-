#pragma once

#ifndef SUDOKU_H
#define SODUKU_H

#include <iostream>

class Data
{
private:
	int table[9][9];

public:
	Data() {
		for (int i = 0; i < 9; i++)	{
			for (int j = 0; j < 9; j++)	{
				table[i][j] = 0;
			}
		}
	}

	// No destructor needed I beleive
	//~Data() { 	}

	/*****************************************
	* Methods
	*****************************************/
	bool insert(int, int, char);
	Data returnData();

};

/*******************************************************
*
*
*
*******************************************************/
bool Data::insert(int number_answer, int row, char col)
{
	if (table[row][col] == 0)
		table[row][col] = number_answer;
	else
		return false;

	return true;
}

/*******************************************************
*
*
*
*******************************************************/
Data Data::returnData()
{

}




# endif SUDOKU_H 
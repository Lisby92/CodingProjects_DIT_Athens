/*************************************************************************
Implementation File	: TyposStoixeiouMenu.h
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: Header file of TyposStoixeiouMenu.c
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#ifndef __TSMENU__
#define __TSMENU__
#include <stdio.h>
#include <string.h>
#define SIZE 100

typedef struct Komvos {
	char  Name[SIZE]; 	// Name of Category or Plate
	int Category;	// 0 if Plate, 1 if Category
	float Price;	// 0 if category , valid only if Plate
}TStoixeiouMenu;

//========== PROTOTYPE FUNCTIONS ============//
//============= TO HANDLE DATA ==============//

void TSSetValue(TStoixeiouMenu *Lhs, TStoixeiouMenu Rhs);
int TSPrintValue(TStoixeiouMenu V);
int TSReadValue(TStoixeiouMenu * V);

#endif

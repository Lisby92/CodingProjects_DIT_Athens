/*************************************************************************
Implementation File	: TyposStoixeiouMenu.c
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: Data handling of Menu items
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#include "TyposStoixeiouMenu.h"
#include <stdlib.h>
#include <stdio.h>

//================= DATA FUNCTIONS ====================//

void TSSetValue(TStoixeiouMenu *Lhs, TStoixeiouMenu Rhs)
{//sets data
	Lhs->Category = Rhs.Category;
	if(!Rhs.Category)//is plate
		Lhs->Price = Rhs.Price;
	strcpy(Lhs->Name, Rhs.Name);
} // Lhs = Rhs

int TSPrintValue(TStoixeiouMenu V)
{//output data
	int fd;

	if(!V.Category)//is not a category
		fd = fprintf(stdout,"%s : %.2f", V.Name, V.Price);
	else
		fd = fprintf(stdout,"%s", V.Name);
	return fd;
}
int TSReadValue(TStoixeiouMenu * V)
{//input data
	int fd;

 	fd = fscanf(stdin,"%s %d %f", V->Name, &(V->Category), &(V->Price));
 	getchar();
 	if(fd == 3) //3 data types from each node
 	{
 		if(V->Category == 0 || V->Category == 1)
 			return 0;//all ok
 	}
 	return 1;
}

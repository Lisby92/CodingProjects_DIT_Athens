/*************************************************************************
Implementation File	: typos_stoixeiouDDA.c
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Implementation of data handling functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#include "typos_stoixeiouDDA.h"

int TSDDA_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	if(strcmp(s1.word, s2.word) == 0)
		return 1;
	else
		return 0;
}

int TSDDA_mikrotero(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	if(strcmp(s1.word,s2.word) < 0)
		return 1;
	else
		return 0;		
}

int TSDDA_megalytero(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	if(strcmp(s1.word,s2.word) > 0)
		return 1;
	else
		return 0;	
}

void TSDDA_setValue (TStoixeiouDDA *target, TStoixeiouDDA source)
{
	target->index = source.index;
	strncpy(target->word, source.word, SIZE);
}
 
int TSDDA_readValue (FILE *from, TStoixeiouDDA * Elem)
{
	int fd;

    fd = fscanf(from,"%s",Elem->word);
      
    if(fd == 1)
	   	return 1;
	else if(fd == EOF)
		return 0;   
    else
      	return 0;
}

int TSDDA_writeValue(FILE *to,TStoixeiouDDA Elem)
{
	int fd;

	fd = fprintf(to, "%d : %s, ", Elem.index, Elem.word);
	return fd;
}
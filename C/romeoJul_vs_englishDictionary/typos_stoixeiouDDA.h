/*************************************************************************
Header File			: typos_stoixeiouDDA.h
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Data handling functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#ifndef __TStoixeioyDDA__ 
#define __TStoixeioyDDA__
#include <stdio.h>
#include <string.h>
#define SIZE 50 //biggest english word at 45ish (chemichal compounds excluded)

typedef struct dedomena {	// tree node data
        char    word[SIZE]; /* a word, the key of BST*/
        int 	index; 		/* the index of a word to an element of the array of frequencies */
} TStoixeiouDDA;

/* basic comparisons to use in insert-delete-search functions of BST */
int TSDDA_iso(TStoixeiouDDA Elem1, TStoixeiouDDA Elem2);
int TSDDA_mikrotero(TStoixeiouDDA Elem1, TStoixeiouDDA Elem2);
int TSDDA_megalytero(TStoixeiouDDA Elem1, TStoixeiouDDA Elem2);
/* set-read-write data functions */
void TSDDA_setValue (TStoixeiouDDA *target, TStoixeiouDDA source);
int TSDDA_readValue (FILE *from, TStoixeiouDDA * Elem);
int TSDDA_writeValue(FILE *to, TStoixeiouDDA Elem);

#endif
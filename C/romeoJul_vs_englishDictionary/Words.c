/*************************************************************************
Implementation File	: Words.c
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Implementation of words functions through DDA functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Words.h"
#include "typos_stoixeiouDDA.h"

#define Simple 1 /* 0 for AVL and 1 for Simple BST */
   
#if (Simple)
#include "ch8_BSTpointer.h" /* for Simple implemenentaion */
#else /*AVL*/
#include "ch8_AVLpointer.h" /* for AVL implementation */
#endif

typedef struct RecWords 
{
     typos_deikti WordsRiza; 		/* Words includes the actual BST */
     int SearchFrequencies[100000]; /* array of times each word is found */
     int wordCounter;				/* counts current number of words in BST*/
     int wordsFound;				/* counter of words found in BST */
     int wordsNotFound;				/* counter of words not found in BST */
     double InsertTime [10];   		/* insert time after 1024, 2048,... words*/
     double CheckTime;         		/* search time */
     double DiadromhTime;	  		/* inorder route time */
} RecWords;

typosWords dhmiourgia_Words()
{
   typosWords WordsNew=malloc(sizeof(RecWords));
   
#if (Simple)
   printf("use simple BST\n");
   Tree_dimiourgia(&(WordsNew->WordsRiza));
#else /*AVL*/
    printf("use AVL BST\n");
    AVLTree_dimiourgia(&(WordsNew->WordsRiza));
#endif	
	WordsNew->wordCounter=0;
	WordsNew->wordsFound=WordsNew->wordsNotFound=0;
	
   return WordsNew;         
}

void katastrofh_Words(typosWords * Wordsptr)
{
#if (Simple)
   Tree_katastrofi(&(*Wordsptr)->WordsRiza);
#else /*AVL*/
   AVLTree_katastrofi(&(*Wordsptr)->WordsRiza);
#endif
   free(*Wordsptr);
   *Wordsptr=NULL;
}

void InsertWord(typosWords Words, char * w)
{
	/* Input w
		sets stoixeio DDA (kai ta 2 melh) word + index (wordCounter)
		calls eisagogi_komvou sto DDA 
		sets SearchFrequency[wordCounter]=0;
		wordCounter++;
	*/
	TStoixeiouDDA X;
	int 		  error[1];

	X.index = Words->wordCounter;
    strncpy(X.word, w, SIZE);

#if (Simple)
	Tree_eisagogi(&(Words->WordsRiza), X, error);
#else /*AVL*/
	int 				ypsilotero[1];
	AVLTree_eisagogi(&(Words->WordsRiza), X, ypsilotero, error);
#endif
	Words->SearchFrequencies[Words->wordCounter] = 0;
	Words->wordCounter ++;

}

void CheckWord(typosWords Words, char * w)
{
	/* Input w
		sets w se neo stoixeio DDA
		calls anazitisi_komvou and
		if found 
			calls periexomena and increases frequency in array SearchFrequencies[index]
	*/
	TStoixeiouDDA X;
	typos_deikti  tn_pos;
	int 		  found[1];
	int 		  tn_index;

	strncpy(X.word, w, SIZE);

#if (Simple)
	Tree_anazitisi(Words->WordsRiza, X, &tn_pos, found);
	if(found[0] == 1)//found element
	{
		Tree_periexomeno(tn_pos, &X);
		tn_index = X.index;
		Words->SearchFrequencies[tn_index] ++;
		Words->wordsFound ++;
	}
	else//didnt find element
		Words->wordsNotFound ++;
#else /*AVL*/
	AVLTree_anazitisi(Words->WordsRiza, X, &tn_pos, found);
	if(found[0] == 1)//found element
	{
		AVLTree_periexomeno(tn_pos, &X);
		tn_index = X.index;
		Words->SearchFrequencies[tn_index] ++;
		Words->wordsFound ++;
	}
	else//didnt find element
		Words->wordsNotFound ++;
#endif
}

/* inorder and visit functions implemented through Tree_ and AVLTree_ functions */

void Visit(typos_deikti tree, typosWords Words, FILE *out)
{   //prints node's data if visited while following the inorder route 
	TStoixeiouDDA X;

#if (Simple)
	Tree_periexomeno(tree, &X);
#else /*AVL*/
	AVLTree_periexomeno(tree, &X);
#endif
	if(Words->SearchFrequencies[X.index] >= 1)
	{
		TSDDA_writeValue(out, X);
		fprintf(out, "was found with frequency : %d.\n", Words->SearchFrequencies[X.index]);
	}
}


void Inorder_Route(typos_deikti tree, typosWords Words, FILE *out)
{   // follows inorder route recursively

#if (Simple)
	typos_deikti paidi;

	if(!Tree_keno(tree))
	{   
		paidi = Tree_apaidi(tree);
        Inorder_Route(paidi, Words, out);
		Visit(tree, Words, out);
		paidi = Tree_dpaidi(tree);
        Inorder_Route(paidi, Words, out);
	}
#else /*AVL*/
	typos_deikti paidi;

	if(!AVLTree_keno(tree))
	{   
		paidi = AVLTree_apaidi(tree);
        Inorder_Route(paidi, Words, out);
		Visit(tree, Words, out);
		paidi = AVLTree_dpaidi(tree);
        Inorder_Route(paidi, Words, out);
	}
#endif

}

void ShowCommonWords(FILE *out, typosWords Words)
{
	/* inorder route of BST and printing of elements with frequenxy >= 1 */
	fprintf(out, "------------------------------------------------------\n");
	fprintf(out, "RESULTS BELOW\n------------------------------------------------------\n");
	Inorder_Route(Words->WordsRiza, Words, out);
}

/* functions to handle timers */

void SetInsertTime(typosWords Words, double time, int position)
{
	/* Input time, position
		sets Words.InsertTime[position]=time;
	*/	
	Words->InsertTime[position] = time;		
}

void SetCheckTime(typosWords Words, double time)
{
	/* Sets Search Times */
	Words->CheckTime = time;
}

void SetDiadromhTime(typosWords Words, double time)
{	
	/* Input time
		sets Words.DiadromhTime=time;
	*/
	Words->DiadromhTime = time;
}

void PrintData(FILE *out, typosWords Words)
{
	/* prints InsertTimes, DiadromhTime and Counters*/
	int 			   i;
	
	fprintf(out, "------------------------------------------------------");
	fprintf(out, "\nTIMERS AND COUNTERS\n------------------------------------------------------\nInsert times after 1024, 2048, .... words :\n");
	for(i = 0; i<= 6; i++)
		fprintf(out, "%f milliseconds ", Words->InsertTime[i]);
	fprintf(out, "\nTime spent on searching common words : %f milliseconds\n", Words->CheckTime);
	fprintf(out, "Time spent on following inorder route : %f milliseconds\n", Words->DiadromhTime);
	fprintf(out, "Total words inserted : %d\n", Words->wordCounter);
	fprintf(out, "Total words found : %d\n", Words->wordsFound);
	fprintf(out, "Total words not found : %d\n\n", Words->wordsNotFound);
}



/*************************************************************************
Header File			: Words.h
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Word handling functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#ifndef __TYPOS_WORDS__ 
#define __TYPOS_WORDS__

/* ALIASING */
typedef struct RecWords * typosWords; 

/* Prototype Functions */

/* functions implemented through BST functions */
typosWords dhmiourgia_Words();
void katastrofh_Words(typosWords * Wordsptr);
void InsertWord(typosWords Words, char * w);
void CheckWord(typosWords Words, char * w);
void ShowCommonWords(FILE *out, typosWords Words);

/* functions that handle timers */
void SetInsertTime(typosWords Words, double time, int position);
void SetCheckTime(typosWords Words, double time);
void SetDiadromhTime(typosWords Words, double time);

void PrintData(FILE *out, typosWords Words);

#endif
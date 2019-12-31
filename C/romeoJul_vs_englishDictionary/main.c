/*************************************************************************
Implementation File : main.c
Author Date         : V. Mavrommatis 04/05/2018
Purpose             : Client program to find common words between 2 text files
Revision            : < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "typos_stoixeiouDDA.h"
#include "Words.h"
#include <time.h>

void InitialiseTree(FILE *wordlist, typosWords W);
void SearchUpdateTree(FILE *wordlist, typosWords W);
void Results(FILE *out, typosWords W);


int main(void)
{ 	
	typosWords 		A = NULL;	//pointer to info tree node
   	FILE 				*WordList; 	//input abc...z, random_order , freeq_based
   	FILE 				*Text;		//Romeo n' Jul
   	FILE 				*Output;	//our results

   	WordList = fopen("wordsByABC.txt", "r");
   	if(WordList == NULL)
   	{
   		printf("Error : Input file can't be opened\n");
   		exit(1);
   	}

   	Text     = fopen("RomeoAndJuliet.txt", "r");
   	if(Text == NULL)
   	{
   		printf("Error : Input file can't be opened\n");
   		exit(1);
   	}

   	Output	 = fopen("Simple_ABC.txt", "w+");
   	if(Output == NULL)
   	{
   		printf("Error : Output file can't be opened\n");
   		exit(1);
   	}

  	A = dhmiourgia_Words();  	
  	InitialiseTree(WordList, A);
	  SearchUpdateTree(Text, A);
	  Results(Output, A);
	  katastrofh_Words(&A);
	
	  fclose(WordList);
	  fclose(Text);
	  fclose(Output);

  	return 0;
}

void InitialiseTree(FILE * wordlist, typosWords W)
{
	/* Reads words from wordlist and inserts into DDA using InsertWord
		after 1024, 2048, 4096,... words sets array times using SetInsertTime 
	*/
	  clock_t          t1, t2;
   	double           ratio = 1./CLOCKS_PER_SEC;
   	double           time;
   	int              i = 512;
   	int              index = 0;
   	int              counter = 0;
   	TStoixeiouDDA    X;
   	int 	           result = 1;

   	while(index <= 6 && result!= 0)
   	{
   		i*= 2;
  /* Record the start time */
   		t1 = clock();
		while(counter < i)
		{
			result = TSDDA_readValue(wordlist, &X);
			if(result == 0)
				break;
			InsertWord(W, X.word);
			counter ++;
		}
	/* Record the end time */
    	t2 = clock();
    	time = ratio*(long)t2 - ratio*(long)t1;
    	time*= 1000.0; //convert to milliseconds
    	SetInsertTime(W, time, index);
    	index ++;
   	}
}

void SearchUpdateTree(FILE *wordlist, typosWords W)
{
	/* Reads words from wordlist and calls CheckWord 
	   updates times using SetCheck Time
	*/
	clock_t 		     t1, t2;
	double 			     ratio = 1./CLOCKS_PER_SEC;
 	double 			     time = 0.0;
	TStoixeiouDDA    X;
 	int 			       result = 1;

	while(result)
	{
  	result = TSDDA_readValue(wordlist, &X);
   	if(!result)
   		break;
   	/* Record the start time */
   	t1 = clock();
   	CheckWord(W, X.word);
   	/* Record the end time */
   	t2 = clock();
   	time += (ratio*(long)t2 - ratio*(long)t1); //we dont want to add reading time just the searching part
   	}
   	time *= 1000.0; //convert to milliseconds
    SetCheckTime(W, time);
}

void Results(FILE *out, typosWords W)
{
	/* calls ShowCommonWords, updates time diadromh using SetDiadromhTime and Prints times and Counters*/
	clock_t 		 t1, t2;
  double 			 ratio = 1./CLOCKS_PER_SEC;
  double 			 time = 0.0;

  /* Record the start time */
	t1 = clock();
	ShowCommonWords(out, W);
  /* Record the end time */
	t2 = clock();
	time = ratio*(long)t2 - ratio*(long)t1;
	time *= 1000.0; //convert to milliseconds
	SetDiadromhTime(W, time);
	
	PrintData(out, W);
}
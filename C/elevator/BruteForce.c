/* 1115201200106 */
/* Vasileios Mavrommatis */
/* Project 3 */
/* elevator.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "_generate.h"

// Global ints we want all functions to have access to //
int nrid, nst, *dests;
// Prototypes of bfw, gen_comb_norep_lex_init , gen_comb_norep_lex_next and sort_bot_top functions // 
int gen_comb_norep_lex_init(unsigned char *vector, const unsigned char n, const unsigned char k);

int gen_comb_norep_lex_next(unsigned char *vector, const unsigned char n, const unsigned char k);

int bfw(int a, int b);

void sort_bot_top(int *array, int size);

// Brute Force Method //
#ifdef BF
int solve(int nrid, int nst, int *dests)
{
	int 		  b, d, i, j, l, x;
	unsigned char k, n;
	int 		  min_cost, cost, minD, walk_cost;
	int 		  diff_dests, nfl, flag;
	int           gen_result;         				// Return value of generation functions //
	unsigned int  combs;        					// Counting generated sequences //
	unsigned char *stops, *all_stops, *min_stops;

	walk_cost = 0;
	nfl = 0;
	for(i = 0; i < nrid; i++)
	{
		walk_cost += *(dests + i);
		if(*(dests + i) >= nfl)
		{
			nfl = *(dests + i);
		}
	}
	// Sorting the dests array //
	sort_bot_top(dests, nrid);
	// Some minor checks for (not too much)"special" data //
	if(walk_cost == 0)
	{
		printf("No elevator stops");
		return walk_cost;
	}
	if(nst == 0)
	{
		printf("No elevator stops");
		return walk_cost;
	}
	else if(nst == 1)
	{
		min_cost = walk_cost;
		flag = 0;
		for(i = 0; i < nrid; i++)
		{
			d = *(dests + i);
			for(j = 0; j < (nfl + 1); j++)
			{
				cost = bfw(j, j);
				if(cost < min_cost)
				{
					min_cost = cost;
					flag = j;
				}
			}
		}
		printf("\nElevator stop is : %d\n", flag);
		return min_cost;
	}
	else if(nst == 2)
	{
		min_cost = walk_cost;
		x = 0;
		l = 0;
		for(i = 0; i < nrid; i++)
		{
			d = *(dests + i);
			for(j = 0; j < (nfl + 1); j++)
			{
				for(b = j; b < (nfl + 1); b++)
				{
					cost = bfw(j, b);
					if(cost < min_cost)
					{
						min_cost = cost;
						x = j;
						l = b;
					}
				}
			}
		}
		if(x == 0)
		{
			printf("\nElevator stop is : %d\n", l);
			return min_cost;
		}
		printf("\nElevator stops are : %d %d\n", x, l);
		return min_cost;
	}
	diff_dests = nrid;
	for(i = 0; i < (nrid - 1); i++)
	{
		d = *(dests + i);
		for(j = (i + 1); j < nrid; j++)
		{
			b = *(dests + j);
			if(b == d)
			{
				diff_dests --;
			}
		}
	}
	if(diff_dests <= nst)
	{
		printf("\nElevator stops are : ");
		for(i = 0; i < nrid; i++)
		{
			d = *(dests + i);
			if(d == 0)
			{
				continue;
			}
			flag = 0;
			for(j = (i + 1); j < nrid; j++)
			{
				b = *(dests + j);
				if(b == d)
				{
					flag = 1;
				}
			}
			if(flag == 0)
			{
				printf("%d ", d);
			}
		}
		return 0;
	}	
	// Main part is here //
	// Will generate all possible combinations bottom to top of nst stops from logical 0 to nfl //
	// And calculate the minimal cost for each combination holding them minimal stops for that one //
	// Thats combinations without repetition : n! / ((n - k)! * k!) == Binomial(n, k) when k <= n //
	n = nfl;
	n++;
	k = nst;
	stops = malloc(k * sizeof(unsigned char));
	min_stops = malloc(k * sizeof(unsigned char));
	for(i = 0; i < nst; i++)
	{
		*(min_stops + i) = 0;
	}
	combs = 0;
	for(i = 0; i < (n + 1); i ++)
	{
		*(stops + i) = i;
	}
	gen_result = gen_comb_norep_lex_init(stops, n, k);
	min_cost = walk_cost;
	if(gen_result == GEN_EMPTY)
 	{
 		combs++;
 	}
 	
 	all_stops = malloc(k * sizeof(unsigned char));
 	for(x = 0; x < k; x++)
 	{
 		*(all_stops + x) = 0;
 	}
	while(gen_result == GEN_NEXT)
	{
		combs ++;
 		for(x = 0; x < nst; x++)
 		{
 			all_stops[x] = stops[x];
 		}
 		cost = 0;
 		for(j = 0; j < nrid; j++)
 		{
 			d = *(dests + j);
			minD = 255;
			for(l = 0; l < nst; l++)
			{ 
				if(abs(d - *(all_stops + l)) < minD)
				{
					minD = abs(d - *(all_stops + l));
				}
			}
			if(d < minD)
			{
				cost += d;
			}
			else
			{
				cost += minD;
			}
 		}
 		minD = 255;
 		if(cost == 0)
 		{
 			printf("\nElevator stops are : ");
 			for (j = 0; j < nst; j++)										
 			{
 				if(*(all_stops + j) != 0)
 				{
 					printf("%d ", *(all_stops + j));
 				}
 			}
 			return cost;
 		}
 		else if(cost < min_cost)
 		{
 			min_cost = cost;
 			for(j = 0; j < nst; j++)
 			{
 				*(min_stops + j) = *(all_stops + j);
 			}
 		}
 		gen_result = gen_comb_norep_lex_next(stops, n, k);
 	}
 	printf("\nElevator stops are : ");
 	for (j = 0; j < nst; j++)
 	{
 		if(*(min_stops + j) != 0)
 		{
 			printf("%d ", *(min_stops + j));
		}
	}
 	return min_cost;
}	
#endif
// main function starts here //
int main(void)
{
	int cost, i;
	
	scanf("%d%d", &nrid, &nst);
	dests = malloc(nrid * sizeof(int));
	for(i = 0; i < nrid; i++)
	{
		scanf("%d", (dests + i));
	}
	printf("\n");
	cost = solve(nrid, nst, dests);
	printf("\nCost is : %d", cost);
	printf("\n");
	free(dests);
	return 0;
}
// bfw function starts here //
// Was supposed to be fw...but wasn't completely useless //
int bfw(int a, int b)
{
	int i, d, cost;
	
	cost = 0;
	for(i = 0; i < nrid; i ++)
	{
		d = *(dests + i);
		if((d < abs(b - d)) && (d < abs(a - d)))
		{
			// Better walk //
			cost += d;
		}
		else if(abs(b - d) < abs(d - a))
		{
			// b is closer //
			cost += abs(b - d);
		}
		else
		{
			// a is closer //
			cost += abs(d - a);
		}
	}
	return cost;
}
// gen_comb_norep_lex_init function starts here //
int gen_comb_norep_lex_init(unsigned char *vector, const unsigned char n, const unsigned char k)
{

	int j;
	// Test for special cases //
	if(k > n)
	{
 		return(GEN_ERROR);
	}

	if(k == 0)
	{
 		return(GEN_EMPTY);
	}
	// Initialize: vector[0, ..., k - 1] are 0, ..., k - 1 //
	for(j = 0; j < k; j++)
	{
		vector[j] = j;
	}
	return(GEN_NEXT);
}
// gen_comb_norep_lex_next function starts here //
int gen_comb_norep_lex_next(unsigned char *vector, const unsigned char n, const unsigned char k)
{
	
	int j;
	// Easy case, increase rightmost element //
	if(vector[k - 1] < n - 1)
 	{
 		vector[k - 1]++;
 		return(GEN_NEXT);
 	}
	// Find rightmost element to increase //
	for(j = k - 2; j >= 0; j--)
 	{
 		if(vector[j] < n - k + j)
  		{
  			break;
  		}
  	}
	// Terminate if vector[0] == n - k //
	if(j < 0)
 	{
 		return(GEN_TERM);
 	}
	// Increase //
	vector[j]++;
	// Set right-hand elements //
	while(j < k - 1)
 	{
 		vector[j + 1] = vector[j] + 1;
 		j++;
	}
	return(GEN_NEXT);
}
// sort_bot_top function starts here //
void sort_bot_top(int *array, int size)
{
	int i, j, temp;

	for(i = 0; i < size; i++)
	{
		for(j = (i + 1); j < size; j++)
		{
		 // if(*(array + i) < *(array + j)) for top_bot //
			if(*(array + i) > *(array + j))
			{
				// swap the values //
				temp = *(array + i);
				*(array + i) = *(array + j);
				*(array + j) = temp;
			}
		}
	}
}
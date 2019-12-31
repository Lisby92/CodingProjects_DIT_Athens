/* 1115201200106 */
/* Vasileios Mavrommatis */
/* Project 3 */
/* elevator.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

// Global ints we want all functions to have access to //
int nrid, nst, *dests, **mem, **stops_DP;

// Prototypes of fw, M, M_DP and M_mem functions //
int fw(int a, int b);

int M(int i, int j);

int M_mem(int i, int j);

int M_DP(int i, int j);

/* Recursive method */
#ifdef REC
int solve(int nrid, int nst, int *dests)
{	
	int i;
	int cost, min_cost, walk_cost;
	int nfl, last;
	
	last = 0;
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
	min_cost = walk_cost;
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
	// Main part is here //
	// Maths is : M(i,j) == (k = 0) ~ i:( min: { M(k , (j - 1)) - fw(k , inf) + fw(k , i) + fw(i , inf) } , 0 <= i <= nfl , 1 <= j <= nst //
	// min_cost == (i = 0) ~ nfl:( min: { M(i , nst) } ) last stop == i //
	min_cost = INT_MAX;
	for(i = 0; i <= nfl; i++)
	{
		cost = M(i, nst);
		if(cost < min_cost)
		{
			last = i;
			min_cost = cost;
		}
	}
	printf("\nLast stop at floor : %d", last);
	return min_cost;
}
#endif
/* Recursive with memorisation */
#ifdef MEM
int solve(int nrid, int nst, int *dests)
{
	int i, j;
	int cost, min_cost, walk_cost;
	int nfl, last;

	last = 0;
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
	min_cost = walk_cost;
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
	// Dynamically allocate memory to hold the minimum costs in the mem array //
	mem = malloc((nfl + 1) * sizeof(int *));
	if(mem == NULL)
	{
		return -1;
	}
	for(i = 0; i < (nfl + 1); i++)
	{
		*(mem + i) = malloc((nst + 1) * sizeof(int));
		if(*(mem + i) == NULL)
		{
			return -1;
		}
	}
	// Initialize array Mi,0 = walking , and all rest infinite to indicare current cost hasnt been calculated yet //
	for(i = 0; i < (nfl + 1); i++)
	{
		for(j = 0; j < (nst + 1); j++)
		{
			*(*(mem + i) + j) = INT_MAX;
		}
	}
	for(i = 0; i < (nfl + 1); i++)
	{
		*(*(mem + i) + 0) = walk_cost;
	}
	min_cost = INT_MAX;
	// Going top to bottom to calculate all the possible minimum costs //
	for(i = nfl; i >= 0; i--)
	{
		cost = M_mem(i, nst);
		if(cost <= min_cost)
		{
			last = i;
			min_cost = cost;
		}
	}
	printf("\nLast stop at floor : %d", last);
	return min_cost;
}
#endif
/* Dynamic programming */
#ifdef DP
int solve(int nrid, int nst, int *dests)
{
	int i, j;
	int cost, min_cost, walk_cost;
	int nfl, last, flag, stop;
	int *all_stops, counter;

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
	// Some base cases of troll input ;p //
	if(walk_cost == 0)
	{
		for(i = 0; i <= nfl; i++)
		{
			printf("  %d", walk_cost);
		}
		printf("\nNo elevator stops");
		return walk_cost;
	}
	if(nst == 0)
	{
		for(i = 0; i <= nfl; i++)
		{
			if(walk_cost > 99)
			{
				printf("%d ", walk_cost);
			}
			else if(cost > 9)
			{
				printf(" %d ", walk_cost);
			}
			else
			{
				printf("  %d ", walk_cost);
			}
		}
		printf("\nNo elevator stops");
		return walk_cost;
	}
	// Dynamically allocate memory to hold the minimum costs in the mem array //
	mem = malloc((nfl + 1) * sizeof(int *));
	for(i = 0; i < (nfl + 1); i++)
	{
		*(mem + i) = malloc((nst + 1) * sizeof(int));
	}
	// Initialize array Mi,0 = walking , and all rest infinite to indicate current cost hasnt been calculated yet //
	for(i = 0; i < (nfl + 1); i++)
	{
		for(j = 0; j < (nst + 1); j++)
		{
			*(*(mem + i) + j) = INT_MAX;
		}
	}
	for(i = 0; i < (nfl + 1); i++)
	{
		*(*(mem + i) + 0) = walk_cost;
	}
	min_cost = INT_MAX;
	// Dynamic array to hold the k stop where the minimum cost of M(i,j-1) was found //
	stops_DP = malloc((nfl + 1) * sizeof(int *));
	for(i = 0; i < (nfl + 1); i++)
	{
		*(stops_DP + i) = malloc((nst + 1) * sizeof(int));
	}
	for(i = 0; i < (nfl + 1); i++)
	{
		for(j = 0; j < (nst + 1); j++)
		{
			*(*(stops_DP + i) + j) = 0;
		}
	}
	printf("\n");
	// Going bottom to top to calculate all the possible minimum costs //
	for(j = 0; j < (nst + 1); j ++)
	{
		for(i = 0; i < (nfl + 1); i++)
		{
			// Columns of 3 digits max thats 999 max cost //
			cost = M_DP(i, j);
			if(cost > 99)
			{
				printf("%d ", cost);
			}
			else if(cost > 9)
			{
				printf(" %d ", cost);
			}
			else
			{
				printf("  %d ", cost);
			}
			if(cost < min_cost)
			{
				last = i;
				min_cost = cost;
				stop = j;
			}
		}
		printf("\n");
	}
	all_stops = malloc((nst + 1) * sizeof(int));
	for(i = 0; i < (nst + 1); i++)
	{
		*(all_stops + i) = 0;
	}
	*(all_stops + nst) = last;
	counter = stop; 
	while(counter >= 0)
	{
		stop = *(*(stops_DP + last) + counter);
		counter --;
		last = stop;
		*(all_stops + counter) = last;
	}
	stop = *(all_stops + nst);
	// Checking case only one stop needed //
	flag = 0;
	for(i = 0; i < nst; i++)
	{
		if((*(all_stops + i) != stop) && (*(all_stops + i) != 0))
		{
			flag = 1;
		}
	}
	if(flag == 0)
	{
		printf("Elevator stop is : %d", stop);
		return min_cost;
	}
	// Multiple stops are needed //
	printf("Elevator stops are : ");
	for(i = 0; i < (nst +1); i++)
	{
		if(*(all_stops + i) != 0)
		{
			printf("%d ", *(all_stops + i));
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
// fw function starts here //
int fw(int a, int b)
{
	int i, d, cost;

	cost = 0;
	if(a == 0)
	{
		if(b == INT_MAX)
		{
			for(i = 0; i < nrid; i++)
			{
				cost += *(dests + i);
			}
			return cost;
		}
		else if(b < INT_MAX)
		{
			for(i = 0; i < nrid; i++)
			{
				d = *(dests + i);
				if(d <= b)
				{
					if(d < abs(b - d))
					{
						cost += d;
					}
					else
					{
						cost += abs(b - d);
					}
				}
			}
			return cost;
		}
	}
	else
	{
		if(a == b)
		{
			return 0;
		}
		if(b == INT_MAX)
		{
			for(i = 0; i < nrid; i++)
			{
				d = *(dests + i);
				if(d > a)
				{
					cost += abs(d - a);
				}
			}
			return cost;
		}
		else
		{
			for(i = 0; i < nrid; i++)
			{
				d = *(dests + i);
				if((d < b) && (d > a))
				{
					if(abs(b - d) < abs(d - a))
					{
						cost += abs(b - d);
					}
					else
					{
						cost += abs(d - a);
					}
				}
			}
			return cost;	
		}
	}
}
// M function starts here //
int M(int i, int j)
{
	int k, cost, walk_cost, min, x, nfl;

	walk_cost = 0;
	nfl = 0;
	for(x = 0; x < nrid; x++)
	{
		walk_cost += *(dests + x);
		if(*(dests + x) >= nfl)
		{
			nfl = *(dests + x);
		}
	}
	// Base case to exit the recursion //
	if(j == 0)
	{
		return walk_cost;
	}
	else
	{
		min = INT_MAX;
		for(k = 0; k <= i; k++)
		{
			cost = (M(k, (j - 1)) - fw(k, INT_MAX) + fw(k, i) + fw(i, INT_MAX));
			if(cost < min)
			{
				min = cost;
			}
		}
		return min;
	}
}
// M_mem function starts here //
int M_mem(int i, int j)
{
	int k, l, flag, cost, min;

	// Base case to exit the recursion //
	if(j == 0)
	{
		return *(*(mem + i) + j);
	}
	// Have already found cost so wont need to calculate again //
	else if(*(*(mem + i) + j) != INT_MAX)
	{
		return *(*(mem + i) + j);
	}
	// Calculate the minimum cost and put it in the array //
	else
	{
		min = INT_MAX;
		for(k = 0; k <= i; k++)
		{
			cost = (M_mem(k, (j - 1)) - fw(k, INT_MAX) + fw(k, i) + fw(i, INT_MAX));
			if(cost < min)
			{
				min = cost;
			}
		}
		*(*(mem + i) + j) = min;
		return min;
	}
}
// M_DP function starts here //
int M_DP(int i, int j)
{
	int k, l, flag, cost, min;

	// Base case to exit the recursion //
	if(j == 0)
	{
		return *(*(mem + i) + j);
	}
	// Have already found cost so wont need to calculate again //
	else if(*(*(mem + i) + j) != INT_MAX)
	{
		return *(*(mem + i) + j);
	}
	// Calculate the minimum cost and put it in the array //
	else
	{
		min = INT_MAX;
		for(k = 0; k <= i; k++)
		{
			cost = (M_DP(k, (j - 1)) - fw(k, INT_MAX) + fw(k, i) + fw(i, INT_MAX));
			if(cost < min)
			{
				min = cost;
				flag = k;
			}
		}
		*(*(mem + i) + j) = min;
		*(*(stops_DP + i) + j) = flag;
		return min;
	}
}
// ######## PROJECT 1 ######## //
// ###### 1115201200106 ###### //
// ## Basileios Mavrommatis ## //

#include <stdio.h>
#define K 8
#define MAXNUM 2147483645

int main()
	
{
	int current, div, j, next, rem, div_f, flag, counter, i ;
	
	flag = 0;
	counter = 0;
	i = 4;						
	while(i<= MAXNUM)				//check numbers until MAXNUM//
	{
		flag = 0;					//initialize flag, K potent concurrent numbers = counter//
		counter = 0;				// div is the possible divisor of the i term, j = counter helping to print concurrent numbers found up to K// 
		div = 2;
		j = 0;
		
		while((div*div) <= i && flag == 0)		//check for possible divisors up to sqrt(i), no point looking further//
		{
			if(i % (div*div) == 0)
			{
				flag = 1;						//found > out of the loop//
			}
			else
			{
				div ++;							//else check next divisor//
			}
		}
		if(flag ==1)							//if found//
		{
			counter = 1;						//have to increment counter of conc numbers and current will be the next number to check if is nsqf number//
			current = i + 1;
			div_f = 2;							//div_f = the new divisor for the current terms//
			while(((div_f*div_f) <= current) && (counter < K))	//checking next numbers like before//
			{
				if(current % (div_f*div_f) == 0)
				{
					counter ++;
					current ++;
					div_f = 2;
				}
				else
				{
					div_f ++;
				}
			}
		}
		if(counter == K)            //found K concurrent//
		{
			j = 0;
			while(j<K)
			{	printf("\n");
				printf("%d = ", i + j) ;		//print first term i + (j=0) = i and so on//
				rem = i + j;					//to analyze the number , rem = current remainder starting at number itself//
				div = 2;
				if(rem % (div*div) == 0)
				{
					rem /= (div*div) ;					//divide with the corresponding divisors until mod gives 0 then check next //
					printf("%d x %d ", div, div) ;
					while(rem % div == 0)
					{
						printf("x %d ", div) ;
						rem /= div ;
					}
					for(next = (div + 1) ; ((next*next) <= i) ; next ++)     //do same for next divisor//
					{
						while(rem % next == 0)
						{
							printf("x %d ", next) ;
							rem /= next ;
						}			
					}
					if (rem != 1)						//no point printing 1//
					{
						printf("x %d\n", rem) ;
					}
					j ++;							//increment j to analyze next term//
				}
				else
				{
					while((div*div) <= rem)			//if the i+j term wasn't a mod(2*2) =0 check 3*3...// 
					{								//analyze and print//
						if(rem % (div*div) == 0)
						{
							rem /= (div*div) ;
							printf("%d x %d ", div, div) ;
							while(rem % div == 0)
							{
								printf("x %d ", div) ;
								rem /= div ;
							}
							for(next = (div + 1) ; ((next*next) <= i) ; next ++)
							{
								while(rem % next == 0)
								{
									printf("x %d ", next) ;
									rem /= next ;
								}			
							}
							if (rem != 1)
							{
								printf("x %d\n", rem) ;
							}
							j ++ ;
						}
						div ++ ;
					}
				}
			}
			i = MAXNUM + 1;				//prototype exit :P so let it be//
		}
		else
		{
			i++;						// next term to check//
		}	
	}
	return 0;							//all_good > au revoir//
}

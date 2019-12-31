/*************************************************************************
Implementation File	: B1_0.cpp
Author Date			: V. Mavrommatis 17/12/2018
Purpose				: Simulates the operation of a building
Revision			: < Vasileios_Mavrommatis - 18/12/2018 >
**************************************************************************/
#include <iostream>
#include <cstdlib>
#include <string>
#include "Building2.h"
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{
	if(argc == 8)
	{
		int N = atoi(argv[1]);
		int Nf = atoi(argv[2]);
		int Ng = atoi(argv[3]);
		int No = atoi(argv[4]);
		int Nl = atoi(argv[5]);
		int K = atoi(argv[6]);
		int L = atoi(argv[7]);
		//initial argument reality checks
		if(N < 0 || Nf < 0 || Ng < 0 || No < 0 || Nl < 0 || K < 0 || L < 0)
		{
			cout << "Please give >= 0 integers\n";
			return 1;
		}
		else if(Nf >= N/3)
		{
			cout << "Floor capacity must be < (building capacity) / 3\n";
			return 1;
		}
		else if(Ng >= N/2)
		{
			cout << "Ground floor capacity must be < (building capacity /2)\n";
			return 1;
		}
		else if(No >= Nf)
		{
			cout << "Office capacity must be < floor capacity\n";
			return 1;
		}
		else if(Nl <= No)
		{
			cout << "Elevator capacity must be > office capacity\n";
			return 1;
		}
		/* SIMULATION STARTS */
		time_t t;//time variable used as seed
		srand(time(&t));//initialise rand
		int rand_floor = 0;//for random floor generator
		int rand_office = 0;/////////// office/////////
		int left;//left outside
		int entered = 0;//entered building
		int pos = 0;//current citizen to enter
		int g_entered = 0;//entered ground
		int between;//stuck between inside building and ground floor
		int flag = 0;//signals in the main loop
		//create a new building
		Building new_building(0, N, Nf, Ng, No, Nl);
		//create the K citizens with random office and floor destinations
		Citizen **Angry_N_Waiting = new Citizen*[K];
		for(int i = 0; i < K; i++)
		{
			rand_floor = rand() % (4 + 1 - 1) + 1;//floors 1~4 generator
			rand_office = rand() % (10 + 1 - 1) + 1;//offices 1~10 generator
			Angry_N_Waiting[i] = new Citizen(rand_floor, rand_office, 0);
		}
		//create a list of done citizens
		int *Done_N_Leaving = new int[Nl];
		for(int i = 0; i < Nl; i++)
			Done_N_Leaving[i] = -1;
		cout << "\n";
		//Try enter the building
		for(int i = 0; i < K; i++)
			entered += new_building.enter();
		cout << "\n";
		//get number of citizens who got to enter the building
		for(int i = 0; i < entered; i++)
			g_entered += new_building.g_enter(Angry_N_Waiting[i]);
		between = entered - g_entered;//count stuck
		cout << "\n";
		left = K - entered;//left outside
		pos = g_entered;//next from outside to enter
		//operate the elevator for L cycles
		for(int i = 0; i < L; i++)
		{	
			g_entered = 0;
			new_building.operate(Angry_N_Waiting, Done_N_Leaving);//work with current
			new_building.exit(Done_N_Leaving);//exit done citizens
			if(left > 0)//people outside building
			{
				flag = 1;
				entered = 0;//reset to count new
				for(int j = 0; j < left; j++)
					entered += new_building.enter();//count entering building
				left -= entered;//update left outside
			}
			if(flag == 0)//no more waiting outside
			{
				for(int j = 0; j < between; j++)
				{
					if(new_building.g_enter(Angry_N_Waiting[pos]))//entered ground?
					{
						g_entered++;
						pos++;
					}
				}
			}
			else//have to add the new entered rest of them
			{
				for(int j = 0; j < (between + entered); j++)
				{
					if(new_building.g_enter(Angry_N_Waiting[pos]))
					{
						g_entered++;
						pos++;
					}
				}
			}
			cout << "\n";
			between-= g_entered;//recalibrate
			if(flag == 1)//correct number
				between+=entered;//add the new
			if(left <= 0)//dont get in upper if again
				flag = 0;
		}
		//kill the citizens
		for(int i = 0; i < K; i++)
        	delete Angry_N_Waiting[i];
    	delete[] Angry_N_Waiting;
    	delete[] Done_N_Leaving;
	}
	else//invalid arguments
	{
		cout << "Please give valid >=0 integers for N, Nf, Ng, No, Nl, K, L!\n";
		return 1;
	}
	/* SIMULATION ENDS */
	return 0;
}


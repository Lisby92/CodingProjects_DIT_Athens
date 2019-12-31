/*************************************************************************
Implementation File	: sim_SD.cpp
Author Date			: V. Mavrommatis 1/12/2018
Purpose				: Simulates a Student Day between parents and teachers
Revision			: < Vasileios_Mavrommatis - 4/12/2018 >
**************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Student_Day.h"
#include <time.h>
/*NOTE
//#define bad_mood (0~1)//could be more user-defined (yet again pseudorandom) instead of mood_roulette()
//fload = rand; rand > bad_mood ? return mood //and play this way with possibilities
END*/
using namespace std;
//simulates the student day
int main(int argc, char **argv)
{	
	int N = -1;//teachers
	int K = -1;//parents
	int L = -1;//early parents
	int R = -1;//remaining parents to come
	int rand_id;//random parent id
	int rand_tea;//random number of teachers a parent will meet
	Parent **Parents;//an array holding pointers to class Parent objects
	Teacher **Teachers;////////////////////////////////// Teacher objects
	/*SIMULATION STARTING*/
	srand(time(NULL));//used as seed in rand generators
	cout << "Please give N number of teachers!\n";
	cin >> N;
	cout << "Please give K number of parents!\n";
	cin >> K;
	if(N < 1 || K < 1)
	{
		cout << "Error: Please give valid (positive integer) number of N teachers and K parents!\n";
		return 1;
	}
	Classroom new_classroom(K, N);//create a classroom that can host N teachers and a queue of K parents to meet the teachers
	Teachers = new Teacher*[N];//create the N teachers
	for(int i = 0; i < N; i++)//construct the teachers
		Teachers[i] = new Teacher(i+1, 1);//initialised as available with ids (1~N)
	Parents = new Parent*[K];//create the K parents
	for(int i = 0; i < K; i++)
	{
		rand_id = rand() % (999999 + 1 - 100000) + 100000;//6-digit parent id generator
		Parents[i] = new Parent(rand_id, N, 0);//construct parent
		rand_tea = rand() % (N + 1 - 1) + 1;//teachers to meet generator(1~N)
		Parents[i]->set_list(rand_tea);//assign him a random list of teachers
	}
	L = rand() % (K + 1 - 1) + 1;//early parents generator(1~K)
	new_classroom.early_enter(L);//enque the first L(<= K) of them
	cout << L << " early parents entered!\n";
	R = K - L;//calculate available parents
	cout << R << " parents available before start!\n\n";
	new_classroom.set_available(R);//update available parents
	new_classroom.start(Teachers, Parents);//start the Student_Day
	//free memory allocated
	for(int i = 0; i < K; i++)
		delete Parents[i];
	delete[] Parents;
	for(int i = 0; i < N; i++)
		delete Teachers[i];
	delete[] Teachers;
	
	/*SIMULATION ENDING*/
	return 0;
}
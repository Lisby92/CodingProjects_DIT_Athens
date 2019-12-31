/***************************************************************************
Implementation File : EcoSim.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates an ecosystem with a random map, plants and animals
                that can breed grow feed and die, during 4 different seasons
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include "Map.h"

using namespace std;

int main(int argc, char** argv)
{
  int mapWidth;
  string startingEpoch;
  int totalDays;
  time_t t;
  srand(time(&t));
  if(argc == 4)//arguments are ok!
  {
    mapWidth = atoi(argv[1]);
    if(mapWidth < 10)
    {
      cout << "Error: Please give map size >= 10 tiles!\n";
      return 1;
    }
    startingEpoch.assign(argv[2]);
    if((strcmp(argv[2], "winter") != 0) && (strcmp(argv[2], "summer") != 0) &&
       (strcmp(argv[2], "autumn") != 0) && (strcmp(argv[2], "spring") != 0))
    {
      cout << "Error: Invalid starting epoch!\n";
      return 1;
    }
    totalDays = atoi(argv[3]);
    if(totalDays < 1)
    {
      cout << "Error: Invalid total simulation days!\n";
      return 1;
    }
  }
  else
  {
    cout << "Error: Invalid arguments!\n";
    return 1;
  }
  //start of simulation
  Map ecoSys(mapWidth, startingEpoch);//create the (mapWidth x mapWidth)tiles 2D map grid
  ecoSys.generateMap();//generate the map
  ecoSys.placePlants();//generate and place the starting plants
  ecoSys.placeAnimals();//generate and place the starting animals
  ecoSys.printSystem();//print terrain
  cout << endl;
  ecoSys.printPlantStatistics();//print plant stats
  cout << endl;
  ecoSys.printAnimalStatistics();//print animal stats
  cout << endl;
  ecoSys.runEcosystem(totalDays);//run the ecosystem
  ecoSys.printSystem();//print terrain
  cout << endl;
  ecoSys.printPlantStatistics();//print plant stats
  cout << endl;
  ecoSys.printAnimalStatistics();//print animal stats
  cout << endl;
  //end of simulation
  return 0;
}
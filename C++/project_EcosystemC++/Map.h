/*************************************************************************
Implementation File : Map.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of Map.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
**************************************************************************/
#ifndef __Map__
#define __Map__
#include "Tile.h"
#include "Date.h"
class Map
{
  private:
    int terrainSize;//number of tiles on each side
    int growthPeriodP;//plant growth rate
    int growthPeriodA;//animals growth rate
    int breedingRepPeriodP;//plants breed rate
    int breedingRepPeriodH;//herbivores breed rate
    int breedingRepPeriodC;//carnivores breed rate
    Tile *** terrain;//terrainSize x terrainSize 2D grid
    Date * dayOfYear;//current timeframe

  public:
    Map(int, string);//constructor
    ~Map();//destructor
    void generateMap();//create the map
    void generateRiver(const int &, const int &);//create the river
    void generateLake(const int &, const int &, const int &);//create a square lake
    void generateHills(const int &, int &);//create hills of random size 2-4
    void generateMeadow();//create the meadows
    void printSystem() const;//print terrain map along with plant tokens, if any exist
    int countWater();//count water tiles
    int countMeadow();//count meadow tiles
    int countHills();//count hill tiles
    void printPlantStatistics() const;//print total number of plants
    void placePlants();//sprout the starting plants
    void placeAnimals();//spawn the starting animals
    void applySeason(string);//apply current season at day1 and every 90 days after it, with unique effects
    void runEcosystem(const int &);//ecosystem turns alive , animals feed grow and breed, and plants grow and breed
    void dailyReset(const int &);//interface to plant and animal growing, and hunger resetting of animals
    bool animalMovement(const int &, const int &, int &, int &, string);//animal that failed to find food on current tile moves
    int animalEating(string, const int &, const int &, const int &, const int &, const int &,
                     const bool &, const int &, const int &);//animal eats the found food
    void checkDeadEntities();//remove dead animals and plants from map
    void animalBreeding(const int &);//animals breed
    void plantBreeding(const int &);//plants breed
    void printAnimalStatistics() const;//print total number of animal kinds
};

#endif
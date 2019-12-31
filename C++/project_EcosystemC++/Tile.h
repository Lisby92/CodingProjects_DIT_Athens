/*************************************************************************
Implementation File : Tile.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of Tile.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
**************************************************************************/
#ifndef __Tile__
#define __Tile__
#include <string>
#include "Plant.h"
#include "Animal.h"
#include "List.h"
using namespace std;

class Tile
{
  private:
    string token;
    Seeded * nativePlantA;//the seeded plant of the tile
    Seedless * nativePlantB;//the seedless plant // only one will be used at any time
    List * nativeAnimals;//a list of resident animals
    int animalCount;//number of animals currently on tile

  public:
    Tile();//constructor
    ~Tile();//destructor
    void setToken(string);//set tile token on placing
    int getCount() const;//get current list residing animals
    void setCount(const int &);//update current list residing animals
    void printType() const;//print tile token or plant token
    string getToken() const;//get terrain token
    string getPlantToken() const;//get plant token if any
    int getSize() const;//get plant size
    bool getPlant() const;//see if plant has ever sprouted here
    void sprout(string, string, const int &, const int &, const double &, const double &,
               const int &, const int &, const int &, const int &, const int &, const bool &);//sprout a new plant
    void spawnH(string, string, const int &, const int &, const int &, const int &, const int &, 
                const int &, const int &, const bool &, const bool &, const bool &, const bool &,
                const bool &, const bool &, const bool &, const int &);//spawn a new herbivore
    void spawnC(string, string, const int &, const int &, const int &, const int &, const int &, 
                const int &, const int &, const bool &, const bool &, const bool &, const bool &,
                const bool &, const bool &, const int &, const int &, const bool &);//spawn a new carnivore
    void printNativePlant() const;//called by printSystem to print plant token
    void printAnimalStats() const;//prints animal statistics // unused
    void printPlantStats() const;//prints plant statistics //unused
    bool isAlive() const;//check if plant is alive, if it exists
    void reset();//reset animal list hunger
    void growPlant();//plant grows or gets sick and stats increase or decrease
    void raiseAnimal();//animal grows after set number of days based on epoch
    void removePlant();//removes dead plants from map
    void removeAnimal();//removes dead animals from map
    int eatPlant(const int &, const int &, const bool &);//herbivore eats the plant
    int eatAnimal(const int &);//carnivore eats another animal
    void updateMove(const int &, const int &, const int &);//animals cords updated after movement
    void updateEat(const bool &, const int &, const int &);//animals food variables updated after meal
    void getAnimals(int *, int *, int *, string *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);//get needed animals and plants to run the simulation
    void updateHungry();//update hunger count , set carnivore hunter the 8th day, or kill the underfed animal after 10 days
    bool hasFoliage() const;//check if plant has foliage left
    bool hasSeeds() const;//check if plant has seeds left
    int getVictims(const int &, const int &, string, const bool &, const int &, const int &, const int &);//get carnivore prey if on same tile cords
    bool breedPlant(string, const int &, const int &);//plant breeds
    void breedAnimal(string);//animal breeds
    void setHibernation();//set hibernating animals to sleep during winter
    void setWakedUp();//wake up sleeping hibernating animals during spring
    double getBreedProb() const;//get plant breedding probality
    void countAnimals(int &, int &, int &, int &, int &, int &, int &) const;//count total animals
    void getPlantStats(int &, int &, int &, int &, int &) const;//count total plants
};

#endif
/**************************************************************
Implementation File : Plant.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of Plant.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
***************************************************************/
#ifndef __Plant__
#define __Plant__
#include <string>
using namespace std;
//simulates a plant
class Plant
{
  private:
    string name;//name of plant
    string token;//map display token
    int x;//plant cord horizontal
    int y;//plant cord vertical
    double breedingProb;//probability to breed
    double illnessProb;//probability of illness
    int life;//lifeforce counter, 0 == death
    int lifeFactor;// growth == (life +lifeFactor), illness == (life - lifeFactor)
    bool status;//true = alive, false = dead

  public:
    Plant(string, string, int, int, double, double, int, int, bool);//constructor
    ~Plant();//destructor
    void loseLife(const int &);//plant loses life after illness or eating
    void gainLife();//plant gains life after growing 
    int getHP() const;//get plant hp left
    void setHP(const int &);//update plant current hp
    string getName() const;//get plant name
    int getX() const;//get horizontal cord
    int getY() const;//get vertical cord
    double getBreed() const;//get breeding probality
    int getFactor() const;//get life factor
    string getToken() const;//get token
    void setDead();//sets the plant dead after multiple illnesses or total eating
    bool getStatus() const;//check if plant is alive
    int grow();//grow the plant
    double getIllProb() const;//get illness probability
    void setFactor(const int &);//set plant's lifefactor
    void setSick(const double &);//set plant's sick prob
    void setBreed(const double &);//set plant's breed prob
    void setCords(const int &, const int &);//set plants cords
    void setStatus(const bool &);//set plant's status(alive or dead)
    void setName(string);//set plant's name
    void setToken(string);//set plant's token
};
//Cat.A) plant that carries seeds
class Seeded : public Plant
{
  private:
    int seeds;//fruits or seeds the tree carries
    int foliage;//amount of leaves
    int size;//size of tree

  public:
    Seeded(string, string, int, int, double, double, int, int, int, int, int, bool);//constructor
    ~Seeded();//destructor
    int getEaten(const int &);//herbivore animal eats the seeded plant
    int getClimbEaten(const int &);//herbivore climber animal eats both seeds and leaves
    int getSeedsEaten(const int &);//herbivore climber eats just the seeds
    void expand();//plant expands and grows
    int shrink();//plant gets ills and shrinks
    void printStats() const;//print plant stats
    int getSize() const;//get plant's size
    bool hasFoliage() const;//check if plant has leaves left
    bool hasSeeds() const;//check if plant has seeds left
    void setFoliage(const int &);//update leaves
    void setSeeds(const int &);//update seeds
    void setSize(const int &);//update size
    void replace(string, string, const int &, const int &, const double &, const double &,
               const int &, const int &, const int &, const int &, const int &, const bool &);
    //replace a dead seeded plant with a new seedling after successful plant breed
};
//Cat.B) plant without seeds
class Seedless : public Plant
{
  private:

  public:
    Seedless(string, string, int, int, double, double, int, int, bool);//constructor
    ~Seedless();//destructor
    int getEaten(const int &);//herbivore eats the seedless plant
    void printStats() const;//print plant stats
    void replace(string, string, const int &, const int &, const double &, const double &,
                const int &, const int &, const bool &);
    //replace a dead seedless plant with a new seedling after successful plant breed
};

#endif
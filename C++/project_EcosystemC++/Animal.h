/***************************************************************
Implementation File : Animal.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of Animal.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************/
#ifndef __Animal__
#define __Animal__

#include <string>
using namespace std;

class Animal
{
  private:
    string name;//animal's name
    string token;//animal's token
    int size;//its size
    int speed;//it's speed
    int hungerCount;//days left starving
    int eatenFood;//food eaten so far
    int eatCount;//damage per hour or meal needed
    int x;//horizontal cord
    int y;//vertical cord
    bool isAlive;//alive or dead
    bool isHungry;//hungry or filled
    bool inHeat;//can breed or no
    bool isAdult;//adult or young
    bool hibernates;//hibernates during winter or no
    bool inHibernation;//currently sleeping or awake
  public:
    Animal(string, string, int, int, int, int, int, int, int,
            bool, bool, bool, bool, bool, bool);//constructor
    ~Animal();//destructor
    bool getAlive() const;//check life signs
    string getName() const;//get animal name
    string getToken() const;//get the token
    void resetHunger();//reset animal's hunger
    int getHungerCount() const;//check days starving
    bool getSleeping() const;//check if animal sleeps (during hbernation winter)
    int getSpeed() const;//get animal's speed
    int getX() const;//get vertical cord
    int getY() const;//get horizontal cord
    int getSize() const;//get size
    int getEaten() const;//get eaten food so far
    void setEaten(const int &);//update eaten food
    int getPerMeal() const;//get eatCount
    bool getHungry() const;//check if hungry
    void move(const int &, const int &);//animal rellocates
    void eat(const bool &, const int &);//animal eats a meal or plant
    int raise();//animal grows
    void getKilled();//animal got killed by hunter or hunger
    void setHungerCount(const int &);//update days hungry
    void setHungry(const bool &);//set animal to hungry or filled
    void setSleeping(const bool &);//set animal sleeping or wake it up
    bool getHibernate() const;//check if animal hibernates during winter
    bool getHeat() const;//check if animal can breed
};
// Deer, Rabbit, Groundhog, Salmon
class Herbivore : public Animal
{
  private:
    bool canClimb;//can climb seeded trees to eat the seeds
    int neededFood;//daily needs

  public:
    Herbivore(string, string, int, int, int, int, int, int, int,
            bool, bool, bool, bool, bool, bool, bool, int);//constructor
    void growH();//herbivore grows
    ~Herbivore();//destructor
    int getClimb() const;//check if animal can climb
    int getNeeds() const;//get daily needs
};
// Fox, Bear, Wolf
class Carnivore : public Animal
{
  private:
    int attack;//hunter's attack used when attacking same size prey
    int defence;//victim's defense used when being hunted by same size carnivore hunter
    bool killer;//carnivore so hungry it kills other carnivores to feed too

  public:
    Carnivore(string, string, int, int, int, int, int, int, int,
            bool, bool, bool, bool, bool, bool, int, int, bool);//constructor
    ~Carnivore();//destructor
    void growC();//carnivore grows
    void setKiller(const bool &);//update killer status
    bool getKiller() const;//check if carnivore hunt's carnivores too
    int getDefence() const;//get defense stat
    int getAttack() const;//get attack stat
};

#endif
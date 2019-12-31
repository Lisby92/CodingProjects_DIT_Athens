/******************************************************
Implementation File : Animal.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates the animals of the ecosystem
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
******************************************************/
#include <cstdlib>
#include <iostream>

#include "Animal.h"

using namespace std;
/* ANIMAL FUNCTIONS */
Animal::Animal(string nam = "Monkey", string key = "M", int siz = 2, int spd = 10, int hunger = 10,
               int eaten = 0, int perHour = 3, int crd1 = 10, int crd2 = 30, bool alive = true, bool hungry = true,
               bool heated = false, bool adult = true, bool hibern = false, bool sleeping = false)
: name(nam), token(key), size(siz), speed(spd), hungerCount(hunger), eatenFood(eaten), eatCount(perHour), x(crd1),
  y(crd2), isAlive(alive), isHungry(hungry), inHeat(heated), isAdult(adult),
  hibernates(hibern), inHibernation(sleeping) 
{
  /*
  cout << "Animal created successfully with the follwing attributes\n";
  cout << "Name : " << name << endl;
  cout << "Token : " << token << endl;
  cout << "Size : " << size << endl;
  cout << "Speed : " << speed << endl;
  cout << "Hunger Count : " << hungerCount << endl;
  cout << "Eaten Food : " << eatenFood << endl;
  cout << "Food per hour : " << eatCount << endl;
  cout << "Cords : (" << x << ", " << y << ")\n";
  cout << "Alive : " << isAlive << endl;
  cout << "Hungry : " << isHungry << endl;
  cout << "Heat : " << inHeat << endl;
  cout << "Adult : " << isAdult << endl;
  cout << "Hibernates : " << hibernates << endl;
  cout << "Currently hibernating : " << inHibernation << endl;
  */
}

Animal::~Animal()
{
  //cout << "Destroying animal...\n";
}

bool Animal::getAlive() const
{
  if(isAlive == true)
    return true;
  else//animal is dead
    return false;
}

string Animal::getName() const
{
  return name;
}

string Animal::getToken() const
{
  return token;
}

void Animal::resetHunger()
{
  if(inHibernation == false)//hunger resets only when not hibernating, if animal does anyway
  {
    isHungry = true;
    eatenFood = 0;
  }
}

int Animal::getHungerCount() const
{
  return hungerCount;
}

bool Animal::getSleeping() const
{
  return inHibernation;
}

bool Animal::getHeat() const
{
  return inHeat;
}

void Animal::setSleeping(const bool & newStatus)
{
  inHibernation = newStatus;
}

bool Animal::getHibernate() const
{
  return hibernates;
}

int Animal::getSpeed() const
{
  return speed;
}

int Animal::getX() const
{
  return x;
}

int Animal::getY() const
{
  return y;
}

bool Animal::getHungry() const
{
  return isHungry;
}

int Animal::getSize() const
{
  return size;
}
int Animal::getEaten() const
{
  return eatenFood;
}
int Animal::getPerMeal() const
{
  return eatCount;
}

void Animal::setHungry(const bool & newHungry)
{
  isHungry = newHungry;
}

void Animal::move(const int & newX, const int & newY)
{
  x = newX;
  y = newY;
}

void Animal::eat(const bool & newIsHungry, const int & newEatenFood)
{
  isHungry = newIsHungry;
  eatenFood += newEatenFood;
}

void Animal::setEaten(const int & neweat)
{
  eatenFood = neweat;
}

void Animal::getKilled()
{
  isAlive = false;
}

void Animal::setHungerCount(const int & newHc)
{
  hungerCount = newHc;
}

int Animal::raise()
{
  string key = getName();
  string temp1 = "Salmon";
  string temp2 = "Rabbit";
  string temp3 = "Groundhog";
  string temp4 = "Deer";
  string temp5 = "Fox";
  string temp6 = "Wolf";
  string temp7 = "Bear";
  bool sleeping = getSleeping();
  if(sleeping == true)
    return -1;
  if(key.compare(temp1) == 0)//salmon
  {
    inHeat = true;
    isAdult = true;
    return -1;
  }
  if(key.compare(temp2) == 0)//rabbit
  {
    if(size < 2)
      size++;
    if(speed < 5)
      speed += 2;
    if(size == 2)
    {
      inHeat = true;
      isAdult = true;
    }
    return 1;
  }
  if(key.compare(temp3) == 0)//groundhog
  {
    if(size < 3)
      size++;
    if(speed < 4)
      speed++;
    if(size == 3)
    {
      inHeat = true;
      isAdult = true;
    }
    return 1;
  }
  if(key.compare(temp4) == 0)//deer
  {
    if(size < 5)
      size++;
    if(speed < 7)
      speed += 2;
    if(size == 5)
    {
      isAdult = true;
      inHeat = true;
    }
    return 1;
  }
  if(key.compare(temp5) == 0)//fox
  {
    if(size < 4)
      size++;
    if(speed < 6)
      speed++;
    if(eatCount < 6)
      eatCount++;
    if(size == 4)
    {
      isAdult = true;
      inHeat = true;
    }
    return 0;
  }
  if(key.compare(temp6) == 0)//wolf
  {
    if(size < 7)
      size++;
    if(speed < 7)
      speed += 2;
    if(eatCount < 7)
      eatCount += 2;
    if(size == 7)
    {
      isAdult = true;
      inHeat = true;
    }
    return 0; 
  }
  if(key.compare(temp7) == 0)//bear
  {
    if(size < 8)
      size += 2;
    else if(size == 9)
      size ++;
    if(eatCount < 8)
      eatCount += 2;
    else if(eatCount == 9)
      eatCount++;
    if(size == 10)
    {
      isAdult = true;
      inHeat = true;
    }
    return 0;
  }
  return -1;
}
/* FUNCTIONS OF HERBIVORES */
Herbivore::Herbivore(string nam = "Zebra", string key = "H", int siz = 10, int spd = 15,
                    int hunger = 20, int eaten = 0, int perHour = 3, int crd1 = 36, int crd2 = 212,
                    bool alive = true, bool hungry = true, bool heated = true,
                    bool adult = true, bool hibern = false, bool sleeping = true,
                    bool climber = false, int needs = 100)
: Animal(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2,
         alive, hungry, heated, adult, hibern, sleeping), canClimb(climber), neededFood(needs)
{
  /*
  cout << "A herbivore has been constructed with the follwing attributes\n";
  cout << "Climber : " << canClimb << endl;
  cout << "Food Needed : " << neededFood << endl;
  */
}

Herbivore::~Herbivore()
{
  //cout << "Destroying Herbivore...\n";
}

void Herbivore::growH()
{
  string key = getName();
  string temp1 = "Rabbit";
  string temp2 = "Groundhog";
  string temp3 = "Deer";
  if(key.compare(temp1) == 0)//rabbit
  {
    if(neededFood < 4)
      neededFood++;
  }
  else if(key.compare(temp2) == 0)//groundhog
  {
    if(neededFood < 5)
      neededFood++;
  }
  else if(key.compare(temp3) == 0)//deer
  {
    if(neededFood < 7)
      neededFood += 2;
  }
}

int Herbivore::getClimb() const
{
  return (int)canClimb;
}
int Herbivore::getNeeds() const
{
  return neededFood;
}

/* FUNCTIONS OF CARNIVORES */
Carnivore::Carnivore(string nam = "Tiger", string key = "C", int siz = 20, int spd = 20,
                    int hunger = 30, int eaten = 0, int perHour = 10000, int crd1 = 0, int crd2 = 4,
                    bool alive = true, bool hungry = true, bool heated = false,
                    bool adult = false, bool hibern = false, bool sleeping = false,
                    int atk = 30, int def = 20, bool mood = false)
: Animal(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2,
         alive, hungry, heated, adult, hibern, sleeping), attack(atk), defence(def), killer(mood)
{
  /*
  cout << "A carnivore has been constructed with the follwing attributes\n";
  cout << "Attack : " << attack << endl;
  cout << "Defence : " << defence << endl;
  */
}

Carnivore::~Carnivore()
{
  //cout << "Destroying Carnivore...\n";
}

void Carnivore::growC()
{
  string key = getName();
  string temp1 = "Fox";
  string temp2 = "Wolf";
  string temp3 = "Bear";
  if(key.compare(temp1) == 0)//fox
  {
    if(attack < 5)
      attack++;
    if(defence < 5)
      defence++;
  }
  else if(key.compare(temp2) == 0)//wolf
  {
    if(attack < 7)
      attack += 2;
    if(defence < 5)
      defence += 2;
  }
  else if(key.compare(temp3) == 0)//bear
  {
    if(attack < 8)
      attack += 2;
    if(defence < 8)
      defence += 2;
  }
}

void Carnivore::setKiller(const bool & mood)
{
  killer = mood;
}

bool Carnivore::getKiller() const
{
  return killer;
}

int Carnivore::getDefence() const
{
  return defence;
}

int Carnivore::getAttack() const
{
  return attack;
}
/***************************************************************************
Implementation File : Plant.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates the plants of the ecosystem
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************************/
#include "Plant.h"
#include <iostream>
#include <cstdlib>
using namespace std;
/* PLANT FUNCTIONS */
Plant::Plant(string title = "Lemontree", string key = "L", int cord1 = 15, int cord2 = 219,
             double breed = 0.1, double ill = 0.5, int hp = 100, int factor = 4, bool state = true)
: name(title), token(key), x(cord1), y(cord2), breedingProb(breed),
  illnessProb(ill), life(hp), lifeFactor(factor), status(state)
{ 
  /*
  cout << "A new plant with the following attributes has been created!\n";
  cout << "Name : " << name << endl;
  cout << "Token : " << token << endl;
  cout << "Cords : (" << x << ", " << y << ")\n";
  cout << "Probability to breed : " << breedingProb << endl;
  cout << "Probability to get sick : " << illnessProb << endl;
  cout << "HP : " << life << endl;
  cout << "Life force : " << lifeFactor << endl;
  cout << "State : " << status << endl;
  */
}

Plant::~Plant()
{
  //cout << name << " destroyed...\n";
}

void Plant::loseLife(const int & damage)
{
  if(life != 0)
  {
    if(life <= damage)
    {
      life = 0;//dead
      setDead();//plant paradise inc
    }
    else
      life -= damage;
  }
}

void Plant::gainLife()
{
  life += lifeFactor;
}

int Plant::getHP() const
{
  return life;
}

int Plant::getFactor() const
{
  return lifeFactor;
}

void Plant::setHP(const int & newHP)
{
  life = newHP;
}

string Plant::getToken() const
{
  return token;
}

double Plant::getIllProb() const
{
  return illnessProb;
}

string Plant::getName() const
{
  return name;
}

int Plant::getX() const
{
  return x;
}

int Plant::getY() const
{
  return y;
}

double Plant::getBreed() const
{
  return breedingProb;
}

bool Plant::getStatus() const
{
  return status;
}

void Plant::setDead()
{
  status = false;
}

void Plant::setFactor(const int & newFactor)
{
  lifeFactor = newFactor;
}

void Plant::setSick(const double & newProb)
{
  illnessProb = newProb;
}

void Plant::setBreed(const double & newProb)
{
  breedingProb = newProb;
}

void Plant::setCords(const int & newCord1, const int & newCord2)
{
  x = newCord1;
  y = newCord2;
}

void Plant::setStatus(const bool & newState)
{
  status = newState;
}

void Plant::setName(string newName)
{
  name = newName;
}

void Plant::setToken(string newToken)
{
  token = newToken;
}

int Plant::grow()
{
  string key = getToken();
  string temp1 = "G";
  string temp2 = "A";
  double probIll = getIllProb();
  double chance = ((double) rand() / (RAND_MAX));
  if((key.compare(temp1) == 0) || (key.compare(temp2) == 0))//seedless
  {
    if(chance > probIll)//plant will stay healthy and grow
      gainLife();
    else//plant got sick and will lose life
      loseLife(lifeFactor);
    return 0;//dont care
  }
  else//seeded
  {
    if(chance > probIll)//plant will stay healthy and grow with lifeFactor steps
    {
      gainLife();//1 for foliage
      gainLife();//1 for seeds
      gainLife();//1 for seeds
      return 1;//seeded grew
    }
    else//plant got sick and will lose life with lifeFactor steps
      return 2;//seeded shrank
  }
}
/* SEEDED FUNCTIONS */
Seeded::Seeded(string title = "Lemontree", string key = "L", int cord1 = 15,
              int cord2 = 219, double breed = 0.1, double ill = 0.5, int hp = 100,
              int factor = 4, int food = 30, int leaves = 100, int sz = 5, bool state = true)
: Plant(title, key, cord1, cord2, breed, ill, hp, factor, state),
  seeds(food), foliage(leaves), size(sz)
{
  /*
  cout << "A new seeded plant with the following attributes has been created!\n";
  cout << "Seeds : " << seeds << endl;
  cout << "Foliage : " << foliage << endl;
  cout << "Size : " << size << endl;
  */
}

Seeded::~Seeded()
{
  //cout << "Seeded plant destroyed...\n";
}

int Seeded::getClimbEaten(const int & damage)
{
  int eatCount = 0;
  if((foliage + seeds) <= damage)//plant will be totally eaten
  {
    eatCount = (foliage + seeds);
    seeds = 0;
    foliage = 0;
  }
  else//plant will survive
  {
    if(seeds > 0)//plant got seeds still
    {
      if(seeds > damage)//some seeds will survive
      {
        eatCount += damage;
        seeds -= damage;
      }
      else//all the seeds will be eaten
      {
        eatCount += seeds;
        seeds = 0;
        if(eatCount < damage)//animal still hungry
        {
          int eatMore = damage - eatCount;
          if(foliage > eatMore)//some leaves will survive
          {
            foliage -= eatMore;
            eatCount += eatMore;
          }
          else//plant is totally eaten
          {
            eatCount += foliage;
            foliage = 0;
          }
        }
      }
    }
    else//all the seeds are eaten
    {
      if(foliage > damage)//some leaves will survive
      {
        foliage -= damage;
        eatCount += damage;
      }
      else//plant is totally eaten
      {
        eatCount += foliage;
        foliage = 0;
      }
    }
  }
  loseLife(eatCount);
  return eatCount;
}

int Seeded::getEaten(const int & damage)
{
  int eatCount = 0;
  if(foliage == 0)//just seeds left but u cant get them :P
    return eatCount;
  if(foliage <= damage)//no leaves will survive
  {
    eatCount += foliage;
    foliage = 0;
  }
  else//some leaves will survive
  {
    eatCount += damage;
    foliage -= damage;
  }
  loseLife(eatCount);
  return eatCount;
}

int Seeded::getSeedsEaten(const int & damage)
{
  int eatCount = 0;
  if(seeds == 0)//just foliage left but you cant eat it :P
    return eatCount;
  if(seeds <= damage)//no leaves will survive
  {
    eatCount += seeds;
    seeds = 0;
  }
  else//some leaves will survive
  {
    eatCount += damage;
    seeds -= damage;
  }
  loseLife(eatCount);
  return eatCount;
}

void Seeded::expand()
{
  int factor = getFactor();
  foliage += factor;
  seeds += (2 * factor);
  if(foliage >= factor)
    size++;
}

int Seeded::shrink()
{
  int lostLife = 0;
  int factor = getFactor();
  if(foliage > factor)
  {
    foliage -= factor;
    lostLife += factor;
  }
  else
  {
    lostLife += foliage;
    foliage = 0;
  }
  if(seeds > (2 * factor))
  {
    seeds -= (2 * factor);
    lostLife += (2 * factor);
  }
  else
  {
    lostLife += seeds;
    seeds = 0;
  }
  if(foliage <= factor)
    if(size > 0)//can't have negative size
      size--;  
  return lostLife;  
}

void Seeded::printStats() const
{
  cout << "Name : " << getName() << endl;
  cout << "Token : " << getToken() << endl;
  cout << "Cords : (" << getX() << ", " << getY() << ")\n";
  cout << "Probability to breed : " << getBreed() << endl;
  cout << "Probability to get sick : " << getIllProb() << endl;
  cout << "HP : " << getHP() << endl;
  cout << "Life force : " << getFactor() << endl;
  cout << "Seeds : " << seeds << endl;
  cout << "Foliage : " << foliage << endl;
  cout << "Size : " << size << endl;
  cout << "Status : " << getStatus() << endl;
}

int Seeded::getSize() const
{
  return size;
}

bool Seeded::hasFoliage() const
{
  if(foliage < 1)
    return false;
  else
    return true;
}

bool Seeded::hasSeeds() const
{
  if(seeds < 1)
    return false;
  else
    return true;
}

void Seeded::setFoliage(const int & newFoliage)
{
  foliage = newFoliage;
}

void Seeded::setSeeds(const int & newSeeds)
{
  seeds = newSeeds;
}

void Seeded::setSize(const int & newSize)
{
  size = newSize;
}

void Seeded::replace(string title, string key, const int & cord1,
const int & cord2, const double & breed, const double & ill, const int & hp,
const int & factor, const int & fruit, const int & leaves, const int & sz, const bool & state)
{
  setFoliage(leaves);
  setSeeds(fruit);
  setSize(sz);
  setName(title);
  setToken(key);
  setCords(cord1, cord2);
  setStatus(state);
  setHP(hp);
  setFactor(factor);
  setBreed(breed);
  setSick(ill);
}
/* SEEDLESS FUNCTIONS */
Seedless::Seedless(string title = "Lemontree", string key = "L", int cord1 = 15, int cord2 = 219,
                double breed = 0.1, double ill = 0.5, int hp = 1, int factor = 4, bool state = true)
: Plant(title, key, cord1, cord2, breed, ill, hp, factor, state)
{
  //cout << "A new seedless plant has been created!\n";
}

Seedless::~Seedless()
{
  //cout << "Seedless plant destroyed...\n";
}

int Seedless::getEaten(const int & damage)
{
  int eatCount = 0;
  if(getHP() > damage)//plant survived 
    eatCount = damage;
  else//plant got totally eaten
    eatCount = getHP();
  loseLife(damage);
  return eatCount;
}

void Seedless::printStats() const
{
  cout << "Name : " << getName() << endl;
  cout << "Token : " << getToken() << endl;
  cout << "Cords : (" << getX() << ", " << getY() << ")\n";
  cout << "Probability to breed : " << getBreed() << endl;
  cout << "Probability to get sick : " << getIllProb() << endl;
  cout << "HP : " << getHP() << endl;
  cout << "Life force : " << getFactor() << endl;
  cout << "Status : " << getStatus() << endl;
}

void Seedless::replace(string title, string key, const int & cord1,
const int & cord2, const double & breed, const double & ill, const int & hp,
const int & factor, const bool & state)
{
  setName(title);
  setToken(key);
  setCords(cord1, cord2);
  setStatus(state);
  setHP(hp);
  setFactor(factor);
  setBreed(breed);
  setSick(ill);
}
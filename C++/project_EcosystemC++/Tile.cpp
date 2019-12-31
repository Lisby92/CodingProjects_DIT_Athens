/***************************************************************************
Implementation File : Tile.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates a fragment area of the map
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "Tile.h"
using namespace std;
/* TILE FUNCTIONS */
Tile::Tile()
: token("\0"), nativePlantA(NULL), nativePlantB(NULL), animalCount(0)
{
  nativeAnimals = new List();
  //cout << "A new tile type " << token;
  //cout << " has been created!\n";
}

Tile::~Tile()
{
  delete nativePlantA;
  delete nativePlantB;
  delete nativeAnimals;
  //cout << "Tile destroyed!\n";
}

void Tile::setToken(string type)
{
  token.assign(type);
}

int Tile::getCount() const
{
  return animalCount;
}

void Tile::setCount(const int & newCount)
{
  animalCount = newCount;
}

string Tile::getToken() const
{
  return token;
}

bool Tile::getPlant() const
{
  if(nativePlantA != NULL || nativePlantB != NULL)
    return true;
  else
    return false;
}

int Tile::getSize() const
{
  if(nativePlantA == NULL)
    return 0;//seedless has no size
  if(nativePlantB == NULL)
    return nativePlantA->getSize();
  return 0;
}

void Tile::printType() const
{
  if(isAlive() == 0)
    cout << token;
  else
    printNativePlant();
}

string Tile::getPlantToken() const
{
  if(nativePlantB != NULL)
    if(nativePlantB->getStatus() == true)
      return nativePlantB->getToken();//seedless has no size
  if(nativePlantA != NULL)
    if(nativePlantA->getStatus() == true)
      return nativePlantA->getToken();
  return "N/A";
}


void Tile::sprout(string name, string key, const int & cord1, const int & cord2,
                 const double & breedProb, const double & illProb, const int & hp,
                 const int & factor, const int & food = 0, const int & leaves = 0,
                 const int & size = 0, const bool & state = true)
{
  if(food == 0 && leaves == 0 && size == 0)
  {
    nativePlantB = new Seedless(name, key, cord1, cord2, breedProb, illProb, hp, factor, state);
  }
  else
  {
    nativePlantA = new Seeded(name, key, cord1, cord2, breedProb,
                            illProb, hp, factor, food, leaves, size, state);
  }
}

void Tile::spawnH(string nam, string key, const int & siz, const int & spd, const int & hunger,
                 const int & eaten, const int & perHour, const int & cord1, const int & cord2, const bool & alive,
                 const bool & hungry, const bool & heated, const bool & adult, const bool & hibern,
                 const bool & sleeping, const bool & climber, const int & needs)
{
  int cur = getCount();
  nativeAnimals->addNodeH(nam, key, siz, spd, hunger, eaten, perHour, cord1, cord2, alive,
                                    hungry, heated, adult, hibern, sleeping, climber, needs);
  cur++;
  setCount(cur);
}

void Tile::spawnC(string nam, string key, const int & siz, const int & spd, const int & hunger,
                 const int & eaten, const int & perHour, const int & cord1, const int & cord2, const bool & alive,
                 const bool & hungry, const bool & heated, const bool & adult, const bool & hibern,
                 const bool & sleeping, const int & atk, const int & def, const bool & mood)
{
  int cur = getCount();
  nativeAnimals->addNodeC(nam, key, siz, spd, hunger, eaten, perHour, cord1, cord2, alive,
                                    hungry, heated, adult, hibern, sleeping, atk, def, mood);
  cur++;
  setCount(cur);
}

bool Tile::isAlive() const
{
  if(nativePlantA == NULL && nativePlantB == NULL)//plant has never sprouted here
    return false;
  if(nativePlantB != NULL)
    if(nativePlantB->getStatus() == true)
      return true;
  if(nativePlantA != NULL)
    if(nativePlantA->getStatus() == true)
      return true;
  return false;      
}

double Tile::getBreedProb() const
{
  if(isAlive() != 0)
  {
    if(nativePlantA != NULL)
      if(nativePlantA->getStatus() == true)
        return nativePlantA->getBreed();
    if(nativePlantB != NULL)
      if(nativePlantB->getStatus() == true)
        return nativePlantB->getBreed();
  }
  return -0.0;
}

void Tile::printNativePlant() const
{
  if(isAlive() != 0)
  {
    if(nativePlantA != NULL)
      if(nativePlantA->getStatus() == true)
        cout << nativePlantA->getToken();
    if(nativePlantB != NULL)
      if(nativePlantB->getStatus() == true)
        cout << nativePlantB->getToken();
  }
}

void Tile::printAnimalStats() const
{
  if(getCount() == 0)
  {
    //cout << "N/A";
  }
  else
    nativeAnimals->printList();  
}

void Tile::printPlantStats() const
{
  if(isAlive() != 0)
  {
    if((nativePlantA == NULL) && (nativePlantB != NULL))
    {
      nativePlantB->printStats();
      cout << "\n";
    }
    if((nativePlantB == NULL) && (nativePlantA != NULL))
    {
      nativePlantA->printStats();
      cout << "\n";
    }
  }
}

void Tile::reset()
{
  if(getCount() != 0)
    nativeAnimals->searchReset();
}

void Tile::growPlant()
{
  if(isAlive() != 0)
  {
    int result;
    int lostLife = 0;
    if(nativePlantB == NULL)
    {
      result = nativePlantA->grow();
      if(result == 1)//seeded grew
        nativePlantA->expand();
      else if(result == 2)//seeded shrank
        lostLife = nativePlantA->shrink();
      if(lostLife > 0)
        nativePlantA->loseLife(lostLife);
    }
    if(nativePlantA == NULL)
      result = nativePlantB->grow();
  }
}

void Tile::raiseAnimal()
{
  if(getCount() > 0)
    nativeAnimals->searchRaise();
}

void Tile::removePlant()
{
  if(nativePlantA != NULL)
  {
    if(nativePlantA->getHP() == 0)
    {
      if(nativePlantA->getStatus() == true)
      {
        nativePlantA->setDead();
        return;
      }
    }
  }
  if(nativePlantB != NULL)
  {
    if(nativePlantB->getHP() == 0)
    {
      if(nativePlantB->getStatus() == true)
      {
        nativePlantB->setDead();
        return;
      }
    }
  }
}

void Tile::removeAnimal()
{
  int curSize = getCount();
  if(curSize > 0)//there were animal spawns in this tile
  {
    int * victims = new int[curSize];
    for(int i = 0; i < curSize; i++)
      victims[i] = -1;//init
    nativeAnimals->searchDead(victims);
    for(int i = curSize-1; i >= 0; i--)
      if(victims[i] != -1)//dead animal position here
        nativeAnimals->delNode(victims[i]);//remove node from list
    delete[] victims;  
  }
}

int Tile::eatPlant(const int & damage, const int & climber, const bool & target)
{
  if(nativePlantA != NULL)//seeded
  {
    if(nativePlantA->getStatus() == true)//plant alive
    {
      if(climber == 1)
      {
        if(target == true)//climber can eat seeds and foliage
          return nativePlantA->getClimbEaten(damage);
        else//target == false : climber can eat only the seeds
          return nativePlantA->getSeedsEaten(damage);
      }
      else//non climber can eat just foliage
        return nativePlantA->getEaten(damage);
    }
  }
  if(nativePlantB != NULL)//seedless
    if(nativePlantB->getStatus() == true)//plant alive
      return nativePlantB->getEaten(damage);
  return 0;  
}

bool Tile::hasFoliage() const
{
  if(nativePlantA != NULL)
    if(nativePlantA->getStatus() == true)//plant alive
      return nativePlantA->hasFoliage();
  if(nativePlantB != NULL)
    if(nativePlantB->getStatus() == true)//plant alive
      return true;
  return false;  
}

bool Tile::hasSeeds() const
{
  if(nativePlantA != NULL)
    if(nativePlantA->getStatus() == true)//plant alive
      return nativePlantA->hasSeeds();
  if(nativePlantB != NULL)
    if(nativePlantB->getStatus() == true)//plant alive
      return true;
  return false;  
}

int Tile::eatAnimal(const int & nodePos)
{
  return nativeAnimals->searchHunted(nodePos);
}

void Tile::getAnimals(int * xList, int * yList, int * spdList, string * nameList, int * posList,
                      int * sizeList, int * atkList, int * defList, int * climbList,
                      int * needsList, int * eatenList, int * perMealList, int * hungerList, int * killerList)
{
  int curSize = getCount();
  if(curSize > 0)
  {
    nativeAnimals->getCords(xList, yList, spdList, nameList, posList, sizeList, atkList, defList, climbList, needsList, eatenList, perMealList, hungerList, killerList);
  }
}

void Tile::updateEat(const bool & filled, const int & eatenFood, const int & nodePos)//meta sto list kai meta sto idio to animal.cpp
{
  nativeAnimals->searchEatUpdate(filled, eatenFood, nodePos);
}

void Tile::updateMove(const int & cord1, const int & cord2, const int & nodePos)
{
  nativeAnimals->searchMoveUpdate(cord1, cord2, nodePos);
}

void Tile::updateHungry()
{
  nativeAnimals->searchHungry();
}

int Tile::getVictims(const int & hunterSize, const int & hunterSpd, string hunterName,
const bool & hunterMood, const int & hunterAtk, const int & cord1, const int & cord2)
{ 
  if(getCount() > 0)
    return nativeAnimals->searchVictim(hunterSize, hunterSpd, hunterName, hunterMood, hunterAtk, cord1, cord2);
  else
    return -1;//list empty
}

bool Tile::breedPlant(string name, const int & tileCord1, const int & tileCord2)
{
  string temp1 = "G";
  string temp2 = "A";
  string temp3 = "O";
  string temp4 = "M";
  string temp5 = "P";
  string temp6 = "^";
  string temp7 = "\"";
  string temp8 = "#";
  string tileToken = getToken();
  if(name.compare(temp1) == 0)//Grass
  {
    if(tileToken.compare(temp7) == 0)//meadow tile
    {
      if(nativePlantB == NULL)
      {
        sprout("Grass", "G", tileCord1, tileCord2, 0.15, 0.15, 5, 4, 0, 0, 0, true);
        return true;
      }
      else if(nativePlantB->getStatus() == false)
      {
        nativePlantB->replace("Grass", "G", tileCord1, tileCord2, 0.15, 0.15, 5, 4, true);
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  if(name.compare(temp2) == 0)//Algae
  {
    if(tileToken.compare(temp8) == 0)//water tile
    {
      if(nativePlantB == NULL)
      {
        sprout("Algae", "A", tileCord1, tileCord2, 0.25, 0.25, 5, 2, 0, 0, 0, true);
        return true;
      }
      else if(nativePlantB->getStatus() == false)
      {
        nativePlantB->replace("Algae", "A", tileCord1, tileCord2, 0.25, 0.25, 5, 2, true);
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  if(name.compare(temp3) == 0)//Oak
  {
    if(tileToken.compare(temp7) == 0)//meadow tile
    {
      if(nativePlantA == NULL)
      {
        sprout("Oak","O", tileCord1, tileCord2, 0.20, 0.20, 45, 15, 15, 30, 5, true);
        return true;
      }
      else if(nativePlantA->getStatus() == false)
      {
        nativePlantA->replace("Oak","O", tileCord1, tileCord2, 0.20, 0.20, 45, 15, 15, 30, 5, true);
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  if(name.compare(temp4) == 0)//Mapple
  {
    if((tileToken.compare(temp7) == 0) || (tileToken.compare(temp6) == 0))//meadow or hill tile
    {
      if(nativePlantA == NULL)
      {
        sprout("Mapple", "M", tileCord1, tileCord2, 0.05, 0.05, 30, 10, 10, 20, 2, true);
        return true;
      }
      else if(nativePlantA->getStatus() == false)
      {
        nativePlantA->replace("Mapple", "M", tileCord1, tileCord2, 0.05, 0.05, 30, 10, 10, 20, 2, true);
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  if(name.compare(temp5) == 0)//Pine
  {
    if((tileToken.compare(temp7) == 0) || (tileToken.compare(temp6) == 0))//meadow or hill tile
    {
      if(nativePlantA == NULL)
      {
        sprout("Pine","P", tileCord1, tileCord2, 0.15, 0.15, 60, 20, 20, 40, 5, true);
        return true;
      }
      else if(nativePlantA->getStatus() == false)
      {
        nativePlantA->replace("Pine","P", tileCord1, tileCord2, 0.15, 0.15, 60, 20, 20, 40, 5, true);
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  return false;//something went wrong
}

void Tile::breedAnimal(string token)
{
  int count = getCount();
  if(count > 0)
  {
    count += nativeAnimals->searchHeated(token);
    setCount(count);
  }
}

void Tile::setHibernation()
{
  if(getCount() > 0)
    nativeAnimals->setSleeping();
}

void Tile::setWakedUp()
{
  if(getCount() > 0)
    nativeAnimals->setAwaken();
}

void Tile::countAnimals(int & numSalmon, int & numRabbit, int & numGroundhog, int & numDeer,
                        int & numFox, int & numWolf, int & numBear) const
{
  if(getCount() > 0)
  {
    nativeAnimals->searchTotal(numSalmon, numRabbit, numGroundhog, numDeer,
    numFox, numWolf, numBear);
  }
}

void Tile::getPlantStats(int & numAlgae, int & numGrass,
                         int & numOak, int & numMapple, int & numPine) const
{
  string temp1 = "G";
  string temp2 = "A";
  string temp3 = "O";
  string temp4 = "M";
  string temp5 = "P";
  string nativePlant = getPlantToken();
  if(temp1.compare(nativePlant) == 0)//Grass
    numGrass++;
  else if(temp2.compare(nativePlant) == 0)//Algae
    numAlgae++;
  else if(temp3.compare(nativePlant) == 0)//Oak
    numOak++;
  else if(temp4.compare(nativePlant) == 0)//Mapple
    numMapple++;
  else if(temp5.compare(nativePlant) == 0)//Pine
    numPine++;
}
/***************************************************************************
Implementation File : Map.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates a terrain map with river, lake, hills and meadows
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "Map.h"
#include "Plant.h"
#include "Animal.h"
using namespace std;
/* MAP FUNCTIONS */
Map::Map(int size, string epoch)
: terrainSize(size), growthPeriodP(0), growthPeriodA(0),
  breedingRepPeriodP(0), breedingRepPeriodH(0), breedingRepPeriodC(0)
{
  //creating the empty terrain tiles
  terrain = new Tile**[terrainSize];
  for(int i = 0; i < terrainSize; i++)
  {
    terrain[i] = new Tile*[terrainSize];
    for(int j = 0; j < terrainSize; j++)
      terrain[i][j] = new Tile();
  }
  //creating the starting time setting
  dayOfYear = new Date(epoch);
  cout << "A new ecosystem has been created!\n";
}

Map::~Map()
{
  delete dayOfYear;
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
      delete terrain[i][j];
    delete[] terrain[i];
  }
  delete[] terrain;
  cout << "Ecosystem destroyed!\n";
}

void Map::generateMap()
{
  int start = rand() % ((terrainSize - 5) - 5 + 1) + 5;
  int side = rand() % (3 - 0 + 1) + 0;
  generateRiver(start, side);
  int lakeSize = rand() % (((terrainSize / 3) + 1) - (terrainSize / 4)) + (terrainSize / 3);//min == 3x3 tiles, ~= 1/10 of map
  int lakeCord1 = rand() % ((terrainSize - lakeSize + 1) - (lakeSize - 1)) + (lakeSize - 1);//starting point is fixed on cords
  int lakeCord2 = rand() % ((terrainSize - lakeSize + 1) - (lakeSize - 1)) + (lakeSize - 1);//later it expands to all possible ways
  generateLake(lakeSize, lakeCord1, lakeCord2);
  int hillsNum = (terrainSize * terrainSize / 6);// ~= 1/3 of map with randomness on tiles covered(2-4)
  int hillSize;
  cout << "Adding hills ...\n";
  for(int i = 0; i < hillsNum; i++)
  {
    hillSize = rand() % ((4 + 1) - 2) + 2;
    side = rand() % (3 - 0 + 1) + 0;//pick a growing side move from exterior to interior of map
    generateHills(hillSize, side);
  }
  cout << "Hills generated successfully!\n";
  generateMeadow();
  cout << "Meadow generated successfully!\n";
  cout << "Map generated successfully!\n\n";
}

void Map::generateRiver(const int & start, const int & side)
{
  cout << "Adding river ...\n";
  int i = 0;
  int j = 0;
  double chance = 0.0;
  int random = 0;
  int drift;
  if(side == 0)//North
  {
    i = 0;
    drift = start;
    while(i < terrainSize)//till south side is reached
    {
      terrain[i][drift]->setToken("#");
      chance = ((double) rand() / (RAND_MAX));
      if((drift == (terrainSize - 1)) || (drift == 0))//reached east or west side
      {
        i++;
        continue;
      }
      else//can turn more
      {
        if(chance < 0.3)
        {
          random = rand() % (4 - 1 + 1) + 0;
          if(random == 0)//east 1
          {
            drift ++;
            terrain[i][drift]->setToken("#");
          }
          else if(random == 1)//west 1
          {
            drift --;
            terrain[i][drift]->setToken("#");
          }
          else if(random == 2)//east 2
          {
            drift ++;
            terrain[i][drift]->setToken("#");
            if(drift < (terrainSize - 1))
            {
              drift++;
              terrain[i][drift]->setToken("#");
            }
          }
          else if(random == 3)//west 2
          {
            drift--;
            terrain[i][drift]->setToken("#");
            if(drift > 0)
            {
              drift--;
              terrain[i][drift]->setToken("#");
            }
          }
        }
        i++;
      }
    }
  }
  else if(side == 1)//West
  {
    drift = start;
    j = 0;
    while(j < terrainSize)//till east side is reached
    {
      terrain[drift][j]->setToken("#");
      chance = ((double) rand() / (RAND_MAX));
      if((drift == (terrainSize - 1)) || (drift == 0))//reached south or north side
      {
        j++;
        continue;
      }
      else//can turn more
      {
        if(chance < 0.3)
        {
          random = rand() % (4 - 1 + 1) + 0;
          if(random == 0)//south 1
          {
            drift ++;
            terrain[drift][j]->setToken("#");
          }
          else if(random == 1)//north 1
          {
            drift --;
            terrain[drift][j]->setToken("#");
          }
          else if(random == 2)//south 2
          {
            drift ++;
            terrain[drift][j]->setToken("#");
            if(drift < (terrainSize - 1))
            {
              drift++;
              terrain[drift][j]->setToken("#");
            }
          }
          else if(random == 3)//north 2
          {
            drift--;
            terrain[drift][j]->setToken("#");
            if(drift > 0)
            {
              drift--;
              terrain[drift][j]->setToken("#");
            }
          }
        }
        j++;
      }
    }
  }
  else if(side == 2)//South
  {
    i = terrainSize - 1;
    drift = start;
    while(i >= 0)//till north is reached
    {
      terrain[i][drift]->setToken("#");
      chance = ((double) rand() / (RAND_MAX));
      if((drift == (terrainSize - 1)) || (drift == 0))//reached east or west side
      {
        i--;
        continue;
      }
      else//can turn more
      {
        if(chance < 0.3)
        {
          random = rand() % (4 - 1 + 1) + 0;
          if(random == 0)//east 1
          {
            drift ++;
            terrain[i][drift]->setToken("#");
          }
          else if(random == 1)//west 1
          {
            drift --;
            terrain[i][drift]->setToken("#");
          }
          else if(random == 2)//east 2
          {
            drift ++;
            terrain[i][drift]->setToken("#");
            if(drift < (terrainSize - 1))
            {
              drift++;
              terrain[i][drift]->setToken("#");
            }
          }
          else if(random == 3)//west 2
          {
            drift--;
            terrain[i][drift]->setToken("#");
            if(drift > 0)
            {
              drift--;
              terrain[i][drift]->setToken("#");
            }
          }
        }
        i--;
      }
    }
  }
  else if(side == 3)//East
  {
    drift = start;
    j = terrainSize - 1;
    while(j >= 0)//till west side is reached
    {
      terrain[drift][j]->setToken("#");
      chance = ((double) rand() / (RAND_MAX));
      if((drift == (terrainSize - 1)) || (drift == 0))//reached south or north side
      {
        j--;
        continue;
      }
      else//can turn more
      {
        if(chance < 0.3)
        {
          random = rand() % (4 - 1 + 1) + 0;
          if(random == 0)//south 1
          {
            drift ++;
            terrain[drift][j]->setToken("#");
          }
          else if(random == 1)//north 1
          {
            drift --;
            terrain[drift][j]->setToken("#");
          }
          else if(random == 2)//south 2
          {
            drift ++;
            terrain[drift][j]->setToken("#");
            if(drift < (terrainSize - 1))
            {
              drift++;
              terrain[drift][j]->setToken("#");
            }
          }
          else if(random == 3)//north 2
          {
            drift--;
            terrain[drift][j]->setToken("#");
            if(drift > 0)
            {
              drift--;
              terrain[drift][j]->setToken("#");
            }
          }
        }
        j--;
      }
    }
  }
  cout << "River generated successfully!\n";
}

void Map::generateLake(const int & size, const int & cord1, const int & cord2)
{
  cout << "Generating a lake ...\n";//4 combinations for lake expansion
  int random1 = rand() % 2;//pick a 2-way main growing direction
  int random2 = rand() % 2;//pick a 2-way secondary growing direction
  if(random1 == 0)//stretch north
  {
    if(random2 == 0)//stretch west
    {
      for(int i = cord1; i > (cord1 - size); i--)
        for(int j = cord2; j > (cord2 - size); j--)
          terrain[i][j]->setToken("#");
    }
    else if(random2 == 1)//stretch east
    {
      for(int i = cord1; i > (cord1 - size); i--)
        for(int j = cord2; j < (cord2 + size); j++)
          terrain[i][j]->setToken("#");
    }
  }
  else if(random1 == 1)//stretch south
  {
    if(random2 == 0)//stretch west
    {
      for(int i = cord1; i < (cord1 + size); i++)
        for(int j = cord2; j > (cord2 - size); j--)
          terrain[i][j]->setToken("#");
    }
    else if(random2 == 1)//stretch east
    {
      for(int i = cord1; i < (cord1 + size); i++)
        for(int j = cord2; j < (cord2 + size); j++)
          terrain[i][j]->setToken("#");
    }
  }
  cout << "Lake generated successfully!\n";
}

void Map::generateHills(const int & size, int & side)
{
  string temp = "\0";//empty tile
  if(size == 2)//create a size 2 line
  {
    int cord1 = rand() % (terrainSize - size + 2);
    int cord2 = rand() % (terrainSize - size + 2);
    if(side == 0)//expand north
    {
      if(cord1 == 0)
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1-1][cord2]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1-1][cord2]->setToken("^");
      }
      else
        side++;
    }
    if(side == 1)//expand west
    {
      if(cord2 == 0)
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1][cord2-1]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1][cord2-1]->setToken("^");
      }
      else
        side++;
    }
    if(side == 2)//expand south
    {
      if(cord1 == (terrainSize - 1))
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1+1][cord2]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1+1][cord2]->setToken("^");
      }
      else
        side++;
    }
    if(side == 3)//expand east
    {
      if(cord2 != (terrainSize - 1))
      {
        if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
        (temp.compare(terrain[cord1][cord2+1]->getToken()) == 0))
        {
          terrain[cord1][cord2]->setToken("^");
          terrain[cord1][cord2+1]->setToken("^");
        }
      }
    }
  }
  else if(size == 3)//create a boomerang shape
  {
    int cord1 = rand() % (terrainSize - size + 2);
    int cord2 = rand() % (terrainSize - size + 2);
    int turn = rand() % 2;//2-way secondary choise
    if(side == 0)//first north
    {
      if(cord1 == 0)//north border
        side++;
      else if((cord2 == 0) || (cord2 == (terrainSize - 1)))//west or east border
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) != 0) ||
              (temp.compare(terrain[cord1-1][cord2]->getToken()) != 0))//tiles full
        side++;
      else
      {
        if(turn == 0)//then check west first
        {
          if(((cord2 - 1) != 0) && 
          (temp.compare(terrain[cord1-1][cord2-1]->getToken()) == 0))//not west border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1-1][cord2]->setToken("^");
            terrain[cord1-1][cord2-1]->setToken("^");
          }
          else if(((cord2 + 1) != (terrainSize - 1)) && 
          (temp.compare(terrain[cord1-1][cord2+1]->getToken()) == 0))//not east border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1-1][cord2]->setToken("^");
            terrain[cord1-1][cord2+1]->setToken("^");
          }
          else
            side++;
        }
        else if(turn == 1)//then check east first
        {
          if(((cord2 + 1) != (terrainSize - 1)) && 
          (temp.compare(terrain[cord1-1][cord2+1]->getToken()) == 0))//not east border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1-1][cord2]->setToken("^");
            terrain[cord1-1][cord2+1]->setToken("^");
          }
          else if(((cord2 - 1) != 0) && 
          (temp.compare(terrain[cord1-1][cord2-1]->getToken()) == 0))//not west border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1-1][cord2]->setToken("^");
            terrain[cord1-1][cord2-1]->setToken("^");
          }
          else
            side++;
        }
      }
    }
    if(side == 1)//first west
    {
      if(cord2 == 0)//west border
        side++;
      else if((cord1 == 0) || (cord1 == (terrainSize - 1)))//north or south border
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) != 0) ||
              (temp.compare(terrain[cord1][cord2-1]->getToken()) != 0))//tiles full
        side++;
      else
      {
        if(turn == 0)//then check north first
        {
          if(((cord1 - 1) != 0) &&
          (temp.compare(terrain[cord1-1][cord2-1]->getToken()) == 0))//not north border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2-1]->setToken("^");
            terrain[cord1-1][cord2-1]->setToken("^");
          }
          else if((cord1 + 1) != (terrainSize - 1) ||
          (temp.compare(terrain[cord1+1][cord2-1]->getToken()) == 0))//not south border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2-1]->setToken("^");
            terrain[cord1+1][cord2-1]->setToken("^");
          }
          else
            side++;
        }
        else if(turn == 1)//then check south first
        {
          if((cord1 + 1) != (terrainSize - 1) ||
          (temp.compare(terrain[cord1+1][cord2-1]->getToken()) == 0))//not south border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2-1]->setToken("^");
            terrain[cord1+1][cord2-1]->setToken("^");
          }
          else if(((cord1 - 1) != 0) &&
          (temp.compare(terrain[cord1-1][cord2-1]->getToken()) == 0))//not north border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2-1]->setToken("^");
            terrain[cord1-1][cord2-1]->setToken("^");
          }
          else
            side++;
        }
      }
    }
    if(side == 2)//first south
    {
      if(cord1 == (terrainSize - 1))//south border
        side++;
      else if((cord2 == 0) || (cord2 == (terrainSize - 1)))//west or east border
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) != 0) ||
              (temp.compare(terrain[cord1+1][cord2]->getToken()) != 0))//tiles full
        side++;
      else
      {
        if(turn == 0)//then check west first
        {
          if(((cord2 - 1) != 0) && 
          (temp.compare(terrain[cord1+1][cord2-1]->getToken()) == 0))//not west border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1+1][cord2]->setToken("^");
            terrain[cord1+1][cord2-1]->setToken("^");
          }
          else if(((cord2 + 1) != (terrainSize - 1)) && 
          (temp.compare(terrain[cord1+1][cord2+1]->getToken()) == 0))//not east border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1+1][cord2]->setToken("^");
            terrain[cord1+1][cord2+1]->setToken("^");
          }
          else
            side++;
        }
        else if(turn == 1)//then check east first
        {
          if(((cord2 + 1) != (terrainSize - 1)) && 
          (temp.compare(terrain[cord1+1][cord2+1]->getToken()) == 0))//not east border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1+1][cord2]->setToken("^");
            terrain[cord1+1][cord2+1]->setToken("^");
          }
          else if(((cord2 - 1) != 0) && 
          (temp.compare(terrain[cord1+1][cord2-1]->getToken()) == 0))//not west border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1+1][cord2]->setToken("^");
            terrain[cord1+1][cord2-1]->setToken("^");
          }
          else
            side++;
        }
      }
    }
    if(side == 3)//first east
    {
      if(cord2 == (terrainSize - 1))//east border
        side++;
      else if((cord1 == 0) || (cord1 == (terrainSize - 1)))//north or south border
        side++;
      else if((temp.compare(terrain[cord1][cord2]->getToken()) != 0) ||
             (temp.compare(terrain[cord1][cord2+1]->getToken()) != 0))//tiles full
        side++;
      else
      {
        if(turn == 0)//then check north first
        {
          if(((cord1 - 1) != 0) &&
          (temp.compare(terrain[cord1-1][cord2+1]->getToken()) == 0))//not north border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2+1]->setToken("^");
            terrain[cord1-1][cord2+1]->setToken("^");
          }
          else if((cord1 + 1) != (terrainSize - 1) ||
          (temp.compare(terrain[cord1+1][cord2+1]->getToken()) == 0))//not south border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2+1]->setToken("^");
            terrain[cord1+1][cord2+1]->setToken("^");
          }
          else
            side++;
        }
        else if(turn == 1)//then check south first
        {
          if((cord1 + 1) != (terrainSize - 1) ||
          (temp.compare(terrain[cord1+1][cord2+1]->getToken()) == 0))//not south border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2+1]->setToken("^");
            terrain[cord1+1][cord2+1]->setToken("^");
          }
          else if(((cord1 - 1) != 0) &&
          (temp.compare(terrain[cord1-1][cord2+1]->getToken()) == 0))//not north border or full
          {
            terrain[cord1][cord2]->setToken("^");
            terrain[cord1][cord2+1]->setToken("^");
            terrain[cord1-1][cord2+1]->setToken("^");
          }
          else
            side++;
        }
      }
    }
  }
  else if(size == 4)//create a square
  {
    int cord1 = rand() % ((terrainSize - size + 1) - (size - 1)) + (size - 3);
    int cord2 = rand() % ((terrainSize - size + 1) - (size - 1)) + (size - 3);
    if(side == 0)//expand NW
    {
      if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1-1][cord2]->getToken()) == 0) &&
      (temp.compare(terrain[cord1-1][cord2-1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1][cord2-1]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1-1][cord2]->setToken("^");
        terrain[cord1-1][cord2-1]->setToken("^");
        terrain[cord1][cord2-1]->setToken("^");
      }
      else
        side++;
    }
    if(side == 1)//expand SW
    {
      if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1][cord2-1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1+1][cord2-1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1+1][cord2]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1][cord2-1]->setToken("^");
        terrain[cord1+1][cord2-1]->setToken("^");
        terrain[cord1+1][cord2]->setToken("^");
      }
      else
        side++;
    }
    if(side == 2)//expand SE
    {
      if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1+1][cord2]->getToken()) == 0) &&
      (temp.compare(terrain[cord1+1][cord2+1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1][cord2+1]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1+1][cord2]->setToken("^");
        terrain[cord1+1][cord2+1]->setToken("^");
        terrain[cord1][cord2+1]->setToken("^");
      }
      else
        side++;
    }
    if(side == 3)//expand NE
    {
      if((temp.compare(terrain[cord1][cord2]->getToken()) == 0) && 
      (temp.compare(terrain[cord1][cord2+1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1-1][cord2+1]->getToken()) == 0) &&
      (temp.compare(terrain[cord1-1][cord2]->getToken()) == 0))
      {
        terrain[cord1][cord2]->setToken("^");
        terrain[cord1][cord2+1]->setToken("^");
        terrain[cord1-1][cord2+1]->setToken("^");
        terrain[cord1-1][cord2]->setToken("^");
      }
    }
  }
}

void Map::generateMeadow()
{
  cout << "Generating meadow ...\n";
  string temp = "\0";
  for(int i = 0; i < terrainSize; i++)
    for(int j = 0; j < terrainSize; j++)
      if(temp.compare(terrain[i][j]->getToken()) == 0)//found empty tile
        terrain[i][j]->setToken("\"");
}

void Map::printSystem() const
{
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
    {
      terrain[i][j]->printType();
      cout << " ";
    }
    cout << "\n";
  }
}

int Map::countWater()
{
  int counter = 0;
  string target = "#";
  for(int i = 0; i < terrainSize; i++)
    for(int j = 0; j < terrainSize; j++)
      if(target.compare(terrain[i][j]->getToken()) == 0)
        counter++;
  return counter;
}

int Map::countMeadow()
{
  int counter = 0;
  string target = "\"";
  for(int i = 0; i < terrainSize; i++)
    for(int j = 0; j < terrainSize; j++)
      if(target.compare(terrain[i][j]->getToken()) == 0)
        counter++;
  return counter;
}

int Map::countHills()
{
  int counter = 0;
  string target = "^";
  for(int i = 0; i < terrainSize; i++)
    for(int j = 0; j < terrainSize; j++)
      if(target.compare(terrain[i][j]->getToken()) == 0)
        counter++;
  return counter;
}

void Map::printPlantStatistics() const
{
  int numAlgae = 0;
  int numGrass = 0;
  int numOak = 0;
  int numMapple = 0;
  int numPine = 0;
  int totalPlants = 0;
  int totalSeeded = 0;
  int totalSeedless = 0;
  for(int i = 0; i < terrainSize; i++)
    for(int j = 0; j < terrainSize; j++)
      terrain[i][j]->getPlantStats(numAlgae, numGrass, numOak, numMapple, numPine);
  totalSeedless = numAlgae + numGrass;
  totalSeeded = numOak + numMapple + numPine;
  totalPlants = totalSeeded + totalSeedless;
  cout << "Total Plants : " << totalPlants << endl;
  cout << "Total Seedless Plants : " << totalSeedless << endl;
  cout << "Total Grass : " << numGrass << endl;
  cout << "Total Algae : " << numAlgae << endl;
  cout << "Total Seeded Plants : " << totalSeeded << endl;
  cout << "Total Oaks : " << numOak << endl;
  cout << "Total Mapples : " << numMapple << endl;
  cout << "Total Pines : " << numPine << endl;
}

void Map::placePlants()
{
  int waterRate = countWater();
  int mountainRate = countHills();
  int meadowRate = countMeadow();
  int numAlgae = waterRate; //1/1 of water algae (max)
  int numGrass = meadowRate / 2;//1/2 of meadow grass (max)
  int numOak = meadowRate / 3;//1/3 of meadow oak (max)
  int numMapple = (meadowRate / 4);//1/4 of meadow mapple (max)
  int numPine = mountainRate;//1/1 of hills pine (max)
  int cord1;
  int cord2;
  string target = "#";
  int flag;
  for(int i = 0; i < numAlgae; i++)//place the algae
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if((target.compare(terrain[cord1][cord2]->getToken()) == 0) &&
        (terrain[cord1][cord2]->getPlant() == false))//found the correct and empty tile
        {
          terrain[cord1][cord2]->sprout("Algae", "A", cord1, cord2, 0.25, 0.25, 5, 2, 0, 0, 0,true);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  for(int i = 0; i < numGrass; i++)//place the grass
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if((target.compare(terrain[cord1][cord2]->getToken()) == 0) &&
        (terrain[cord1][cord2]->getPlant() == false))//found the correct and empty tile
        {
          terrain[cord1][cord2]->sprout("Grass", "G", cord1, cord2, 0.15, 0.15, 5, 4, 0, 0, 0,true);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  for(int i = 0; i < numOak; i++)//place the oaks
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if((target.compare(terrain[cord1][cord2]->getToken()) == 0) &&
        (terrain[cord1][cord2]->getPlant() == false))//found the correct and empty tile
        {
          terrain[cord1][cord2]->sprout("Oak","O", cord1, cord2,0.20, 0.20, 45, 15, 15, 30, 5,true);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  string target2 = "^";
  for(int i = 0; i < numPine; i++)//place the pines
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if((target.compare(terrain[cord1][cord2]->getToken()) == 0) &&
        (terrain[cord1][cord2]->getPlant() == false))//found the correct and empty tile
        {
          terrain[cord1][cord2]->sprout("Pine","P",cord1,cord2,0.15, 0.15, 60, 20, 20, 40, 5, true);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  for(int i = 0; i < numMapple; i++)//place the mapples
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(((target2.compare(terrain[cord1][cord2]->getToken()) == 0) ||
          (target.compare(terrain[cord1][cord2]->getToken()) == 0)) &&
          (terrain[cord1][cord2]->getPlant() == false))//found the correct and empty tile
        {
          terrain[cord1][cord2]->sprout("Mapple", "M", cord1, cord2, 0.05, 0.05, 30, 10, 10, 20, 2, true);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
}

void Map::placeAnimals()
{
  int waterRate = countWater();
  int mountainRate = countHills();
  int meadowRate = countMeadow();
  //carnivores max number 
  int numFox;
  int numWolf;
  int numBear;
  //herbivores max number 
  int numSalmon = waterRate / 2;
  int numRabbit = meadowRate + waterRate;
  int numGroundhog = (meadowRate / 2) + mountainRate;
  int numDeer = (meadowRate / 6) + (mountainRate / 6) + (waterRate / 6);
  int cord1;
  int cord2;
  int flag;
  string target = "#";
  //animals that managed to actually spawn
  int spawnedS = 0;
  int spawnedR = 0;
  int spawnedG = 0;
  int spawnedD = 0;
  //spawn the herbivores
  for(int i = 0; i < numSalmon; i++)//place the salmons
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnH("Salmon", "H", 1, 5, 0, 0, 1, cord1, cord2,
            true, true, true, true, false, false, false, 1);
          flag = 1;
          spawnedS++;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  for(int i = 0; i < numDeer; i++)//place the deers
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnH("Deer", "H", 5, 8, 0, 0, 2, cord1, cord2,
            true, true, true, true, false, false, false, 8);
          flag = 1;
          spawnedD++;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  for(int i = 0; i < numRabbit; i++)//place the rabbits
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnH("Rabbit", "H", 2, 6, 0, 0, 1, cord1, cord2,
            true, true, true, true, false, false, false, 4);
          flag = 1;
          spawnedR++;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  for(int i = 0; i < numGroundhog; i++)//place the groundhogs
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnH("Groundhog", "H", 3, 5, 0, 0, 1, cord1, cord2,
            true, true, true, true, true, false, true, 5);
          flag = 1;
          spawnedG++;
        }
        cord2++;
      }
      cord1++;
    }
  }
  if(terrainSize < 30)
  {
    numFox = (spawnedR / 50) + (spawnedG / 50);
    numWolf = (spawnedR / 60) + (spawnedG / 60);
    numBear = (spawnedD / 30) + (spawnedS / 50);
  }
  else
  {
    numFox = (spawnedR / 70) + (spawnedG / 70);
    numWolf = (spawnedR / 90) + (spawnedG / 90);
    numBear = (spawnedD / 40) + (spawnedS / 60);
  }
  //spawn the carnivores
  target = "\"";
  for(int i = 0; i < numFox; i++)//place the foxes
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnC("Fox", "C", 4, 6, 0, 0, 6, cord1, cord2,
            true, true, true, true, false, false, 5, 5, false);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "\"";
  string target1 = "^";
  for(int i = 0; i < numWolf; i++)//place the wolves
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if((target.compare(terrain[cord1][cord2]->getToken()) == 0) ||
        (target1.compare(terrain[cord1][cord2]->getToken()) == 0))//found the correct tile
        {
          terrain[cord1][cord2]->spawnC("Wolf", "C", 7, 8, 0, 0, 8, cord1, cord2,
            true, true, true, true, false, false, 8, 6, false);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
  target = "^";
  for(int i = 0; i < numBear; i++)//place the bears
  {
    flag = 0;
    cord1 = rand() % terrainSize;
    cord2 = rand() % terrainSize;
    while(cord1 < terrainSize)
    {
      if(flag == 1)
        break;
      while(cord2 < terrainSize)
      {
        if(flag == 1)
          break;
        if(target.compare(terrain[cord1][cord2]->getToken()) == 0)//found the correct tile
        {
          terrain[cord1][cord2]->spawnC("Bear", "C", 10, 4, 0, 0, 10, cord1, cord2,
            true, true, true, true, true, false, 10, 10, false);
          flag = 1;
        }
        cord2++;
      }
      cord1++;
    }
  }
}

void Map::applySeason(string curSeason)
{
  string temp0 = "winter";
  string temp1 = "spring";
  string temp2 = "summer";
  string temp3 = "autumn";
  if(curSeason.compare(temp0) == 0)//winter effects
  {
    cout << "Winter has come...\n";
    growthPeriodP = 10;//grow every 10 days
    breedingRepPeriodP = 0;//plants dont breed
    growthPeriodA = 30;//grow every 30 days
    breedingRepPeriodH = 18;
    breedingRepPeriodC = 10;
    for(int i = 0; i < terrainSize; i++)
      for(int j = 0; j < terrainSize; j++)
        terrain[i][j]->setHibernation();
  }
  else if(curSeason.compare(temp1) == 0)//spring effects
  {
    cout << "Spring has come...\n";
    growthPeriodP = 5;
    breedingRepPeriodP = 10;
    growthPeriodA = 20;
    breedingRepPeriodH = 12;
    breedingRepPeriodC = 11;
    for(int i = 0; i < terrainSize; i++)
      for(int j = 0; j < terrainSize; j++)
        terrain[i][j]->setWakedUp();
  }
  else if(curSeason.compare(temp2) == 0)//summer effects
  {
    cout << "Summer has come...\n";
    growthPeriodP = 10;
    breedingRepPeriodP = 10;
    growthPeriodA = 30;
    breedingRepPeriodH = 8;
    breedingRepPeriodC = 9;
  }
  else if(curSeason.compare(temp3) == 0)//autumn effects
  {
    cout << "Autumn has come...\n";
    growthPeriodP = 0;//plants dont grow
    breedingRepPeriodP = 20;
    growthPeriodA = 15;
    breedingRepPeriodH = 5;
    breedingRepPeriodC = 9;
  }
}

void Map::runEcosystem(const int & totalDays)
{
  cout << endl;
  string curSeason = dayOfYear->getEpoch();//get starting epoch
  applySeason(curSeason);//apply current epoch effects
  int curDay = 1;//start day counter
  int curHour = 0;//start hour counter
  int animalCount = 0;
  int * xList;//horizontal cords
  int * yList;//vertical cords
  int * spdList;//speed
  int * posList;//node position
  int * sizeList;//size
  int * atkList;//attack
  int * defList;//defence
  int * climbList;//can climb
  int * needsList;//daily needs
  int * eatenList;//eaten food
  int * perMealList;//dph or meal
  int * hungerList;//hunger
  int * killerList;//carnivore hunter
  int eatenAmount;//food gained after finding plant or victim
  int eatenSoFar;//updated eaten
  int newCord1 = 0;//rellocation horizontal
  int newCord2 = 0;//rellocation vertical
  string * nameList;
  string temp1 = "Salmon";
  string temp2 = "Rabbit";
  string temp3 = "Groundhog";
  string temp4 = "Deer";
  string temp5 = "Fox";
  string temp6 = "Wolf";
  string temp7 = "Bear";
  string temp8 = "A";
  bool changeTile = false;//animal moved to a new tile
  int filled = false;//animal filled completely
  int victimPos = 0;
  bool victimFound = false;
  while(curDay <= totalDays)//days left to simulate
  {
    cout << "DAY : " << curDay << endl;
    dailyReset(curDay);//reset hunger, grow if conditions met, etc
    while(curHour < 24)//same day
    {
      for(int i = 0; i < terrainSize; i++)
      {
        for(int j = 0; j < terrainSize; j++)
        {
          animalCount = terrain[i][j]->getCount();
          if(animalCount > 0)//there are animals on this spawn list
          {
            xList = new int[animalCount];//cordsX
            yList = new int[animalCount];//cordsY
            spdList = new int[animalCount];//speeds
            nameList = new string[animalCount];//names
            posList = new int[animalCount];//node positions for cord altering
            sizeList = new int[animalCount];
            atkList = new int[animalCount];
            defList = new int[animalCount];
            climbList = new int[animalCount];
            needsList = new int[animalCount];
            eatenList = new int[animalCount];
            perMealList = new int[animalCount];
            hungerList = new int[animalCount];
            killerList = new int[animalCount];
            for(int k = 0; k < animalCount; k++)
            {//init
              xList[k] = -1;
              yList[k] = -1;
              spdList[k] = -1;
              posList[k] = -1;
              sizeList[k] = -1;
              atkList[k] = -1;
              defList[k] = -1;
              climbList[k] = -1;
              needsList[k] = -1;
              eatenList[k] = -1;
              perMealList[k] = -1;
              hungerList[k] = -1;
              killerList[k] = -1;
            }//get updated animal attributes every hour
            terrain[i][j]->getAnimals(xList, yList, spdList, nameList, posList, sizeList,
            atkList, defList, climbList, needsList, eatenList, perMealList, hungerList, killerList);
            for(int k = 0; k < animalCount; k++)
            {
              changeTile = false;//init for next
              eatenAmount = 0;
              newCord1 = 0;
              newCord2 = 0;
              filled = false;
              eatenSoFar = 0;
              victimPos = -1;
              victimFound = false;
              if((hungerList[k] != -1) && (hungerList[k] != 0))//skip empty and non-hungry
              {
                if(temp1.compare(nameList[k]) == 0)//salmon moves
                {// first check same tile , if no found plant move and check again, if found stay
                  if(terrain[xList[k]][yList[k]]->isAlive() == true)//food on same tile with position!
                  {//salmon just eats algae so no point checking what the plant is just eat it!
                    eatenSoFar = eatenList[k];
                    if(eatenSoFar < needsList[k])
                      eatenAmount = animalEating("H", xList[k], yList[k], perMealList[k], -1, climbList[k], false, -1, -1);
                    eatenSoFar += eatenAmount;//new added on old
                    if(eatenSoFar >= needsList[k])//animal filled
                      filled = true;
                    terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                  }
                  else
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Salmon");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      if(terrain[newCord1][newCord2]->isAlive() == true)
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < needsList[k])
                        {
                          eatenAmount = animalEating("H", newCord1, newCord2, perMealList[k], -1, 
                                                    climbList[k], false, -1, -1);
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= needsList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                      }
                    }
                  }
                }
                else if(temp2.compare(nameList[k]) == 0)//rabbit moves
                {
                  if((terrain[xList[k]][yList[k]]->isAlive() == true) && 
                  (temp8.compare(terrain[xList[k]][yList[k]]->getPlantToken()) != 0) &&
                  (terrain[xList[k]][yList[k]]->getSize() <= sizeList[k]) &&
                  (terrain[xList[k]][yList[k]]->hasFoliage() == true))
                  {//food on same tile with position != Algae and if seeded size less than rabbits
                    eatenSoFar = eatenList[k];
                    if(eatenSoFar < needsList[k])
                    {
                      eatenAmount = animalEating("H", xList[k], yList[k], perMealList[k], -1,
                                                 climbList[k], false, -1, -1);
                    }
                    eatenSoFar += eatenAmount;//new added on old
                    if(eatenSoFar >= needsList[k])//animal filled
                      filled = true;
                    terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                  }
                  else
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Rabbit");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      if((terrain[newCord1][newCord2]->isAlive() == true) && 
                      (temp8.compare(terrain[newCord1][newCord2]->getPlantToken()) != 0) &&
                      (terrain[newCord1][newCord2]->getSize() <= sizeList[k]) &&
                      (terrain[newCord1][newCord2]->hasFoliage() == true))
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < needsList[k])
                        {
                          eatenAmount = animalEating("H", newCord1, newCord2, perMealList[k], -1,
                                                     climbList[k], false, -1, -1);
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= needsList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                      }
                    }
                  }
                }
                else if(temp3.compare(nameList[k]) == 0)//groundhog moves
                {
                  if(terrain[xList[k]][yList[k]]->isAlive() == true)
                  {//food on same tile with position and seeded size <= groundhog size
                    if((terrain[xList[k]][yList[k]]->getSize() <= (3*sizeList[k])) &&
                    (terrain[xList[k]][yList[k]]->getSize() <= sizeList[k]) &&
                    (terrain[xList[k]][yList[k]]->hasSeeds() == true))//can eat leaves and seeds
                    {
                      eatenSoFar = eatenList[k];
                      if(eatenSoFar < needsList[k])
                      {
                        eatenAmount = animalEating("H", xList[k], yList[k], perMealList[k], -1, climbList[k], true, -1, -1);
                      }
                      eatenSoFar += eatenAmount;//new added on old
                      if(eatenSoFar >= needsList[k])//animal filled
                        filled = true;
                      terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                    }
                    else if((terrain[xList[k]][yList[k]]->getSize() <= (3*sizeList[k])) &&
                    (terrain[xList[k]][yList[k]]->hasSeeds() == true))//can eat just the seeds
                    {
                      eatenSoFar = eatenList[k];
                      if(eatenSoFar < needsList[k])
                        eatenAmount = animalEating("H", xList[k], yList[k], perMealList[k], -1, 
                                                  climbList[k], false, -1, -1);
                      eatenSoFar += eatenAmount;//new added on old
                      if(eatenSoFar >= needsList[k])//animal filled
                        filled = true;
                      terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                    }
                  }
                  else
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Groundhog");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      if(terrain[newCord1][newCord2]->isAlive() == true)
                      {
                        if((terrain[newCord1][newCord2]->getSize() <= (3*sizeList[k])) &&
                        (terrain[newCord1][newCord2]->getSize() <= sizeList[k]) &&
                        (terrain[newCord1][newCord2]->hasSeeds() == true))//can eat leaves and seeds
                        {
                          eatenSoFar = eatenList[k];
                          if(eatenSoFar < needsList[k])
                          {
                            eatenAmount = animalEating("H", newCord1, newCord2, perMealList[k], -1, 
                                                      climbList[k], true, -1, -1);
                          }
                          eatenSoFar += eatenAmount;//new added on old
                          if(eatenSoFar >= needsList[k])//animal filled
                            filled = true;
                          terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                        }
                        else if((terrain[newCord1][newCord2]->getSize() <= (3*sizeList[k])) &&
                        (terrain[newCord1][newCord2]->hasSeeds() == true))//can eat just the seeds
                        {
                          eatenSoFar = eatenList[k];
                          if(eatenSoFar < needsList[k])
                          {
                            eatenAmount = animalEating("H", newCord1, newCord2, perMealList[k], -1,
                                                      climbList[k], false, -1, -1);
                          }
                          eatenSoFar += eatenAmount;//new added on old
                          if(eatenSoFar >= needsList[k])//animal filled
                            filled = true;
                          terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                        }
                      }
                    }
                  }
                }
                else if(temp4.compare(nameList[k]) == 0)//deer moves
                {//same tile and plant size <= deer size +4
                  if((terrain[xList[k]][yList[k]]->isAlive() == true) &&
                  (terrain[xList[k]][yList[k]]->getSize() <= (sizeList[k] + 4)) &&
                  (terrain[xList[k]][yList[k]]->hasFoliage() == true))
                  {
                    eatenSoFar = eatenList[k];
                    if(eatenSoFar < needsList[k])
                    {
                      eatenAmount = animalEating("H", xList[k], yList[k], perMealList[k], -1,
                                                 climbList[k], false, -1, -1);
                    }
                    eatenSoFar += eatenAmount;//new added on old
                    if(eatenSoFar >= needsList[k])//animal filled
                      filled = true;
                    terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                  }
                  else
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Deer");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      if((terrain[newCord1][newCord2]->isAlive() == true) &&
                      (terrain[newCord1][newCord2]->getSize() <= (sizeList[k] + 4)) &&
                      (terrain[newCord1][newCord2]->hasFoliage() == true))
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < needsList[k])
                        {
                          eatenAmount = animalEating("H", newCord1, newCord2, perMealList[k], -1,
                                                     climbList[k], false, -1, -1);
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= needsList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                      }
                    }
                  }
                }
                else if(temp5.compare(nameList[k]) == 0)
                {
                  for(int a = 0; a < terrainSize; a++)
                  {
                    if(victimFound == true)
                      break;
                    for(int b = 0; b < terrainSize; b++)
                    {
                      if(victimFound == true)
                        break;
                      victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k], nameList[k],
                      killerList[k], atkList[k], xList[k], yList[k]);//search same way
                      if(victimPos == -1)
                        continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                      else
                        victimFound = true;
                      if(victimFound == true)//eat it
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < perMealList[k])
                        {
                          eatenAmount = animalEating("C", xList[k], yList[k], -1, victimPos,
                          -1, false, a, b);//a and b are the needed victim's tile's indexes
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= perMealList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                        checkDeadEntities();
                        for(int k = 0; k < animalCount; k++)
                        {//init
                          xList[k] = -1;
                          yList[k] = -1;
                          spdList[k] = -1;
                          posList[k] = -1;
                          sizeList[k] = -1;
                          atkList[k] = -1;
                          defList[k] = -1;
                          climbList[k] = -1;
                          needsList[k] = -1;
                          eatenList[k] = -1;
                          perMealList[k] = -1;
                          hungerList[k] = -1;
                          killerList[k] = -1;
                        }//get updated animal attributes every hunt
                        terrain[i][j]->getAnimals(xList, yList, spdList, nameList, posList,
                          sizeList, atkList, defList, climbList, needsList, eatenList,
                          perMealList, hungerList, killerList);
                      }
                    }
                  }
                  if(victimFound == false)//couldnt find anything have to move and look again
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Fox");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      for(int a = 0; a < terrainSize; a++)
                      {
                        if(victimFound == true)
                          break;
                        for(int b = 0; b < terrainSize; b++)
                        {
                          if(victimFound == true)
                            break;
                          victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k], 
                            nameList[k], killerList[k], atkList[k], newCord1, newCord2);//search
                          if(victimPos == -1)
                            continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                          else
                            victimFound = true;
                          if(victimFound == true)//eat it
                          {
                            eatenSoFar = eatenList[k];
                            if(eatenSoFar < perMealList[k])
                            {
                              eatenAmount = animalEating("C", newCord1, newCord2, -1,
                               victimPos, -1, false, a, b);//a and b are the needed victim's tile's indexes
                            }
                            eatenSoFar += eatenAmount;//new added on old
                            if(eatenSoFar >= perMealList[k])//animal filled
                              filled = true;
                            terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                            checkDeadEntities();
                            for(int k = 0; k < animalCount; k++)
                            {//init
                              xList[k] = -1;
                              yList[k] = -1;
                              spdList[k] = -1;
                              posList[k] = -1;
                              sizeList[k] = -1;
                              atkList[k] = -1;
                              defList[k] = -1;
                              climbList[k] = -1;
                              needsList[k] = -1;
                              eatenList[k] = -1;
                              perMealList[k] = -1;
                              hungerList[k] = -1;
                              killerList[k] = -1;
                            }//get updated animal attributes every hour
                            terrain[i][j]->getAnimals(xList, yList, spdList, nameList, posList,
                              sizeList, atkList, defList, climbList, needsList, eatenList,
                              perMealList, hungerList, killerList);
                          }
                        }
                      }
                    }
                  }
                }
                else if(temp6.compare(nameList[k]) == 0)//wolf moves
                {
                  for(int a = 0; a < terrainSize; a++)
                  {
                    if(victimFound == true)
                      break;
                    for(int b = 0; b < terrainSize; b++)
                    {
                      if(victimFound == true)
                        break;
                      victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k], nameList[k],
                        killerList[k], atkList[k], xList[k], yList[k]);//search same way
                      if(victimPos == -1)
                        continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                      else
                        victimFound = true;
                      if(victimFound == true)//eat it
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < perMealList[k])
                        {
                          eatenAmount = animalEating("C", xList[k], yList[k], -1, victimPos,
                           -1, false, a, b);//a and b are the needed victim's tile's indexes
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= perMealList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                        checkDeadEntities();
                        for(int k = 0; k < animalCount; k++)
                        {//init
                          xList[k] = -1;
                          yList[k] = -1;
                          spdList[k] = -1;
                          posList[k] = -1;
                          sizeList[k] = -1;
                          atkList[k] = -1;
                          defList[k] = -1;
                          climbList[k] = -1;
                          needsList[k] = -1;
                          eatenList[k] = -1;
                          perMealList[k] = -1;
                          hungerList[k] = -1;
                          killerList[k] = -1;
                        }//get updated animal attributes every hour
                        terrain[i][j]->getAnimals(xList, yList, spdList, nameList, posList,
                          sizeList, atkList, defList, climbList, needsList,
                          eatenList, perMealList, hungerList, killerList);
                      }
                    }
                  }
                  if(victimFound == false)//couldnt find anything have to move and look again
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Wolf");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      for(int a = 0; a < terrainSize; a++)
                      {
                        if(victimFound == true)
                          break;
                        for(int b = 0; b < terrainSize; b++)
                        {
                          if(victimFound == true)
                            break;
                          victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k],
                            nameList[k], killerList[k], atkList[k], newCord1, newCord2);//search
                          if(victimPos == -1)
                            continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                          else
                            victimFound = true;
                          if(victimFound == true)//eat it
                          {
                            eatenSoFar = eatenList[k];
                            if(eatenSoFar < perMealList[k])
                            {
                              eatenAmount = animalEating("C", newCord1, newCord2, -1,
                                victimPos, -1, false, a, b);//a and b are the needed victim's tile's indexes
                            }
                            eatenSoFar += eatenAmount;//new added on old
                            if(eatenSoFar >= perMealList[k])//animal filled
                              filled = true;
                            terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                            checkDeadEntities();
                            for(int k = 0; k < animalCount; k++)
                            {//init
                              xList[k] = -1;
                              yList[k] = -1;
                              spdList[k] = -1;
                              posList[k] = -1;
                              sizeList[k] = -1;
                              atkList[k] = -1;
                              defList[k] = -1;
                              climbList[k] = -1;
                              needsList[k] = -1;
                              eatenList[k] = -1;
                              perMealList[k] = -1;
                              hungerList[k] = -1;
                              killerList[k] = -1;
                            }//get updated animal attributes every hour
                            terrain[i][j]->getAnimals(xList, yList, spdList, nameList, posList,
                              sizeList, atkList, defList, climbList, needsList,
                              eatenList, perMealList, hungerList, killerList);
                          }
                        }
                      }
                    }
                  } 
                }
                else if(temp7.compare(nameList[k]) == 0)//bear moves
                {
                  for(int a = 0; a < terrainSize; a++)
                  {
                    if(victimFound == true)
                      break;
                    for(int b = 0; b < terrainSize; b++)
                    {
                      if(victimFound == true)
                        break;
                      victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k], nameList[k],
                        killerList[k], atkList[k], xList[k], yList[k]);//search same way
                      if(victimPos == -1)
                        continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                      else
                        victimFound = true;
                      if(victimFound == true)//eat it
                      {
                        eatenSoFar = eatenList[k];
                        if(eatenSoFar < perMealList[k])
                        {
                          eatenAmount = animalEating("C", xList[k], yList[k], -1,
                            victimPos, -1, false, a, b);//a and b are the needed victim's tile's indexes
                        }
                        eatenSoFar += eatenAmount;//new added on old
                        if(eatenSoFar >= perMealList[k])//animal filled
                          filled = true;
                        terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                        checkDeadEntities();
                        for(int k = 0; k < animalCount; k++)
                        {//init
                          xList[k] = -1;
                          yList[k] = -1;
                          spdList[k] = -1;
                          posList[k] = -1;
                          sizeList[k] = -1;
                          atkList[k] = -1;
                          defList[k] = -1;
                          climbList[k] = -1;
                          needsList[k] = -1;
                          eatenList[k] = -1;
                          perMealList[k] = -1;
                          hungerList[k] = -1;
                          killerList[k] = -1;
                        }//get updated animal attributes every hour
                        terrain[i][j]->getAnimals(xList, yList, spdList, nameList, 
                           posList, sizeList, atkList, defList, climbList, needsList,
                           eatenList, perMealList, hungerList, killerList);
                      }
                    }
                  }
                  if(victimFound == false)//couldnt find anything have to move and look again
                  {
                    changeTile = animalMovement(xList[k], yList[k], newCord1, newCord2, "Bear");
                    if(changeTile == true)//animal move : check then eat
                    { 
                      terrain[i][j]->updateMove(newCord1, newCord2, posList[k]);
                      for(int a = 0; a < terrainSize; a++)
                      {
                        if(victimFound == true)
                          break;
                        for(int b = 0; b < terrainSize; b++)
                        {
                          if(victimFound == true)
                            break;
                          victimPos = terrain[a][b]->getVictims(sizeList[k], spdList[k], 
                            nameList[k], killerList[k], atkList[k], newCord1, newCord2);//search
                          if(victimPos == -1)
                            continue;//tile spawn list was empty or not same cords 2 checks or couldnt eat it
                          else
                            victimFound = true;
                          if(victimFound == true)//eat it
                          {
                            eatenSoFar = eatenList[k];
                            if(eatenSoFar < perMealList[k])
                            {
                              eatenAmount = animalEating("C", newCord1, newCord2, -1,
                                victimPos, -1, false, a, b);//a and b are the needed victim's tile's indexes
                            }
                            eatenSoFar += eatenAmount;//new added on old
                            if(eatenSoFar >= perMealList[k])//animal filled
                              filled = true;
                            terrain[i][j]->updateEat(filled, eatenAmount, posList[k]);
                            checkDeadEntities();
                            for(int k = 0; k < animalCount; k++)
                            {//init
                              xList[k] = -1;
                              yList[k] = -1;
                              spdList[k] = -1;
                              posList[k] = -1;
                              sizeList[k] = -1;
                              atkList[k] = -1;
                              defList[k] = -1;
                              climbList[k] = -1;
                              needsList[k] = -1;
                              eatenList[k] = -1;
                              perMealList[k] = -1;
                              hungerList[k] = -1;
                              killerList[k] = -1;
                            }//get updated animal attributes every hour
                            terrain[i][j]->getAnimals(xList, yList, spdList, nameList,
                              posList, sizeList, atkList, defList, climbList,
                              needsList, eatenList, perMealList, hungerList, killerList);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
            delete[] xList;
            delete[] yList;
            delete[] spdList;
            delete[] nameList;
            delete[] posList;
            delete[] defList;
            delete[] atkList;
            delete[] sizeList;
            delete[] needsList;
            delete[] climbList;
            delete[] perMealList;
            delete[] eatenList;
            delete[] hungerList;
            delete[] killerList;
            checkDeadEntities();
          }
        }
      }
      dayOfYear->progress();//update timeframe(hour,day,epoch)
      curHour++;//hour passed
      if(curHour == 24)
      {
        for(int m = 0; m < terrainSize; m++)
          for(int n = 0; n < terrainSize; n++)
            terrain[m][n]->updateHungry();
      }
      checkDeadEntities();//remove dead animals and plants from the map at end of day
    }
    animalBreeding(curDay);//animals breed if breeding day for them
    plantBreeding(curDay);//plants breed if breeding day for them
    curHour = 0;//initialise hour
    if((curDay % 90) == 0)//every 90 days season changes
    {
      curSeason = dayOfYear->getEpoch();//get updated epoch
      applySeason(curSeason);//apply it
      cout << endl;
      printSystem();//print terrain
      cout << endl;
      printPlantStatistics();//print plant stats
      cout << endl;
      printAnimalStatistics();//print animal stats
      cout << endl;
    }
    curDay++;//day passed
  }
}

void Map::dailyReset(const int & curDay)
{
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
    {
      terrain[i][j]->reset();//get hungry every day
      if(growthPeriodP != 0)//plants can grow normally
        if((curDay % growthPeriodP) == 0)//plant growth day
          terrain[i][j]->growPlant();
      if((curDay % growthPeriodA) == 0)//animal growth day
        terrain[i][j]->raiseAnimal();
    }
  }
}

bool Map::animalMovement(const int & curCord1, const int & curCord2, 
                        int & newCord1, int & newCord2, string key)
{
  int side = rand() % 4;
  string temp8 = "#";
  string temp9 = "^";
  string temp10 = "\"";
  string temp1 = "Salmon";
  string temp2 = "Rabbit";
  string temp3 = "Groundhog";
  string temp4 = "Deer";
  string temp5 = "Fox";
  string temp6 = "Wolf";
  string temp7 = "Bear";
  string tileType;
  int result = 0;
  if(side == 0)//check north first
  {
    if(curCord1 > 0)//stay in limits
    {
      tileType = terrain[curCord1-1][curCord2]->getToken();
      if(key.compare(temp1) == 0)//salmon
      {
        if(tileType.compare(temp8) == 0)//can move on water
          result = 1;
      }
      else if(key.compare(temp2) == 0)//Rabbit
      {
        if(tileType.compare(temp9) != 0)//cant move on mountain
          result = 1;
      }
      else if(key.compare(temp3) == 0)//Groundhog
      {
        if(tileType.compare(temp8) != 0)//cant move on mountain
          result = 1;
      }
      else if(key.compare(temp4) == 0)//Deer
        result = 1;//deers can move anywhere on map
      else if(key.compare(temp5) == 0)//Fox
        result = 1;//foxes can move anywhere on map
      else if(key.compare(temp6) == 0)//Wolf
        result = 1;//wolves can move anywhere on map
      else if(key.compare(temp7) == 0)//Bear
        result = 1;//bears can move anywhere on map
      side++;
      if(result == 1)
      {
        newCord1 = (curCord1 - 1);
        newCord2 = curCord2;
        return true;
      }
      else
        side++;
    }
    else
      side++;
  }
  if(side == 1)//check south first
  {
    if(curCord1 < (terrainSize - 1))//stay in limits
    {
      tileType = terrain[curCord1+1][curCord2]->getToken();
      if(key.compare(temp1) == 0)//salmon
      {
        if(tileType.compare(temp8) == 0)//can move on water
          result = 1;
      }
      else if(key.compare(temp2) == 0)//Rabbit
      {
        if(tileType.compare(temp9) != 0)//cant move on mountain
          result = 1;
      }
      else if(key.compare(temp3) == 0)//Groundhog
      {
        if(tileType.compare(temp8) != 0)//cant move on water
          result = 1;
      }
      else if(key.compare(temp4) == 0)//Deer
        result = 1;//deers can move anywhere on map
      else if(key.compare(temp5) == 0)//Fox
        result = 1;//foxes can move anywhere on map
      else if(key.compare(temp6) == 0)//Wolf
        result = 1;//wolves can move anywhere on map
      else if(key.compare(temp7) == 0)//Bear
        result = 1;//bears can move anywhere on map
      side++;
      if(result == 1)
      {
        newCord1 = (curCord1 + 1);
        newCord2 = curCord2;
        return true;
      }
      else
        side++;
    }
    else
      side++;
  }
  if(side == 2)//check west first
  {
    if(curCord2 > 0)//stay in limits
    {
      tileType = terrain[curCord1][curCord2-1]->getToken();
      if(key.compare(temp1) == 0)//salmon
      {
        if(tileType.compare(temp8) == 0)//can move on water
          result = 1;
      }
      else if(key.compare(temp2) == 0)//Rabbit
      {
        if(tileType.compare(temp9) != 0)//cant move on mountain
          result = 1;
      }
      else if(key.compare(temp3) == 0)//Groundhog
      {
        if(tileType.compare(temp8) != 0)//cant move on water
          result = 1;
      }
      else if(key.compare(temp4) == 0)//Deer
        result = 1;//deers can move anywhere on map
      else if(key.compare(temp5) == 0)//Fox
        result = 1;//foxes can move anywhere on map
      else if(key.compare(temp6) == 0)//Wolf
        result = 1;//wolves can move anywhere on map
      else if(key.compare(temp7) == 0)//Bear
        result = 1;//bears can move anywhere on map
      side++;
      if(result == 1)
      {
        newCord1 = curCord1;
        newCord2 = (curCord2 - 1);
        return true;
      }
      else
        side++;
    }
    else
      side++;
  }
  if(side == 3)//check east first
  {
    if(curCord2 < (terrainSize - 1))//stay in limits
    {
      tileType = terrain[curCord1][curCord2+1]->getToken();
      if(key.compare(temp1) == 0)//salmon
      {
        if(tileType.compare(temp8) == 0)//can move on water
          result = 1;
      }
      else if(key.compare(temp2) == 0)//Rabbit
      {
        if(tileType.compare(temp9) != 0)//cant move on mountain
          result = 1;
      }
      else if(key.compare(temp3) == 0)//Groundhog
      {
        if(tileType.compare(temp8) != 0)//cant move on water
          result = 1;
      }
      else if(key.compare(temp4) == 0)//Deer
        result = 1;//deers can move anywhere on map
      else if(key.compare(temp5) == 0)//Fox
        result = 1;//foxes can move anywhere on map
      else if(key.compare(temp6) == 0)//Wolf
        result = 1;//wolves can move anywhere on map
      else if(key.compare(temp7) == 0)//Bear
        result = 1;//bears can move anywhere on map
      side++;
      if(result == 1)
      {
        newCord1 = curCord1;
        newCord2 = (curCord2 + 1);
        return true;
      }
      else
        side++;
    }
  }
  return false;
}

int Map::animalEating(string type, const int & cord1, const int & cord2, const int & damage,
                       const int & victimPos, const int & climber, const bool & target,
                       const int & victimTile1, const int & victimTile2)
{
  string temp = "C";
  int foodEaten = 0;
  if(temp.compare(type) == 0)//carnivore
  {
    foodEaten = terrain[victimTile1][victimTile2]->eatAnimal(victimPos);
    foodEaten = 100;//just a big enough number to indicate that carnivores get full with just 1 meal.
  }
  else//herbivore
    foodEaten = terrain[cord1][cord2]->eatPlant(damage, climber, target);
  return foodEaten;
}

void Map::checkDeadEntities()
{
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
    {
      terrain[i][j]->removePlant();
      terrain[i][j]->removeAnimal();
    }
  }
}

void Map::animalBreeding(const int & curDay)
{
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
    {
      if((curDay % breedingRepPeriodC) == 0)//carnivore breed day
        terrain[i][j]->breedAnimal("C");
      if((curDay % breedingRepPeriodH) == 0)//herbivore breed day
        terrain[i][j]->breedAnimal("H");
    }
  }
}

void Map::plantBreeding(const int & curDay)
{
  bool result;
  string temp;
  double prob;
  double chance;
  int * newBornsX = new int[terrainSize*terrainSize];
  int * newBornsY = new int[terrainSize*terrainSize];
  int newBornPos = 0;
  int flag = 0;
  for(int i = 0; i < terrainSize*terrainSize; i++)
  {
    newBornsX[i] = -1;
    newBornsY[i] = -1;
  }
  if(breedingRepPeriodP != 0)//plants can breed
  {
    for(int i = 0; i < terrainSize; i++)
    {
      for(int j = 0; j < terrainSize; j++)
      {
        flag = 0;
        result = false;
        chance = ((double) rand() / (RAND_MAX));
        prob = terrain[i][j]->getBreedProb();
        if(chance > prob)//plant failed to breed
          continue;
        for(int k = 0; k < terrainSize*terrainSize; k++)
        {
          if(newBornsX[k] == -1)
            break;
          if((newBornsX[k] == i) && newBornsY[k] == j)
          {
            flag = 1;
            break;
          }
        }
        if(flag == 1)
          continue;
        if(((curDay % breedingRepPeriodP) == 0) && (terrain[i][j]->isAlive() == true))//plant breed day
        { 
          temp = terrain[i][j]->getPlantToken();
          if(j > 0)
          {
            result = terrain[i][j-1]->breedPlant(temp, i, j-1);
            if(result == true)
            {
              newBornsX[newBornPos] = i;
              newBornsY[newBornPos] = j - 1;
              newBornPos++;
            }
          }
          if((result == false) && (j < (terrainSize - 1)))
          {
            result = terrain[i][j+1]->breedPlant(temp, i, j+1);
            if(result == true)
            {
              newBornsX[newBornPos] = i;
              newBornsY[newBornPos] = j + 1;
              newBornPos++;
            }
          }
          if((result == false) && (i < (terrainSize - 1)))
          {
            result = terrain[i+1][j]->breedPlant(temp, i+1, j);
            if(result == true)
            {
              newBornsX[newBornPos] = i + 1;
              newBornsY[newBornPos] = j;
              newBornPos++;
            }
          }
          if((result == false) && (i > 0))
          {
            result = terrain[i-1][j]->breedPlant(temp, i-1, j);
            if(result == true)
            {
              newBornsX[newBornPos] = i - 1;
              newBornsY[newBornPos] = j;
              newBornPos++;
            }
          }
        }
      }
    }
  }
  delete[] newBornsX;
  delete[] newBornsY;
}


void Map::printAnimalStatistics() const
{
  int animalsCount = 0;
  int herbivoreCount = 0;
  int carnivoreCount = 0;
  int numSalmon = 0;
  int numRabbit = 0;
  int numGroundhog = 0;
  int numDeer = 0;
  int numFox = 0;
  int numWolf = 0;
  int numBear = 0;
  for(int i = 0; i < terrainSize; i++)
  {
    for(int j = 0; j < terrainSize; j++)
    {
      terrain[i][j]->countAnimals(numSalmon, numRabbit, numGroundhog, numDeer,
      numFox, numWolf, numBear);
    }
  }
  herbivoreCount = numSalmon + numRabbit + numGroundhog + numDeer;
  carnivoreCount = numFox + numWolf + numBear;
  animalsCount = herbivoreCount + carnivoreCount;
  cout << "Total Animals : " << animalsCount << endl;
  cout << "Total Herbivores : " << herbivoreCount << endl;
  cout << "Total Salmons : " << numSalmon << endl;
  cout << "Total Rabbits : " << numRabbit << endl;
  cout << "Total Groundhogs : " << numGroundhog << endl;
  cout << "Total Deers : " << numDeer << endl;
  cout << "Total Carnivores : " << carnivoreCount << endl;
  cout << "Total Foxes : " << numFox << endl;
  cout << "Total Wolves : " << numWolf << endl;
  cout << "Total Bears : " << numBear << endl;
}
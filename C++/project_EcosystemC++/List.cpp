/***************************************************************************
Implementation File : List.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates a list of tile's resident animals
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
****************************************************************************/
#include <cstdlib>
#include <iostream>
#include "List.h"
using namespace std;
/* FUNCTIONS OF NODE */
Node::Node(string nam = "Zebra", string key = "H", int siz = 10, int spd = 15,
  int hunger = 20, int eaten = 0, int perHour = 2, int crd1 = 36, int crd2 = 212,
  bool alive = true, bool hungry = true, bool heated = true,
  bool adult = true, bool hibern = false, bool sleeping = true,
  bool climber = false, int needs = 100)
{
  animalH = new Herbivore(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2, alive,
                         hungry, heated, adult, hibern, sleeping, climber, needs);
  animalC = NULL;
  //cout << "A new node has been created!\n";
}

Node::Node(string nam = "Tiger", string key = "C", int siz = 20, int spd = 20,
  int hunger = 30, int eaten = 0, int perHour = 10000, int crd1 = 0, int crd2 = 4,
  bool alive = true, bool hungry = true, bool heated = false, bool adult = false,
  bool hibern = false, bool sleeping = false, int atk = 30, int def = 20, bool mood = false)
{
  animalC = new Carnivore(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2, alive,
                         hungry, heated, adult, hibern, sleeping, atk, def, mood);
  animalH = NULL;
  //cout << "A new node has been created!\n";
}

Node::~Node()
{
  if(animalC != NULL)
    delete animalC;
  if(animalH != NULL)
    delete animalH;
  //cout << "Node destroyed...\n";
}

void Node::printNode() const
{
  if(animalC == NULL && animalH != NULL)
    cout << animalH->getName() << " ";
  if(animalH == NULL && animalC != NULL)
    cout << animalC->getName() << " ";
}

string Node::getToken() const
{
  if(animalC == NULL)
    return animalH->getToken();
  else
    return animalC->getToken();
}

string Node::getName() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getName();
  if(animalH == NULL && animalC != NULL)
    return animalC->getName();
  return "N/A";
}

int Node::getHungerCount() const
{
  if(animalC == NULL)
    return animalH->getHungerCount();
  else
    return animalC->getHungerCount();
}

bool Node::getHungry() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getHungry();
  if(animalH == NULL && animalC != NULL)
    return animalC->getHungry();
  return false;
}

void Node::setHungry(const bool & newHungry)
{
  if(animalC == NULL && animalH != NULL)
    animalH->setHungry(newHungry);
  if(animalH == NULL && animalC != NULL)
    animalC->setHungry(newHungry);
}

void Node::resetHunger()
{
  string key;
  if(animalH == NULL)
    key = animalC->getToken();
  else if(animalC == NULL)
    key = animalH->getToken();
  string temp = "C";
  if(key.compare(temp) == 0)//carnivore
    animalC->resetHunger();
  else//herbivore
  {
    int count = animalH->getHungerCount();
    if(count > 6)//herbivores get hungry every 7 days since last full meal
      animalH->resetHunger();
  }
}

void Node::raiseAnimal()
{
  int result = -1;
  if(animalC == NULL)
    result = animalH->raise();//return -1 on hibernating animals
  else if(animalH == NULL)
    result = animalC->raise();
  if(result == 0)//Carnivore raised
    animalC->growC();
  else if(result == 1)//Herbivore raised
    animalH->growH();
}

bool Node::getStatus() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getAlive();
  if(animalH == NULL && animalC != NULL)
    return animalC->getAlive();
  return false;
}

bool Node::getHeat() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getHeat();
  if(animalH == NULL && animalC != NULL)
    return animalC->getHeat();
  return false;
}

bool Node::getHibern() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getHibernate();
  if(animalH == NULL && animalC != NULL)
    return animalC->getHibernate();
  return true;
}

bool Node::getSleep() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getSleeping();
  if(animalH == NULL && animalC != NULL)
    return animalC->getSleeping();
  return true;
}

void Node::setSleep(const bool & newState)
{
  if(animalC == NULL && animalH != NULL)
    animalH->setSleeping(newState);
  if(animalH == NULL && animalC != NULL)
    animalC->setSleeping(newState);
}

int Node::getSpeed() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getSpeed();
  if(animalH == NULL && animalC != NULL)
    return animalC->getSpeed();
  return 0;
}

int Node::getCord1() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getX();
  if(animalH == NULL && animalC != NULL)
    return animalC->getX();
  return 0;
}

int Node::getCord2() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getY();
  if(animalH == NULL && animalC != NULL)
    return animalC->getY();
  return 0;
}

int Node::getSize() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getSize();
  if(animalH == NULL && animalC != NULL)
    return animalC->getSize();
  return 0;
}
int Node::getClimb() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getClimb();
  return 0;
}
int Node::getNeeds() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getNeeds();
  return 0;
}
int Node::getEaten() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getEaten();
  if(animalH == NULL && animalC != NULL)
    return animalC->getEaten();
  return 0;
}
int Node::getPerMeal() const
{
  if(animalC == NULL && animalH != NULL)
    return animalH->getPerMeal();
  if(animalH == NULL && animalC != NULL)
    return animalC->getPerMeal();
  return 0;
}

void Node::cordsUpdate(const int & newCord1, const int & newCord2)
{
  if(animalC == NULL && animalH != NULL)
    animalH->move(newCord1, newCord2);
  else if(animalH == NULL && animalC != NULL)
    animalC->move(newCord1, newCord2);
}

void Node::foodUpdate(const bool & filled, const int & eatenFood)
{
  if(animalC == NULL && animalH != NULL)
  {
    if(filled == true)
      animalH->eat(false, eatenFood);
    else
      animalH->eat(true, eatenFood);
  }
  else if(animalH == NULL && animalC != NULL)
  {
    if(filled == true)
      animalC->eat(false, eatenFood);
    else
      animalC->eat(true, eatenFood);
  }
}

void Node::setHungerCount(const int & newHc)
{
  if(animalC == NULL && animalH != NULL)
    animalH->setHungerCount(newHc);
  else if(animalH == NULL && animalC != NULL)
    animalC->setHungerCount(newHc);
}

void Node::hungerUpdate()
{
  int daysHungry;
  bool sleeping;
  if(animalC == NULL && animalH != NULL)
  {
    sleeping = getSleep();
    if(sleeping == true)//animal hibernates
      return;//no tampering with hunger count it remains stuck since animal slept winter's first day
    daysHungry = getHungerCount();
    if(getEaten() < getNeeds())//didnt feed enough
    {
      if(daysHungry == 16)//animal dies from starvation the 10th day(7 days counted as non-hungry)
        animalH->getKilled();
      else
      {
        daysHungry ++;
        setHungerCount(daysHungry);
      }
    }
    else//fed ok
    {
      daysHungry = 0;
      setHungerCount(daysHungry);
    }
    animalH->setEaten(0);
  }
  else if(animalH == NULL && animalC != NULL)
  {
    sleeping = getSleep();
    if(sleeping == true)//animal hibernates
      return;//no tampering with hunger count it remains stuck since animal slept winter's first day
    daysHungry = getHungerCount();
    if(getEaten() < getPerMeal())//didnt feed enough
    {
      if(daysHungry == 7)//turn into carnivore hunter the 8th day
        animalC->setKiller(true);
      if(daysHungry == 9)//animal dies from starvation the 10th day
        animalC->getKilled();
      else
      {
        daysHungry ++;
        setHungerCount(daysHungry);
      }
    }
    else//fed ok
    {
      daysHungry = 0;
      setHungerCount(daysHungry);
      animalC->setKiller(false);//relax and eat only herbivores
    }
    animalC->setEaten(0);
  }
}

bool Node::getKiller() const
{
  if(animalH == NULL && animalC != NULL)
    return animalC->getKiller();
  return false;
}

void Node::getKilled()
{
  if(animalH == NULL)
    animalC->getKilled();
  else
    animalH->getKilled();
}

int Node::getDefence() const
{
  if(animalH == NULL && animalC != NULL)
    return animalC->getDefence();
  return 1000;
}

int Node::getAttack() const
{
  if(animalH == NULL && animalC != NULL)
    return animalC->getAttack();
  return 0;
}
/* FUNCTIONS OF LIST */
List::List()
: head(NULL), size(0)
{
  //cout << "A new list has been constructed!\n";
}

List::~List()
{
  Node * cur = head;
  Node * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "List deleted succesfully!\n";
}

bool List::isEmpty()
{
  if(size > 0)
    return false;
  else
    return true;
}

void List::addNodeH(string nam = "Zebra", string key = "H", const int & siz = 10,
  const int & spd = 15, const int & hunger = 20, const int & eaten = 0, const int & perHour = 2,
  const int & crd1 = 36, const int & crd2 = 212, const bool & alive = true,
  const bool & hungry = true, const bool & heated = true, const bool & adult = true,
  const bool & hibern = false, const bool & sleeping = true,
  const bool & climber = false, const int & needs = 100)
{
  Node * node = new Node(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2, alive,
                        hungry, heated, adult, hibern, sleeping, climber, needs);
  node->next = head;
  head = node;
  size++;
}

void List::addNodeC(string nam = "Tiger", string key = "C", const int & siz = 10,
  const int & spd = 15, const int & hunger = 20, const int & eaten = 0, const int & perHour = 10000,
  const int & crd1 = 36, const int & crd2 = 212, const bool & alive = true,
  const bool & hungry = true, const bool & heated = true, const bool & adult = true,
  const bool & hibern = false, const bool & sleeping = true,
  const int & atk = 30, const int & def = 20, const bool & mood = false)
{
  Node * node = new Node(nam, key, siz, spd, hunger, eaten, perHour, crd1, crd2, alive,
                        hungry, heated, adult, hibern, sleeping, atk, def, mood);
  node->next = head;
  head = node;
  size++;
}

void List::delNode(const int & target)
{
  if(target == 0)
  {
    Node * temp = head;
    head = head->next;
    temp->next = NULL;
    delete temp;
    size--;
  }
  else
  {
    Node * temp = head;
    Node * cur = head;
    int curPos = 0;
    while(curPos != target)
    {
      temp = cur;
      cur = cur->next;
      curPos++;
    }
    temp->next = cur->next;
    cur->getKilled();
    cur->setHungry(false);
    delete cur;
    size--;
  }
}

void List::printList() const
{
  Node * cur = head;
  while(cur != NULL)
  {
    cur->printNode();
    cur = cur->next;
  }
}

void List::searchReset()
{
  if(isEmpty() == false)//animals spawned here still live
  {
    Node * cur = head;
    while(cur != NULL)
    {
      cur->resetHunger();
      cur = cur->next;
    }
  }
}

void List::searchRaise()
{
  if(isEmpty() == false)//animals spawned here still live
  {
    Node * cur = head;
    while(cur != NULL)
    {
      cur->raiseAnimal();
      cur = cur->next;
    }
  }
}

void List::searchDead(int * positions)
{
  if(isEmpty() == false)//animals spawned here still exist
  {
    int pos = 0;
    int target = 0;
    bool status;
    Node * cur = head;
    while(cur != NULL)
    {
      status = cur->getStatus();
      if(status == false)//dead
        positions[target++] = pos;
      cur = cur->next;
      pos++;
    }
  }
}

void List::getCords(int * xList, int * yList, int * spdList, string * nameList, int * posList,
                    int * sizeList, int * atkList, int * defList, int * climbList,
                    int * needsList, int * eatenList, int * perMealList, int * hungerList, int * killerList)
{
  if(isEmpty() == false)
  {
    int nodePos = 0;
    int pos = 0;
    Node * cur = head;
    while(cur != NULL)
    {
      bool hibernStatus = cur->getSleep();
      string key = cur->getName();
      bool lifeStatus = cur->getStatus();
      if((hibernStatus == false) && (lifeStatus == true))//animal isnt'in hibernation and is alive
      {//hibern animals are hidden burried in the map caves?? :P , no moving, no eating, no getting eaten
        xList[pos] = cur->getCord1();
        yList[pos] = cur->getCord2();
        spdList[pos] = cur->getSpeed();
        nameList[pos] = cur->getName();
        posList[pos] = nodePos;
        sizeList[pos] = cur->getSize();
        climbList[pos] = (int) cur->getClimb();
        needsList[pos] = cur->getNeeds();
        eatenList[pos] = cur->getEaten();
        perMealList[pos] = cur->getPerMeal();
        hungerList[pos] = (int) cur->getHungry();
        killerList[pos] = (int) cur->getKiller();
        atkList[pos] = cur->getAttack();
        defList[pos] = cur->getDefence();
        pos++;
      }
      cur = cur->next;
      nodePos ++;
    }
  }
}

void List::searchMoveUpdate(const int & newCord1, const int & newCord2, const int & nodePos)
{
  if(isEmpty() == false)
  {
    int curPos = 0;
    Node * cur = head;
    while(cur != NULL)
    {
      if(curPos == nodePos)//found the node
      {
        cur->cordsUpdate(newCord1, newCord2);
        break;
      }
      curPos++;
      cur = cur->next;
    }
  }
}

void List::searchEatUpdate(const bool & filled, const int & eatenFood, const int & nodePos)
{
  if(isEmpty() == false)
  {
    int curPos = 0;
    Node * cur = head;
    while(cur != NULL)
    {
      if(curPos == nodePos)//found the node
      {
        cur->foodUpdate(filled, eatenFood);
        break;
      }
      curPos++;
      cur = cur->next;
    }
  }
}

void List::searchHungry()
{
  if(isEmpty() == false)
  {
    Node * cur = head;
    while(cur != NULL)
    {
      cur->hungerUpdate();
      cur = cur->next;
    }
  }
}

int List::searchVictim(const int & hunterSize, const int & hunterSpd, string hunterName,
const bool & hunterMood, const int & hunterAtk, const int & hunterCord1, const int & hunterCord2)
{
  int victimPos = 0;
  Node * cur = head;
  int victimSpeed;
  int victimSize;
  int victimDef;
  int victimCord1;
  int victimCord2;
  bool victimStatus;
  bool victimSleeping;
  string victimName;
  string victimToken;
  string temp1 = "Fox";
  string temp2 = "Wolf";
  string temp3 = "Salmon";
  string temp4 = "Bear";
  string temp5 = "C";
  string temp6 = "H";
  bool huntResult = false;
  if(isEmpty() == false)
  {
    while(cur != NULL)
    {
      victimCord1 = cur->getCord1();
      victimCord2 = cur->getCord2();
      victimStatus = cur->getStatus();
      victimSleeping = cur->getSleep();
      if((victimStatus == true) && (victimSleeping == false) && //victim is alive awaken and at same tile
      ((victimCord1 == hunterCord1) && (victimCord2 == hunterCord2)))
      {
        if(hunterMood == false)//just eating herbivores
        {
          victimSpeed = cur->getSpeed();
          victimSize = cur->getSize();
          victimName = cur->getName();
          victimToken = cur->getToken();
          if((hunterName.compare(temp1) == 0) != (hunterName.compare(temp2) == 0))//fox or wolf
          {
            if((victimName.compare(temp3) != 0 ) && (victimName.compare(temp4) != 0) && 
            (victimName.compare(temp2) != 0) && (victimName.compare(temp1) != 0))//salmon and carnivores skipped
            {
              
              if((victimSize <= hunterSize) && (victimSpeed < hunterSpd))
              {
                huntResult = true;
                break;
              }
            }
          }
          if(hunterName.compare(temp4) == 0)//bear hunter eats every herbivore
          {
            if((victimName.compare(temp4) != 0) && (victimName.compare(temp2) != 0) &&
            (victimName.compare(temp1) != 0))//carnivores skipped
            {
              if(victimSize <= hunterSize)//bear gives insta kill regardless speed
              {
                huntResult = true;
                break;
              }
            }
          }
        }
        if(hunterMood == true)//eating carnivores too // cannibalism disabled
        {//first size check then atk vs def check
          victimToken = cur->getToken();
          if(victimToken.compare(temp6) == 0)//herbivore victim
          {
            victimSpeed = cur->getSpeed();
            victimSize = cur->getSize();
            victimName = cur->getName();
            if((hunterName.compare(temp1) == 0) || (hunterName.compare(temp2) == 0))//fox or wolf
            {
              if(victimName.compare(temp3) != 0)//salmon skipped
              {
                if((victimSize <= hunterSize) && (victimSpeed < hunterSpd))
                {
                  huntResult = true;
                  break;
                }
              }
            }
            if(hunterName.compare(temp4) == 0)//bear hunter eats everything 
            {
              if(victimSize <= hunterSize)//bear gives insta kill regardless speed
              {
                huntResult = true;
                break;
              }
            }
          }
          if(victimToken.compare(temp5) == 0)//carnivore victim
          {
            victimSize = cur->getSize();
            victimDef = cur->getDefence();
            victimName = cur->getName();
            if(victimName.compare(hunterName) != 0) //carnivore victim != hunter (no cannibalism enabled)
            {
              if(hunterName.compare(temp4) == 0)//hunter is bear
              {
                if(hunterSize == 10)//adult bear = insta kill unless adult bear
                {
                  huntResult = true;
                  break;
                }
                if(hunterSize < 10)//young bear gets in fight
                {
                  if(victimSize < hunterSize)//victim is smaller = gets eaten
                  {
                    huntResult = true;
                    break;
                  }
                  if(victimSize == hunterSize)//same size check defence and attack result
                  {
                    if(hunterAtk > victimDef)
                    {
                      huntResult = true;
                      break;
                    }
                  }
                }
              }
              if((hunterName.compare(temp1) == 0) || (hunterName.compare(temp2) == 0))//hunter is fox or wolf
              {
                if(victimSize < hunterSize)//victim is smaller = gets eaten
                {
                  huntResult = true;
                  break;
                }
                if(victimSize == hunterSize)//same size check defence and attack result
                {
                  if(hunterAtk > victimDef)
                  {
                    huntResult = true;
                    break;
                  }
                }
              }
            }
          }
        }
      }
      cur = cur->next;
      victimPos++;
    }
  }
  if(huntResult == true)
    return victimPos;
  else
    return -1;//failed
}

int List::searchHunted(const int & victimPos)
{
  Node * cur = head;
  int curPos = 0;
  int foodGained = 0;
  while(cur != NULL)
  {
    if(curPos == victimPos)//found victim
    {
      foodGained = cur->getSize();//gain food based on size
      cur->getKilled();
      break;
    }
    cur = cur->next;
    curPos ++;
  }
  return foodGained;
}

int List::searchHeated(string token)
{
  Node * cur = head;
  bool breedingState;
  string temp;
  string name;
  string key  = "C";
  string temp1 = "Salmon";
  string temp2 = "Rabbit";
  string temp3 = "Groundhog";
  string temp4 = "Deer";
  string temp5 = "Fox";
  string temp6 = "Wolf";
  string temp7 = "Bear";
  int parCord1;
  int parCord2;
  int newAnimals = 0;
  bool sleeping;
  while(cur != NULL)
  {
    sleeping = cur->getSleep();
    if(sleeping == false)//hibernating animals are lazy to breed
    {
      temp = cur->getToken();
      if(temp.compare(token) == 0)
      {
        breedingState = cur->getHeat();
        if(breedingState == true)
        {
          name = cur->getName();
          parCord1 = cur->getCord1();
          parCord2 = cur->getCord2();
          if(key.compare(token) == 0)//Carnivore//
          {
            newAnimals++;
            if(name.compare(temp5) == 0)//Fox parent
            {
              addNodeC("Fox", "C", 1, 1, 0, 0, 2, parCord1, parCord2,
              true, false, false, false, false, false, 1, 1, false);
            }
            else if(name.compare(temp6) == 0)//Wolf parent
            {
              addNodeC("Wolf", "C", 1, 2, 0, 0, 2, parCord1, parCord2,
              true, false, false, false, false, false, 2, 2, false);
            }
            else if(name.compare(temp7) == 0)//Bear parent
            {
              addNodeC("Bear", "C", 3, 4, 0, 0, 5, parCord1, parCord2,
              true, false, false, false, true, false, 6, 6, false);
            }
          }
          else//herbivore
          {
            newAnimals++;
            if(name.compare(temp1) == 0)//Salmon parent
            {
              addNodeH("Salmon", "H", 1, 5, 0, 0, 1, parCord1, parCord2,
              true, true, false, false, false, false, false, 1);
            }
            else if(name.compare(temp2) == 0)//Rabbit parent
            {
              addNodeH("Rabbit", "H", 1, 2, 0, 0, 1, parCord1, parCord2,
              true, true, false, false, false, false, false, 2);
            }
            else if(name.compare(temp3) == 0)//Groundhog parent
            {
              addNodeH("Groundhog", "H", 2, 3, 0, 0, 1, parCord1, parCord2,
              true, true, false, false, true, false, true, 5);
            }
            else if(name.compare(temp4) == 0)//Deer parent
            {
              addNodeH("Deer", "H", 2, 4, 0, 0, 2, parCord1, parCord2,
              true, true, false, false, false, false, false, 4); 
            }
          }
        }
      } 
    }
    cur = cur->next;
  }
  return newAnimals;
}

void List::setSleeping()
{
  Node * cur = head;
  bool status;
  while(cur != NULL)
  {
    status = cur->getHibern();
    if(status == true)//animal hibernates during winter
      cur->setSleep(true);
    cur = cur->next;
  }
}

void List::setAwaken()
{
  Node * cur = head;
  bool status;
  while(cur != NULL)
  {
    status = cur->getHibern();
    if(status == true)//animal wakes up during spring
      cur->setSleep(false);
    cur = cur->next;
  }
}

void List::searchTotal(int & numSalmon, int & numRabbit, int & numGroundhog, int & numDeer,
                       int & numFox, int & numWolf, int & numBear) const
{
  Node * cur = head;
  bool status;
  string name;
  string temp1 = "Salmon";
  string temp2 = "Rabbit";
  string temp3 = "Groundhog";
  string temp4 = "Deer";
  string temp5 = "Fox";
  string temp6 = "Wolf";
  string temp7 = "Bear";
  while(cur != NULL)
  {
    status = cur->getStatus();
    if(status == true)//animal is alive
    {
      name = cur->getName();
      if(name.compare(temp1) == 0)//Salmon
        numSalmon++;
      else if(name.compare(temp2) == 0)//Rabbit
        numRabbit++;
      else if(name.compare(temp3) == 0)//Groundhog
        numGroundhog++;
      else if(name.compare(temp4) == 0)//Deer
        numDeer++;
      else if(name.compare(temp5) == 0)//Fox
        numFox++;
      else if(name.compare(temp6) == 0)//Wolf
        numWolf++;
      else if(name.compare(temp7) == 0)//Bear
        numBear++;
    }
    cur = cur->next;
  }
}
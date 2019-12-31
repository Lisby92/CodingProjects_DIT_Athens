/*************************************************************************
Implementation File : List.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of List.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
**************************************************************************/
#ifndef __List__
#define __List__

#include <string>

#include "Animal.h"

using namespace std;
//a node of the list containing an animal
class Node
{
  private:
    Herbivore * animalH;//the herbivore animal the node represents
    Carnivore * animalC;//the carnivore animal the node represents
  public:
    Node * next;//pointer to the next node
    Node(string, string, int, int, int, int, int, int, int,
        bool, bool, bool, bool, bool, bool, bool, int);//herbivore constructor
    Node(string, string, int, int, int, int, int, int, int,
        bool, bool, bool, bool, bool, bool, int, int, bool);//carnivore constructor
    ~Node();//destructor
    void printNode() const; // unused
    string getToken() const;//get animal token
    string getName() const;//get animal name
    void resetHunger();//reset animal hunger
    bool getHungry() const;//check if animal is hungry
    void raiseAnimal();//animal grows
    int getHungerCount() const;//get days starving
    int getCord1() const;//get horizontal cord
    int getCord2() const;//get vertical cord
    bool getStatus() const;//check if animal is alive
    bool getHibern() const;//check if animal hibernates during winter
    int getSpeed() const;//get animal speed
    int getSize() const;//get animal size
    int getClimb() const;//check if animal can climb
    int getNeeds() const;//get animal meal needs
    int getEaten() const; //get eaten quantity so far
    int getPerMeal() const; //get damage per hour for herbivores or meal info for carnivores
    void cordsUpdate(const int &, const int &);//update animalslocation after moving
    void foodUpdate(const bool &, const int &);//update animal's feeding variables after meal
    void setHungerCount(const int &);//update animal's days starvating
    bool getKiller() const;//check if carnivore hunts carnivores too
    void hungerUpdate();//update hunger status
    void getKilled();//animal got hunted down or died from starvation
    int getDefence() const;//get defence of carnivore
    int getAttack() const;//get attack of carnivore
    void setHungry(const bool &);//set the animal hungry
    bool getSleep() const;//check if animal is currently hibernating
    void setSleep(const bool &);//set animal sleeping or awaken
    bool getHeat() const;//check if animal is adult and can breed
};
//a dynamic single linked list of animal nodes implemented as stack
//new elements are placed in the list head
class List
{
  private:
    Node * head;//the first node in the chain
    int size;//number of nodes in the list

  public:
    List();//constructor
    ~List();//destructor
    bool isEmpty();//check if empty
    void addNodeH(string, string, const int &, const int &, const int &, const int &, const int &, 
                 const int &, const int &, const bool &, const bool &, const bool &, const bool &,
                 const bool &, const bool &, const bool &, const int &);//adds a node for herbivore
    void addNodeC(string, string, const int &, const int &, const int &, const int &, const int &, 
                 const int &, const int &, const bool &, const bool &, const bool &, const bool &,
                 const bool &, const bool &, const int &, const int &, const bool &);//adds a node for carnivore
    void delNode(const int &);//deletes a node
    void printList() const;//prints the list's nodes // unused
    void searchReset();//search nodes to reset hunger
    void searchRaise();//search nodes to grow animals
    void searchDead(int *);//search dead animals to be removed
    void searchMoveUpdate(const int &, const int &, const int &);//search specific animal to update its cords
    void searchEatUpdate(const bool &, const int &, const int &);//search specific animal to update it's feeding variables
    void getCords(int *, int *, int *, string *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);//get data during sim
    void searchHungry();//search for hungry animals
    int searchVictim(const int &, const int &, string, const bool &, const int &, const int &, const int &);//search for prey during carnivore hunting
    int searchHunted(const int &);//search the successfully hunted animal
    int searchHeated(string);//search animals that can breed
    void setSleeping();//sleep 1st day of winter till end
    void setAwaken();//awake 1st day of spring
    void searchTotal(int &, int &, int &, int &, int &, int &, int &) const;//search count stats for printing
};

#endif
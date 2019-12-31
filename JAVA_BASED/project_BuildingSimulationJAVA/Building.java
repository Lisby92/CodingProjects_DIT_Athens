/********************************************************************************************
Implementation File : Building.java
Author Date     : V. Mavrommatis 17/12/2018
Purpose       : Implements a building
Revision      : < Vasileios_Mavrommatis - 18/12/2018 >
*********************************************************************************************/

public class Building extends Space
{ //subclass of Space
  /*data members*/
  int nfCap;//floor max capacity
  int ngCap;//ground floor max capacity
  int noCap;//office max capacity
  int nlCap;//lift max capacity
  Elevator lift;//the building's lift
  Ground groundFloor;//the building's ground floor
  Floor[] floors;//the buildng's floors
  /*constructor*/
  Building(int cur, int nn, int nnf, int nng, int nno, int nnl)
  {
    super(cur, nn);//create the space
    this.nfCap = nnf;
    this.ngCap = nng;
    this.noCap = nno;
    this.nlCap = nnl;
    lift = new Elevator(0, nnl);//create the elevator
    groundFloor = new Ground(0, nng);//create the ground floor
    floors = new Floor[4];
    for(int i = 1; i <= 4; i++)//create the 4 floors
      floors[i-1] = new Floor(0, nnf, nno, i);
    System.out.println("Building:: A new building is ready for serving citizens!\n");
  }
  /*functions*/
  //get current (space) capacity
  public int getCap()
  {
    return super.getCur();
  }
  //set current (space) capacity
  public void setCap(int cCap)
  {
    super.setCur(cCap);
  }
  //get max (space) capacity
  public int getMax()
  {
    return super.getMax();
  }
  //citizens from outside the building try enter
  public int enter(Citizen next)
  {
    int cCap = getCap();//get current capacity
    int mCap = getMax();//get max capacity

    if(next.isAlone())//alone
    {
      if(cCap >= mCap)//no room to get in
      {
        System.out.println("Building:: Please, come tomorrow!");
        return 0;
      }
      else//there is room
      {
        cCap++;
        setCap(cCap);
        System.out.println("Building:: Citizen got in!");
        return 1;
      }
    }
    else//with escort
    {
      if((cCap + 1) < mCap)//there is room for both
      {
        cCap += 2;
        setCap(cCap);
        System.out.println("Building:: 2 Citizens got in!");
        return 2;
      }
      else//no room for both
      {
        System.out.println("Building:: Not enough room for both. Please, come tomorrow!");
        return 0;
      }
    }
  }
  //citizens that entered the building try get in the ground floor
  public int groundEnter(Citizen next)
  {
    int cCap = groundFloor.getCap();//get ground floor current capacity
    int mCap = groundFloor.getMax();//get ground floor max capacity
    int bCap = getCap();//get building current capacity
    
    if(next.isAlone())//alone
    {
      if(cCap >= mCap)//ground floor full
      {
        System.out.println("Building:: Ground floor full, plz wait!");
        return 0;
      }
      else//can fit in
      {
        bCap--;
        setCap(bCap);//update building cap
        cCap++;
        groundFloor.setCap(cCap);//update ground floor cap
        System.out.println("Building:: Citizen got in Ground floor!");
        groundFloor.enter();//try get in waiting area
        groundFloor.wait(next);//get priority ticket
        return 1;
      }
    }
    else//has escort
    {
      if((cCap + 1) < mCap)//there is room for both
      {
        bCap -= 2;
        setCap(bCap);//update building cap
        cCap += 2;
        groundFloor.setCap(cCap);//update ground floor cap
        System.out.println("Building:: 2 Citizens got in Ground floor!");
        groundFloor.enter();//try get in waiting area
        groundFloor.enter();//enter escort
        groundFloor.wait(next);//get priority ticket
        groundFloor.waitEscort();//sit escort
        return 2;
      }
      else//not enough room for both
      {
        System.out.print("Building:: Not enough room for both. Ground floor full, plz wait!\n");
        return 0;
      }
    }
  }
  //exits citizens and escorts who have finished 
  public void exit(int[] donePassengers)
  {
    for(int i = 0; i < this.nlCap; i++)
    {
      if(donePassengers[i] > -1)
      {
        System.out.print("Building:: Citizen happily exiting the building!\n");
        if(donePassengers[i] != 0)//citizen
          System.out.print("Citizen:: I cannot believe I finished with priority just: " + donePassengers[i] + " !\n");
        else
          System.out.print("Escort:: Finally we're done! That took forever!\n");  
        donePassengers[i] = -1;
      }
    }
  }
  //core function simulating the actions taken inside the building from ground floor till exit
  public void operate(Citizen[] citizens, int[] doneCitizens, int created)
  {
    //cycle starts
    int liftSize = lift.getMax();
    int liftCap;
    int[] passengerList = new int[liftSize];//current list of passengers
    int[] droppedList = new int[liftSize];//list of passengers trying to enter a floor
    int[] exitList = new int[liftSize];//list of passengers who finished with their service
    int[] waiting = new int[liftSize];//list of passengers who failed to enter a full floor and wait for the next cycle
    int[] groundList = new int[this.ngCap];//list of passengers waiting for the lift
    for(int i = 0; i < liftSize; i++)//-1 = empty spot on each list
    {
      droppedList[i] = -1;
      exitList[i] = -1;
      waiting[i] = -1;
      passengerList[i] = -1;
    }
    for(int i = 0; i < this.ngCap; i++)//initialised @ -1
      groundList[i] = -1;
    lift.getOnboard(passengerList);//get current passenger list
    int groundWaitCap = groundFloor.getEntryCap();//get number of waiting for lift
    groundFloor.getList(groundList);
    int min;
    int pos;
    int result = 0;
    int boarded = 0;
    int count = 0;
    boolean escorted;
    boolean alone;
    int spot = 0;
    int prio = 0;
    int prev = -2;
    for(int i = 0; i < groundWaitCap; i++)
    {
      liftCap = lift.getCap();
      min = Integer.MAX_VALUE;//find minimum waiting in ground
      pos = -1;
      for(int j = 0; j < this.ngCap; j++)
      {
        if(groundList[j] > 0 && groundList[j] < min)//avoid empty and escorts
        {
          if(groundList[j] > prev)
          {
            min = groundList[j];
            pos = j;
          }
        }
      }
      prev = min;
      if(pos == -1)
        break;
      for(int k = 0; k < created; k++)//find citizen position in array
      {
        prio = citizens[k].getPriority();
        if(prio == min)
        {
          spot = k;
          break;
        }
      }
      if(citizens[spot].isAlone())
        result = lift.enter(citizens[spot], passengerList);
      else
      {
        if((liftCap + 1) < liftSize)
        {
          result = lift.enter(citizens[spot], passengerList);//enter citizen
          result += lift.enter(citizens[spot+1], passengerList);//enter escort
        }
        else
        {
          System.out.print("Elevator:: You are not allowed to enter " + prio + " and escort!\n");
          groundList[pos] = -1;//erase from waiting either way to skip his lower priority
          continue;
        }
      }
      if(result == 1)//passenger boarded
        groundFloor.remove(min);
      else if(result == 2)
      {
        groundFloor.remove(min);
        groundFloor.removeEscort();
      }
      boarded += result;
      groundList[pos] = -1;//erase from temp waiting either way to skip his lower priority
    }
    groundWaitCap-= boarded;
    groundFloor.exit(boarded);//they on lift now
    groundFloor.setEntryCap(groundWaitCap);//entrance update
    lift.setOnboard(passengerList);//update passenger list
    lift.print(0, 1);
    int floorCap;
    int dropped = 0;
    for(int i = 1; i <= 4; i++)
    {
      floorCap = floors[i-1].getEntryCap();//get floor cap
      dropped = lift.stopUp(citizens, passengerList, droppedList, i, floorCap, this.nfCap, created);//stop at next floor and attempt to unload
      lift.setOnboard(passengerList);//update list
      floors[i-1].enter(citizens, droppedList, this.nlCap, created);//enter on stop up
      lift.print(i, 1);
      lift.exit(dropped);//set elevator new cap based on dropped passengers both waiting in floor area and inside office
    }
    System.out.print("\n");
    for(int i = 0; i < this.nlCap; i++)//find the passengers that have to wait the next cycle to enter their floors/offices
      waiting[i] = passengerList[i];
    lift.setOnboard(waiting);//update with the passengers left
    int cap;
    int curFloor;
    for(int i = 4; i >= 1; i--)
    {  
      liftCap = lift.getCap();//get lift capacity
      cap = this.nlCap - liftCap;//number of seats available to board descending passengers who are done
      curFloor = i - 1;
      result = floors[i-1].exit(cap, curFloor, exitList, this.nlCap, citizens, created);//find exit targets and stop down
      lift.stopDown(passengerList, exitList);//at floor i
      lift.setOnboard(passengerList);//update based on boarded happy citizens
      lift.print(i, -1);
      liftCap += result;//count number of done passengers
      lift.setCap(liftCap);//update lift capacity for next stop
    }
    result = lift.emptyAll(waiting, passengerList, doneCitizens);
    lift.setOnboard(waiting);//update list with the ones that will be inside in the next cycle as priority passengers
    lift.exit(result);
    lift.print(0, 1);
    //cycle ends
  }
}
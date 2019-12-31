/*************************************************************************
Implementation File : Elevator.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's the building's elevator
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Elevator extends Space
{ //subclass of Space
  /*data members*/
  int[] onboard;
  /*constructor*/
  Elevator(int cur, int max)
  {
    super(cur, max);//create the space
    this.onboard = new int[max];//initialise list of priorities to -1
    for(int i = 0; i < max; i++)
      this.onboard[i] = -1;
    System.out.println("Elevator:: A Lift has been created!");
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
  //get list of onboard passengers
  public void getOnboard(int[] list)
  {
    int liftSize = getMax();
    for(int i = 0; i < liftSize; i++)
      list[i] = this.onboard[i];
  }
  //set list of onboard passengers
  public void setOnboard(int[] list)
  {
    int liftSize = getMax();
    for(int i = 0; i < liftSize; i++)
      this.onboard[i] = list[i];
  }
  //print current list of passengers
  public void print(int floor, int direction)
  {
    int liftSize = getMax();
    System.out.print("Elevator:: Current Passengers : [");
    for(int i = 0; i < liftSize; i++)
    {
      System.out.print(this.onboard[i]);
      if(i < (liftSize - 1))
        System.out.print(", ");
    }
    System.out.print("] at Floor[" + floor + "]");
    if(direction == 1)
    {
      if(floor < 4)
        System.out.print(", lift going up!\n");
      else
        System.out.print(", lift reached the top floor!\n");
    } 
    else if(direction == -1)
      if(floor > 0)
        System.out.print(", lift going down!\n");
  }
  //citizens and escorts (who got ticket) from ground floor try enter the lift
  public int enter(Citizen nextPassenger, int[] passengerList)
  {
    int cCap = getCap();
    int mCap = getMax();
    int priority = nextPassenger.getPriority();
    if(cCap >= mCap)
    {
      System.out.print("Elevator:: You are not allowed to enter " + priority + "!\n");
      return 0;
    }
    else
    {
      System.out.print("Entrance:: Citizen has exited the ground waiting area and is boarding the lift!\n");
      for(int i = 0; i < mCap; i++)
      {
        if(passengerList[i] == -1)//sit here plz
        {
          passengerList[i] = priority;
          break;
        }
      }
      cCap++;
      setCap(cCap);
      if(priority != 0)//non-escort citizen
        System.out.print("Elevator:: Visitor in the lift with priority number: " + priority + "!\n");
      else//escort
        System.out.print("Elevator:: Escort in the lift!\n");
      return 1;
    }
  }
  //updates lift's capacity after exiting done citizens
  public void exit(int exited)
  {
    int cCap = getCap();
    cCap -= exited;
    setCap(cCap);
  }
  //stop up at next floor and if there is enough space , unload them
  public int stopUp(Citizen[] citizens, int[] passengerList, int[] droppedList, int floor, int floorCap, int floorMaxCap, int created)
  {
    int min;
    int pos;
    int priority;
    int drops = 0;
    int dest;
    int mCap = getMax();
    int on = 0;
    int spot = 0;
    int prio = 0;
    int prev = -2;

    for(int i = 0; i < mCap; i++)
      if(this.onboard[i] > 0)//full seat, avoid escorts
        on++;//count passengers on as sets with their escorts or no
        
    while(on > 0)//try unload next passenger based on priority
    {
      min = Integer.MAX_VALUE;
      pos = -1;
      on--;
      for(int i = 0; i < mCap; i++)//find minimum priority
      {
        if(this.onboard[i] > 0 && this.onboard[i] < min)//full seat, avoid escorts
        {
          if(this.onboard[i] > prev)
          {
            min = this.onboard[i];
            pos = i;
          } 
        }
      }
      prev = min;
      if(pos == -1)//lift empty
        return drops;
      for(int i = 0; i < created; i++)//find citizen position in array
      {
        prio = citizens[i].getPriority();
        if(prio == min)
        {
          spot = i;
          break;
        }
      }

      dest = citizens[spot].getFloor();
        if(dest != floor)//not my floor
          continue;
      if(citizens[spot].isAlone())//is alone
      {
        if(floorCap < floorMaxCap)//there is room
        {
          for(int i = 0; i < mCap; i++)
          {
            if(passengerList[i] == prio)//found him on passenger list
            {
              droppedList[drops] = prio;
              drops++;
              passengerList[i] = -1;
              System.out.print("Citizen:: " + prio + " Entering floor number[" + floor + "] !\n");
              floorCap++;
              break;
            }
          }
        }
        else//floor is full
          System.out.print("Elevator:: Sorry, priority " + prio + ", floor number[" + floor + "] is full !\n");
      }
      else//has escort
      {
        if((floorCap + 1) < floorMaxCap)//there is room
        {
          for(int i = 0; i < mCap; i++)
          {
            if(passengerList[i] == prio)//found him on passenger list
            {
              droppedList[drops++] = prio;//citizen
              droppedList[drops++] = 0;//escort next to citizen
              passengerList[i] = -1;
              for(int j = 0; j < mCap; j++)
              {
                if(passengerList[j] == 0)//escort sit
                {
                  passengerList[j] = -1;//empty escort
                  break;
                }
              }
              System.out.print("Citizen:: " + prio + " entering floor number[" + floor + "] !\n");
              System.out.print("Escort:: Entering floor number[" + floor + "] !\n");
              floorCap += 2;
              break;
            }
          }  
        }
        else//no room for both
          System.out.print("Elevator:: Sorry, priority " + prio + " and Escort, floor number[" + floor + "] is full !\n");
      }
    }
    return drops;
  }
  //stop down at next floor and update passenger list based on exit needs
  public void stopDown(int[] passengerList, int[] exitList)
  {
    int mCap = getMax();
    for(int i = 0; i < mCap; i++)
    {
      if(passengerList[i] == -1)//empty elevator seat
      {
        for(int j = 0; j < mCap; j++)
        {
          if(exitList[j] != -1)//valid exit target
          {
            passengerList[i] = exitList[j];
            exitList[j] = -1;
            break;
          }
        }
      }
    }
  }
  //empties done citizens and returns the amount
  public int emptyAll(int[] waiting, int[] passengerList, int[] exitList)
  {
    int total = 0;
    int flag;
    int target;
    int mCap = getMax();

    for(int i = 0; i < mCap; i++)
    {
      if(waiting[i] == -1)//empty
      {
        exitList[i] = passengerList[i];
        passengerList[i] = -1;
        total++;
      }
    }
    return total;
  }
}
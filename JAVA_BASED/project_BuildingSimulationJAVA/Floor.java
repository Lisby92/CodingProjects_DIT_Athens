/*************************************************************************
Implementation File : Floor.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implements a building's floor
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Floor extends Space
{ //subclass of Space
  /*data members*/
  int noCap;
  int level;
  int[] list;
  EntryArea floorEntry;
  Office[] offices;
  /*constructor*/
  Floor(int cur, int max)//used to create a ground floor w/o offices
  {
    super(cur, max);//create the space
    this.noCap = 0;//no offices
    this.level = 0;//ground level
    this.list = null;//no waiting
    this.floorEntry = new EntryArea(cur, max);//create the entrance
    this.offices = null;//no offices here
    System.out.println("Floor[" + this.level + "]:: A Floor has been created "
                       + "with number [" + this.level + "] !");
  }
  /*constructor*/
  Floor(int cur, int max, int no, int lvl)//used to create a floor w offices
  {
    super(cur, max);//create the space
    this.noCap = no;//office cap
    this.level = lvl;//floor's level
    this.list = new int[max];//waiting list
    for(int i = 0; i < max; i++)//initialise @ empty
      this.list[i] = -1;
    this.floorEntry = new EntryArea(cur, max);//create the entrance  
    this.offices = new Office[10];//create the 10 offices
    for(int i = 1; i <= 10; i++)
      this.offices[i-1] = new Office(0, no, i);
    System.out.println("Floor[" + this.level + "]:: A Floor has been created "
                       + "with number [" + this.level + "]! ");  
  }
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
  //get current (Entry Area) capacity
  public int getEntryCap()
  {
    return floorEntry.getCap();
  }
  //set current (Entry Area) capacity
  public void setEntryCap(int cCap)
  {
    floorEntry.setCap(cCap);
  }
  //citizens from inside the lift attempt to enter the floor they need
  public void enter(Citizen[] citizens, int[] droppedList, int liftSize, int created)
  {
    int dest;
    int cCap;
    int min;
    int result;
    int priority;
    int floor;
    int counter = 0;
    int on = 0;
    int pos;
    int mCap = getMax();
    int prev = -2;
    int spot = 0;
    int prio = 0;
    int officeCap;
    //part 1
    for(int i = 0; i < mCap; i++)
      if(list[i] > 0)
        counter++;  
    while(counter > 0)//if people waiting already they surely have lower priority than any new
    {
      counter--;
      min = Integer.MAX_VALUE;
      pos = -1;
      for(int i = 0; i < mCap; i++)//find next minimum
      {
        if(list[i] < min && list[i] > 0)
        {
          if(list[i] > prev)
          {
            min = list[i];
            pos = i;
          }
        }
      }
      if(pos == -1)
        break;
      prev = min;
      for(int i = 0; i < created; i++)//find citizen position in array
      {
        prio = citizens[i].getPriority();
        if(prio == min)
        {
          spot = i;
          break;
        }
      }
      dest = citizens[spot].getOffice();
      floor = citizens[spot].getFloor();
      for(int i = 1; i < 11; i++)
      {
        if(floor != this.level)//not my floor
          break;
        else if(dest != i)//not my office
          continue;
        else //found my office
        {
          cCap = getEntryCap();
          if(citizens[spot].isAlone())//is alone
          {
            result = this.offices[i-1].enter(citizens[spot]);//attempt to enter
            if(result == 1)//i got in
            {
              cCap--;
              setEntryCap(cCap);
              list[pos] = -1;
              break;
            }
          }
          else//we are two
          {
            officeCap = offices[i-1].getCap();//get office current cap
            if((officeCap + 1) < this.noCap)//there is room
            {
              result = this.offices[i-1].enter(citizens[spot]);
              result = this.offices[i-1].enter(citizens[spot+1]);
              cCap -= 2;
              setEntryCap(cCap);
              list[pos] = -1;//empty citizen
              for(int j = 0; j < mCap; j++)
              {
                if(list[j] == 0)
                {
                  list[j] = -1;//empty escort
                  break;
                }
              }
              break;
            }
            else
            {
              System.out.print("Floor[" + this.level + 
                "]:: Please, wait outside for entrance in office number["
               + i + "], priority " + prio + " and Escort!\n");
            }
          }
        }
      }
    }
    prev = -2;
    //part 2 open the lift door to unload fresh
    for(int i = 0; i < liftSize; i++)
      if(droppedList[i] > 0)
        on++;  
    while(on > 0)
    {
      on--;
      min = Integer.MAX_VALUE;
      pos = -1;
      for(int i = 0; i < liftSize; i++)//find next minimum
      {
        if(droppedList[i] < min && droppedList[i] > 0)
        {
          if(droppedList[i] > prev)
          {
            min = droppedList[i];
            pos = i;
          }
        }
      }
      if(pos == -1)
        break;
      prev = min;
      for(int i = 0; i < created; i++)//find citizen position in array
      {
        prio = citizens[i].getPriority();
        if(prio == min)
        {
          spot = i;
          break;
        }
      }
      dest = citizens[spot].getOffice();
      floor = citizens[spot].getFloor();
      for(int i = 1; i < 11; i++)
      {
        if(floor != this.level)//not my floor
          break;
        else if(dest != i)//not my office
          continue;
        else //found my office
        {
          cCap = getEntryCap();
          if(citizens[spot].isAlone())//is alone
          {
            result = this.offices[i-1].enter(citizens[spot]);//attempt to enter
            if(result == 0)//i didnt got in
            {
              cCap++;
              setEntryCap(cCap);
              for(int j = 0; j < mCap; j++)
              {
                if(list[j] == -1)
                {
                  list[j] = min;
                  break;
                }
              }
              //had break here
            }
          }
          else//we are two
          {
            officeCap = offices[i-1].getCap();//get office current cap
            if((officeCap + 1) < this.noCap)//there is room
            {
              result = this.offices[i-1].enter(citizens[spot]);
              result = this.offices[i-1].enter(citizens[spot+1]);
            }
            else//sit in waiting area
            {
              System.out.print("Office[" + i + 
                "]:: Please, wait outside for entrance in office number[" + 
                i + "], priority " + prio + " and Escort!\n");
              cCap += 2;
              setEntryCap(cCap);
              for(int j = 0; j < mCap; j++)
              {
                if(list[j] == -1)
                {
                  list[j] = min;//sit citizen
                  break;
                }
              }
              for(int j = 0; j < mCap; j++)
              {
                if(list[j] == -1)
                {
                  list[j] = 0;//sit escort
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
  //citizens from all the floor's offices are chosen to exit
  public int exit(int cCap, int floor, int[] exitList, int liftSize, Citizen[] citizens, int created)
  {
    int result;
    int counter = 0;
    for(int i = 0; i < 10; i++)//for all the floor offices
    {
      if(counter < cCap)//if lift got room
      {
        result = offices[i].exit(cCap, exitList, liftSize, citizens, created);
        if(result > 0)//office had people to exit
          counter+= result;
      }
      else
        break;
    }
    return counter;
  }
}
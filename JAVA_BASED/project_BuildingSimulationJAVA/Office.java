/*************************************************************************
Implementation File : Office.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's a floor's office
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Office extends Space
{ //subclass of Space
  /*data members*/
  int offId;//unique for each office (1-10)
  int[] list;//list of priorities waiting inside office
  /*constructor*/
  Office(int cur, int max, int id)
  {
    super(cur, max);//create the space
    this.offId = id;//give unique id to the office
    list = new int[max];
    for(int i = 0; i < max; i++)//initialise @ empty
      this.list[i] = -1;
    System.out.println("Office[" + this.offId + "]:: Office number ["
                       + this.offId + "] has been created!"); 
  }
  /*functions*/
  //get office id
  public int getId()
  {
    return this.offId;
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
  //passengers try to enter the office
  public int enter(Citizen next)
  {
    int cCap = getCap();
    int mCap = getMax();
    int priority = next.getPriority();
    if(cCap >= mCap)//office full
    {
      System.out.print("Office[" + this.offId + "]:: Please, wait outside for entrance in office number[" + this.offId + "] citizen with priority: " + priority + "!\n");
      return 0;
    }
    else//there is room
    {
      cCap++;
      setCap(cCap);
      if(!next.isEscort())
        System.out.print("Citizen:: Entering office number[" + this.offId +"] citizen with priority: " + priority +"!\n");
      else
        System.out.print("Escort:: Entering office number[" + this.offId +"]!\n");
      for(int i = 0; i < mCap; i++)//sit citizen on list
      {
        if(list[i] == -1)//found an empty spot inside office
        {
          list[i] = priority;
          break;
        }
      }
      return 1;
    }
  }
  //passengers are chosen to exit the office
  public int exit(int cap, int[] exitList, int exitCap, Citizen[] citizens, int created)
  {
    int randomFinished;
    int counter = 0;
    int cCap = getCap();
    int mCap = getMax();
    int prio = 0;
    int spot = 0;
    randomFinished = (int) (Math.random() * cCap + 2);//2 - office current capacity
    if(randomFinished > cap)//more finished than the lift can carry
      randomFinished = cap;//correct the number 
    if(randomFinished > cCap && randomFinished <= cap)
      randomFinished = cCap;
    if(counter < randomFinished)//can get more
    {
      for(int i = 0; i < mCap; i++)
      {
        if(counter < randomFinished)
        {
          if(list[i] == 0)//someone was sitting here
            continue;//skip escort
          else if(list[i] != -1)//someone was sitting here
          {
            for(int j = 0; j < created; j++)//find citizen position in array
            {
              prio = citizens[j].getPriority();
              if(prio == list[i])
              {
                spot = j;
                break;
              }
            }
            if(citizens[spot].isAlone())//is alone
            {
              for(int j = 0; j < exitCap; j++)
              {
                if(exitList[j] == -1)//spot
                {
                  exitList[j] = list[i];//get him on the exit list
                  list[i] = -1;//clear position
                  counter ++; //increment found counter
                  cCap--; //decrement office cap
                  setCap(cCap);//update office capacity
                  break;
                }
              }
            }
            else//has escort
            {
              if((counter + 1) < randomFinished)//room for both in lift
              {
                for(int j = 0; j < exitCap; j++)
                {
                  if(exitList[j] == -1)//spot
                  {
                    exitList[j] = list[i];//get him on the exit list
                    for(int k = 0; k < exitCap; k++)
                    {
                      if(exitList[k] == -1)
                      {
                        exitList[k] = 0;//get escort on list
                        break;
                      }
                    }
                    list[i] = -1;//clear position
                    for(int k = 0; k < mCap; k++)
                    {
                      if(list[k] == 0)//escort
                      {
                        list[k] = -1;//empty
                        break;
                      }
                    }
                    counter += 2; //increment found counter
                    cCap -= 2; //decrement office cap
                    setCap(cCap);//update office capacity
                    break;
                  }
                }
              }
            }
          }    
        }
        else
          break;
      }
    }
    return counter;
  }
}

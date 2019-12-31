/*************************************************************************
Implementation File : Ground.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implements a building's ground floor
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Ground extends Floor
{
  /*data members*/
  int priority;
  int[] waiting;
  /*constructor*/
  Ground(int cur, int max)
  {
    super(cur, max);//create the floor
    this.priority = 1;//current priority to be given to first citizen
    this.waiting = new int[max];
    for(int i = 0; i < max; i++)//initialise @ empty
      this.waiting[i] = -1;
    System.out.println("Ground:: A ground floor has been created!");  
  }
  /*functions*/
  //gets next priority to be given
  public int getPriority()
  {
    return this.priority;
  }
  //get current (space) capacity
  public int getCap()
  {
    return super.getCap();
  }
  //get max (space) capacity
  public int getMax()
  {
    return super.getMax();
  }
  //set current (space) capacity
  public void setCap(int cCap)
  {
    super.setCap(cCap);
  }
  //get current (entry area) capacity
  public int getEntryCap()
  {
    return super.getEntryCap();
  }
  //set current (entry area) capacity
  public void setEntryCap(int cCap)
  {
    super.setEntryCap(cCap);
  }
  //sets next priority to be given
  public void setPriority(int newPrio)
  {
    this.priority = newPrio;
  }
  //citizen enters ground floor entry area to get priority ticket
  public void enter()
  {
    int cCap = getEntryCap();//get ground floor entrance capacity
    System.out.println("Ground_Floor:: Citizen exited the ground floor and is moving to the ground floor waiting area!");
    cCap++;
    setEntryCap(cCap);
    System.out.println("Entrance:: Citizen entered the ground floor waiting area and is going to get priority ticket!");
  }
  //citizen is given next priority ticket and escort gets 0
  public void wait(Citizen next)
  {
    int newPriority = getPriority();
    int mCap = getMax();
    if(next.isEscort())//escorts have 0 priority
    {
      next.setPriority(0);
      for(int i = 0; i < mCap; i++)
      {
        if(this.waiting[i] == -1)//empty
        {
          this.waiting[i] = 0;
          break;
        }
      }
    }
    else//is citizen give him priority
    {
      next.setPriority(newPriority);
      newPriority ++;
      setPriority(newPriority);
      for(int i = 0; i < mCap; i++)
      {
        if(this.waiting[i] == -1)//empty
        {
          this.waiting[i] = (newPriority -1);
          break;
        }
      }
    }
    if(!next.isEscort())
      System.out.println("Citizen:: Waiting for the lift!");
    else
      System.out.println("Escort:: Waiting for the lift!");
  }
  //holds escorts position in the ground floor list
  public void waitEscort()
  {
    int mCap = getMax();
    for(int i = 0; i < mCap; i++)
    {
      if(this.waiting[i] == -1)//empty
      {
        this.waiting[i] = 0;
        break;
      }
    }
    System.out.println("Escort:: Waiting for the lift!");
  }
  //gets the list of citizens waiting for the lift
  public void getList(int[] list)
  {
    int mCap = getMax();
    for(int i = 0; i < mCap; i++)
      list[i] = this.waiting[i];
  }
  //removes target citizen after boarding the lift
  public void remove(int target)
  {
    int mCap = getMax();
    for(int i = 0; i < mCap; i++)
    {
      if(this.waiting[i] == target)
      {
        this.waiting[i] = -1;
        break;
      }
    }
  }
  //removes escort of citizen after boarding the lift
  public void removeEscort()
  {
    int mCap = getMax();
    for(int i = 0; i < mCap; i++)
    {
      if(this.waiting[i] == 0)
      {
        this.waiting[i] = -1;
        break;
      }
    }
  }
  //exits citizens after boarding the lift and updates capacity
  public void exit(int number)
  {
    int cCap = getCap();
    int mCap = getMax();
    System.out.print("Ground Floor:: " + number + " Citizens are exiting the ground floor!\n");
    cCap -= number;
    setCap(cCap);
  }
}
/*************************************************************************
Implementation File : Citizen.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's a citizen
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Citizen 
{ /*data members*/
  int floorDest;//floor destination
  int officeDest;//office destination
  int priority;//unique for each citizen
  boolean alone;
  boolean escort;
  /*constructor*/
  Citizen(int fdest, int odest, int prio, boolean alon, boolean esco)
  {
    this.floorDest = fdest;
    this.officeDest = odest;
    this.priority = prio;
    this.alone = alon;
    this.escort = esco;
    if(!esco)
    {
      System.out.println("Citizen:: I'm going to floor " + fdest
                       + " and office " + odest + "!");
    }
  }
  /*functions*/
  //get floor destination
  public int getFloor()
  {
    return this.floorDest;
  }
  //get office destination
  public int getOffice()
  {
    return this.officeDest;
  }
  //get citizen's priority ticket
  public int getPriority()
  {
    return this.priority;
  }
  //sets citizen's priority ticket
  public void setPriority(int newPrio)
  {
    this.priority = newPrio;
    if(!isEscort())
      System.out.println("Citizen:: I got priority ticket: " + newPrio + "!");
  }
  //checks if citizen came without an escort
  public boolean isAlone()
  {
    if(alone)
      return true;
    else
      return false;
  }
  //checks if citizen is an escort
  public boolean isEscort()
  {
    if(escort)
      return true;
    else
      return false;
  }
  //sets last of K citizen who had escort but exceeded K to alone
  public void setAlone(boolean status)
  {
    this.alone = status;
  }
}
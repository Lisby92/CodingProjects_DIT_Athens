/*************************************************************************
Implementation File : EntryArea.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's a floor's entry area
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class EntryArea extends Space
{ //subclass of Space
  /*constructor*/
  EntryArea(int cur, int max)
  {
    super(cur, max);
    System.out.println("EntryArea:: An Entrance has been created!");
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
}
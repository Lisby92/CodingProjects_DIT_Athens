/**************************************************************************
Implementation File : Space.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's the concept of capacity that subclasses inherit
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
***************************************************************************/
public class Space
{ /*data members*/
  int curCap;
  int maxCap;
  /*constructor*/
  Space(int cCap, int mCap)
  {
    this.curCap = cCap;
    this.maxCap = mCap;
    System.out.println("Space:: Space created with current capacity "
                       + cCap + " and max capacity " + mCap + "!");
  }
  /*functions*/
  //get current capacity
  public int getCur()
  {
    return this.curCap;
  }
  //set current capacity
  public void setCur(int cCap)
  {
    this.curCap = cCap;
  }
  //get max capacity
  public int getMax()
  {
    return this.maxCap;
  }
}
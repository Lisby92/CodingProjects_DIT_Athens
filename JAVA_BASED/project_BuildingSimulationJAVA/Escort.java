/*************************************************************************
Implementation File : Escort.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Implement's a citizen's escort
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class Escort extends Citizen
{ //subclass of Citizen
  /*constructor*/
  Escort(int fdest, int odest)
  {
    super(fdest, odest, 0, false, true);//has priority 0, is not alone, is escort
    System.out.println("Escort:: I'm escorting Citizen to floor " + fdest
                       + " and office " + odest + "!");
  }
}
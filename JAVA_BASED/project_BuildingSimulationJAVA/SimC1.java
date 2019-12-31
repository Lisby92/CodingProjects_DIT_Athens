/*************************************************************************
Implementation File : SimC1.java
Author Date     : V. Mavrommatis 22/12/2018
Purpose       : Simulates the operation of a building serving citizens
Revision      : < Vasileios_Mavrommatis - 26/12/2018 >
**************************************************************************/
public class SimC1 
{ //this function chooses randomly if citizen will have escort or no
  public static boolean roulette()
  {
    double rand1 = Math.random();
    double rand2 = Math.random();
    if(rand1 > rand2)
      return true;
    else
      return false;
  }
  //main starts here
  public static void main(String[] args)
  {
    if(args.length > 0)
    {
      int n, nf, ng, no, nl, k, l;
      n = Integer.parseInt(args[0]);
      nf = Integer.parseInt(args[1]);
      ng = Integer.parseInt(args[2]);
      no = Integer.parseInt(args[3]);
      nl = Integer.parseInt(args[4]);
      k = Integer.parseInt(args[5]);
      l = Integer.parseInt(args[6]);
      if(n < 0 || nf < 0 || ng < 0 || no < 0 || nl < 0 || k < 0 || l < 0)
      {
        System.err.println("Please give >= 0 integers");
        System.exit(1);
      }
      else if(nf >= n/3)
      {
        System.err.println("Floor capacity must be < (building capacity) / 3");
        System.exit(1);
      }
      else if(ng >= n/2)
      {
        System.err.println("Ground floor capacity must be < (building capacity /2)\n");
        System.exit(1);
      }
      else if(no >= nf)
      {
        System.err.println("Office capacity must be < floor capacity");
        System.exit(1);
      }
      else if(nl <= no)
      {
        System.err.println("Elevator capacity must be > office capacity");
        System.exit(1);
      }
      /* SIMULATION STARTS */
      Building newBuilding = new Building(0, n, nf, ng, no, nl);//create a new building
      int randFloor = 0;//for random floor generator
      int randOffice = 0;/////////// office/////////
      Citizen[] angryNWaiting = new Citizen[k];//citizen and escort array
      int[] aList = new int[k];//list of people outside
      for(int i = 0; i < k; i++)//initialise @ empty
        aList[i] = -1;
      int mark = 0;
      int count = 0;
      int groups = 0;
      while(count < k)//create the citizens and potential escorts
      {
        randFloor = (int) (Math.random() * 4 + 1);
        randOffice = (int) (Math.random() * 10 + 1);
        if(roulette())//has escort
        {
          aList[mark++] = count;
          groups++;
          angryNWaiting[count] = new Citizen(randFloor, randOffice, 0,
          false, false);
          count++;
          if(count < k)//we have room to create the escort
          {
            aList[mark++] = -1;//put empty on escort pos
            angryNWaiting[count] = new Escort(randFloor, randOffice);
            count++;
          }
          else//escort bailed
          {
            angryNWaiting[count-1].setAlone(true);//fix alone data member
            break;
          }
        }
        else//citizen came alone
        {
          aList[mark++] = count;
          groups++;
          angryNWaiting[count] = new Citizen(randFloor, randOffice, 0,
          true, false);
          count++;
        }
      }
      System.out.print("\n");
      int[] doneNLeaving = new int[nl];//create a list of done citizens
      for(int i = 0; i < nl; i++)
        doneNLeaving[i] = -1;
      int pos = 0;//next from outside to enter
      int result = 0;//result of enter
      int temp = groups;
      int temp1 = 0;
      int spot = 0;//bList counter
      int[] bList = new int[n];
      for(int i = 0; i < n; i++)//positions of citizens and escorts in building
        bList[i] = -1;
      mark = 0;  
      while(temp > 0)//Try enter the building
      {
        temp--;
        result = newBuilding.enter(angryNWaiting[pos]);
        if(result == 1)//alone
        {
          aList[mark++] = -1;
          bList[spot++] = pos;
          temp1++;
          pos++;
        }
        else if(result == 2)//with escort
        {
          aList[mark++] = -1;
          aList[mark++] = -1;
          bList[spot++] = pos;
          spot++;
          temp1++;
          pos += 2;
        }
        else
        {
          mark++;
          if(angryNWaiting[pos].isAlone())
            pos++;
          else
          {
            mark++;
            pos += 2;
          }
        }
      }
      System.out.print("\n");
      int between = 0;//counts citizens stuck between main and ground floor
      int left = 0;//counts left outside building
      int pos1 = 0; //next from inside to enter ground
      int spot1 = 0;//gList counter
      int pos2 = 0;//deletes from Blist on entry
      int temp2 = temp1;
      int temp3 = 0;
      int[] gList = new int[ng];
      for(int i = 0; i < ng; i++)//positions of citizens and escorts in ground
        gList[i] = -1;
      while(temp2 > 0)//Try enter the ground floor
      {
        temp2--;
        for(int i = 0; i < n; i++)
        {
          if(bList[i] == pos1)
          {
            pos2 = i;//hold position
            result = newBuilding.groundEnter(angryNWaiting[pos1]);
            break;
          }
        }
        
        if(result == 1)
        {
          bList[pos2] = -1;//remove from building list
          pos2 = 0;
          gList[spot1++] = pos1;
          temp3++;
          pos1++;
        }
        else if(result == 2)//with escort
        {
          bList[pos2] = -1;//remove from building list
          pos2 = 0;
          gList[spot1++] = pos1;
          temp3++;
          pos1 += 2;
          spot1++;
        }
        else
        {
          if(angryNWaiting[pos1].isAlone())
            pos1++;
          else
            pos1 += 2;
        }
      }
      between = temp1 - temp3;//count stuck groups
      System.out.print("\n");
      left = groups - temp1;//groups left
      int temp4 = 0;
      for(int i = 0; i < l; i++)//operate the elevator for L cycles
      {
        newBuilding.operate(angryNWaiting, doneNLeaving, k);//work with current
        newBuilding.exit(doneNLeaving);//exit done citizens
        if(between > 0)//groups stuck inside
        {
          temp1 = 0;
          for(int j = 0; j < n; j++)
          {
            if(bList[j] != -1)//valid group
            {
              result = newBuilding.groundEnter(angryNWaiting[bList[j]]);
              if(result == 1)
              {
                temp1++;
                bList[j] = -1;//remove from building list
              }
              else if(result == 2)//with escort
              {
                temp1++;
                bList[j] = -1;//remove from building list
              }
            }
          }
          between -= temp1;//update stuck based on ground group entries
          System.out.print("\n");
        }
        if(left > 0)//more groups are outside
        {
          temp4 = 0;
          for(int j = 0; j < k; j++)
          {
            if(aList[j] != -1)//valid group
            {
              result = newBuilding.enter(angryNWaiting[aList[j]]);
              if(result == 1)
              {
                temp4++;
                for(int m = 0; m < n; m++)
                {
                  if(bList[m] == -1)
                  {
                    bList[m] = aList[j];
                    aList[j] = -1;//remove from outside list
                    break;
                  }
                }
              }
              else if(result == 2)
              {
                temp4++;
                for(int m = 0; m < n; m++)
                {
                  if(bList[m] == -1)
                  {
                    bList[m] = aList[j];
                    aList[j] = -1;//remove from outside list
                    break;
                  }
                }
              }
            }
          }
          left -= temp4;//update left outside
          between += temp4;//update stuck inside
          System.out.print("\n");
        }
      }
    /* SIMULATION ENDS */
    }
    else //invalid arguments given
    {
      System.err.print("Please give valid integers for "
      + "N, Nf, Ng, No, Nl, K, L as arguments!\n");
      System.exit(1);
    }
  }
  //main ends here  
}
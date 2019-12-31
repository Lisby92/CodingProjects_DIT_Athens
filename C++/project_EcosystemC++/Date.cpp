/****************************************************************************
Implementation File : Date.cpp
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Simulates the time rolling and date changing in the ecosystem
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
*****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "Date.h"
using namespace std;
/* DATE FUNCTIONS */
Date::Date(string startEpoch)
: epoch(startEpoch), day(1), hour(0)
{
  cout << "Constructed a starting date at Epoch: " << epoch;
  cout << " , Day: " << day << " , Hour : " << hour << endl; 
}

Date::~Date()
{
  cout << "\nDate destroyed!\n";
}

void Date::progress()//with hourly steps
{
  if(hour == 23)//day change
  {
    if(day == 89)//epoch change
    {
      string cur = getEpoch();
      //cout << cur << " is cur epoch prior change\n";
      string temp1 = "winter";
      string temp2 = "spring";
      string temp3 = "summer";
      string temp4 = "autumn";
      if(cur.compare(temp1) == 0)//change to spring
        setEpoch(temp2);
      else if(cur.compare(temp2) == 0)//change to summer
        setEpoch(temp3);
      else if(cur.compare(temp3) == 0)//change to autumn
        setEpoch(temp4);
      else if(cur.compare(temp4) == 0)//change to winter
        setEpoch(temp1);
      day = 0;
      hour = 0;
    }
    else
    {
      day++;
      hour = 0;
    }
  }
  else//hour change
    hour++;
}

string Date::getEpoch() const
{
  return epoch;
}

void Date::setEpoch(string newEpoch)
{
  epoch.assign(newEpoch);
}
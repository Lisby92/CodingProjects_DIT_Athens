#include "TimeFrame.hpp"
#include <iostream>

using namespace std;

TimeFrame::TimeFrame(int d = 1, int m = 1, int y = 2019, int h = 0, int min = 0)
: day(d), month(m), year(y), hour(h), minute(min)
{
  /*
  cout << "TimeFrame constructed successfully!\n";
  cout << "Day : " << day << endl;
  cout << "Month : " << month << endl; 
  cout << "Year : " << year << endl; 
  cout << "Hour : " << hour << endl; 
  cout << "Minute : " << minute << endl;
  */
}

TimeFrame::~TimeFrame()
{
  //cout << "TimeFrame destroyed successfully!\n";
}
//thelei optimize gia t ohour HH kai DD ktlpa na vgainoun idia
void TimeFrame::print() const
{
  cout << day << "-";
  cout << month << "-";
  cout << year << " ";
  cout << hour << ":";
  cout << minute << endl;
}

int TimeFrame::getDay() const
{
  return day;
}

int TimeFrame::getMonth() const
{
  return month;
}

int TimeFrame::getYear() const
{
  return year;
}

int TimeFrame::getHour() const
{
  return hour;
}

int TimeFrame::getMinute() const
{
  return minute;
}
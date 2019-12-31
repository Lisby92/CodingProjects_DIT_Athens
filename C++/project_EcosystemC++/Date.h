/*************************************************************************
Implementation File : Date.h
Author Date     : V. Mavrommatis 2/1/2019
Purpose       : Header file of Date.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 15/1/2019 >
**************************************************************************/
#ifndef __Date__
#define __Date__
#include <string>
using namespace std;

class Date
{
  private:
    string epoch;
    int day;
    int hour;

  public:  
    Date(string);//constructor
    ~Date();//destructor
    void progress();//progress with steps (1hour)
    string getEpoch() const;//get current epoch
    void setEpoch(string);//set epoch every 90 days passing
};

#endif
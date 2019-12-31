#ifndef __TIMEFRAME__
#define __TIMEFRAME__

class TimeFrame
{
public:
  TimeFrame(int, int, int, int, int);//constructor
  ~TimeFrame();//destructor
  int getDay() const;//get transaction day
  int getMonth() const;//get transaction month
  int getYear() const;//get transaction year
  int getHour() const;//get transaction hour
  int getMinute() const;//get transaction minute
  void setDay(const int & newVal);//set transaction day
  void setMonth(const int & newVal);//set transaction month
  void setYear(const int & newVal);//set transaction year
  void setHour(const int & newVal);//set transaction hour
  void setMinute(const int & newVal);//set transaction minute
  void print() const;//print the complete date and time (DD-MM-YYYY HH:MM)

private:
  int day;
  int month;
  int year;
  int hour;
  int minute;
};

#endif
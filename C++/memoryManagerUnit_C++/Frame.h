/*************************************************************************
Implementation File : Frame.h
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Header file of Frame.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#ifndef __Frame__
#define __Frame__

class Frame
{
  private:
    char * data;//frameSize = pageSize
    
  public:
  	Frame(int);//constructor//fills pageData with random bytes that could be instructions or actual data
  	~Frame();//destructor
  	char getData(const int &) const;//get byte from specific offset
  	void setData(const int &, const char &);//write byte on specific offset
};

#endif
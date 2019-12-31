/*************************************************************************
Implementation File : Frame.cpp
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Implementation of a frame in RAM
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#include <iostream>
#include <cstdlib>
#include "Frame.h"
using namespace std;

Frame::Frame(int frameSize)
{
  data = new char[frameSize];
  for(int i = 0; i < frameSize; i++)
    data[i] = rand() % (255 + 1 - 0) + 0;//fill with trash
  //cout << "Frame created succesfully!\n";
}

Frame::~Frame()
{
  delete[] data;
  //cout << "Frame destroyed succesfully!\n";
}

char Frame::getData(const int & offset) const
{
  return data[offset];
}

void Frame::setData(const int & offset, const char & byte)
{
  data[offset] = byte;
}
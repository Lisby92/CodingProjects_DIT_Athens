/*************************************************************************
Implementation File : MemoryManagerUnit.h
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Header file of MemoryManagerUnit.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#ifndef __MemoryManagerUnit__
#define __MemoryManagerUnit__
#include "HashPageTable.h"
#include "Frame.h"
//holds various statistics gathered during, and at the end of simulation
class Statistics
{
  private:
    int readDisk;//total R from disk
    int writeDisk;//total W on disk
    int pageFaults;//total page faults
    int frameUsage;//total frames still in use
    int searches;//total searches on the PT entries

  public:
    Statistics();//constructor
    ~Statistics();//destructor
    void displayStats() const;//print gathered statistics
    int getReadD() const;//get disk R
    int getWriteD() const;//get disk W
    int getFaults() const;//get total page faults
    int getSearches() const;//get total searches on nodes
    void setReadD(const int &);//set disk R
    void setWriteD(const int &);//set disk W
    void setFaults(const int &);//set page faults
    void setUsed(const int &);//set used frames
    void setSearches(const int &);//set searches on nodes
};
//the MMU that conducts the memory managing
class MemoryManagerUnit
{
  private:
    int maxFrames;//frames available totally
    int frameSize;//size of frama/page
    HashPageTable * hpt;//virtual adresses table
    Frame ** memory[2];//RAM split in two : 0 ~ maxFrames/2 -1 for proccess 1 and rest for process 2
    char * disk;//the system's local disk
    Statistics results;//holds various statistics and counters

  public:
    MemoryManagerUnit(int, int, int);//constructor
    ~MemoryManagerUnit();//destructor
    void print() const;//display simulation results
    void readDisk(char *) const;//read data from disk when PF(cold or no) occurs
    void writeDisk(const int &, const int &);//write the dirty page that will be replaced back to disk
    char readRAM(const int &, const int &, const int &) const;//read data from a valid RAM frame
    void writeRAM(char *, const int &, const int &);//write data on a valid RAM frame
    void flushWhenFull(const int &, int *);//Page replacement algorithm to be used
    char operate(const int &, const int &, const int &, const int &);//operates the MMU based on references read from the trace
    void find();//find frames used by both processes at end of simulation
};

#endif

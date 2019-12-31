/*************************************************************************
Implementation File : MemoryManagerUnit.cpp
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Implementation of a MMU's operations in memory PT and disk
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include "MemoryManagerUnit.h"
using namespace std;

Statistics::Statistics()
: readDisk(0), writeDisk(0), pageFaults(0), frameUsage(0), searches(0)
{
  //cout << "Statistics section created successfully!\n";
}

Statistics::~Statistics()
{
  //cout << "Statistics section destroyed successfully!\n";
}

void Statistics::displayStats() const
{
  cout << "\nDisplaying MMU statistics below!\n\n";
  cout << "Disk Read : " << readDisk;
  cout << "\nDisk Write : " << writeDisk;
  cout << "\nTotal Page Faults : " << pageFaults;
  cout << "\nTotal Frames still used : " << frameUsage;
  cout << "\nTotal Searches on all chains: " << searches << "\n\n";
}

int Statistics::getReadD() const
{
  return readDisk;
}

int Statistics::getWriteD() const
{
  return writeDisk;
}

int Statistics::getFaults() const
{
  return pageFaults;
}

int Statistics::getSearches() const
{
  return searches;
}

void Statistics::setReadD(const int & R)
{
  readDisk = R;
}

void Statistics::setWriteD(const int & W)
{
  writeDisk = W;
}

void Statistics::setFaults(const int & pf)
{
  pageFaults = pf;
}

void Statistics::setUsed(const int & uf)
{
  frameUsage = uf;
}

void Statistics::setSearches(const int & s)
{
  searches = s;
}

MemoryManagerUnit::MemoryManagerUnit(int ps = 4096, int mfra = 1000, int mfau = 10)
: maxFrames(mfra / 2), frameSize(ps)//its actually realistic sooooo careful with heap :P
{
  hpt = new HashPageTable(mfra, mfau);//create the hash page table
  for(int i = 0; i < 2; i ++)//create the available RAM frames
  {
    memory[i] = new Frame*[mfra / 2];
    for(int j = 0; j < (mfra / 2); j++)
      memory[i][j] = new Frame(ps);
  }
  disk = new char[ps];//create the local disk
  for(int i = 0; i < ps; i++)//much bigger but we will write or read blocks = frame/page size so it's precise
    disk[i] = rand() % (255 + 1 - 0) + 0;//fill with useful data and code to simulate the disk
  //cout << "MMU created successfully!\n";
}

MemoryManagerUnit::~MemoryManagerUnit()
{
  delete hpt;//destroy the hash page table
  for(int i = 0; i < 2; i ++)
  {
    for(int j = 0; j < maxFrames; j++)//destroy RAM
      delete memory[i][j];
    delete[] memory[i];
  }
  delete[] disk;//destroy the local disk
  //cout << "MMU destroyed successfully!\n";
}

void MemoryManagerUnit::readDisk(char * temp) const
{
  //cout << "Retrieving data from disk!\n";
  for(int i = 0; i < frameSize; i++)
    temp[i] = disk[i];//fill with code and data from disk
}

void MemoryManagerUnit::writeDisk(const int & frame, const int & src)
{
  //cout << "Writing data on disk!\n";
  for(int i = 0; i < frameSize; i++)//write data on disk from the dirty frame
    disk[i] = memory[src-1][frame]->getData(i);
}

char MemoryManagerUnit::readRAM(const int & off, const int & frame, const int & src) const
{
  return memory[src-1][frame]->getData(off);//return data to the process
}

void MemoryManagerUnit::writeRAM(char * newData, const int & frame, const int & src)
{
  //cout << "Writing data on RAM!\n";
  for(int i = 0; i < frameSize; i++)//write data on RAM frame
    memory[src-1][frame]->setData(i, newData[i]);
}

void MemoryManagerUnit::flushWhenFull(const int & src, int * victims)
{
  hpt->flushEntries(src, victims, maxFrames);
}

void MemoryManagerUnit::find()
{
  int used = hpt->findUsed();
  results.setUsed(used);
}

char MemoryManagerUnit::operate(const int & key, const int & mode, const int & off, const int & src)
{
  int curPFs = hpt->getCurPF(src);
  int maxPFs = hpt->getMaxPF();
  int * victims = new int[maxFrames];//victims list for flushing
  for(int i = 0; i < maxFrames; i++)
    victims[i] = -1;
  if(curPFs > maxPFs)// k + 1 PF ? flush
  {
    flushWhenFull(src, victims);// flush the frames
    hpt->setCurPF(src, 0);
    for(int i = 0; i < maxFrames; i++)
    {
      if(victims[i] != -1)//dirty victim
      {
        writeDisk(victims[i], src);
        int w = results.getWriteD();
        w++;
        results.setWriteD(w);//had to write on disk
      }
    }
  }
  int hash = hpt->hashFunction(key);//calculate the hash
  //cout << hash << " is the calculated hash from key " << key << "\n";
  int searches = 0;//searches conducted for current reference
  bool dirty;
  char * receivedData = new char[frameSize];//reading from disk = getting blocks
  for(int i = 0; i < frameSize; i++)
    receivedData[i] = -1;//init
  char receivedByte = 1;
  int replacedFrame = -1;
  if(mode == 82)//read
    dirty = false;
  else if(mode == 87)//write
    dirty = true;
  int adjustedFrame = (hash % maxFrames);//adjust since RAM is evenly split
  int frame = hpt->find(adjustedFrame, key, searches, dirty, src, replacedFrame);//look up for the PTE
  searches += results.getSearches();
  results.setSearches(searches);//update searches
  if(frame == -2)//1)Cold PF frame free, read from disk, write RAM, add PTE and send data
  {//2)Cold PF frame occupied and clean, read from disk, write RAM, add PTE and send data
    //cout << "Cold Page Fault - No entry found!\n";
    curPFs = hpt->getCurPF(src);
    curPFs++;
    hpt->setCurPF(src, curPFs);//update faults
    int faults = results.getFaults();
    faults++;
    results.setFaults(faults);
    readDisk(receivedData);
    int r = results.getReadD();
    r++;
    results.setReadD(r);//had to read data from disk
    writeRAM(receivedData, adjustedFrame, src);//write on the correct RAM frame
    if(mode == 82)//read
      hpt->addEntry(key, adjustedFrame, adjustedFrame, src, false, true);
    else if(mode == 87)//write
      hpt->addEntry(key, adjustedFrame, adjustedFrame, src, true, true);
    receivedByte = readRAM(off, adjustedFrame, src); //give the byte to the process
  }
  else if(frame == -1)//PF frame occupied
  {
    if(replacedFrame == -1)//Frame clean, read from disk, write RAM and send data
    {
      //cout << "Page Fault - Entry found invalid! Frame Clean!\n";
    }
    else//Frame Dirty, write dirty at disk, read from disk, write RAM and send data
    {
      //cout << "Page Fault - Entry found invalid! Frame Dirty!\n";
      writeDisk(replacedFrame, src);
      int w = results.getWriteD();
      w++;
      results.setWriteD(w);//had to write on disk
    }
    curPFs = hpt->getCurPF(src);
    curPFs++;
    hpt->setCurPF(src, curPFs);//update faults
    int faults = results.getFaults();
    faults++;
    results.setFaults(faults);
    readDisk(receivedData);
    int r = results.getReadD();
    r++;
    results.setReadD(r);//had to read data from disk
    writeRAM(receivedData, adjustedFrame, src);//write on the correct RAM frame
    receivedByte = readRAM(off, adjustedFrame, src); //give the byte to the process
  }
  else if(frame == -3)//Cold PF frame occupied and dirty, write dirty at disk, read from disk, write RAM and send data
  { //(PTE is added) on searchNode() already
    //cout << "Cold Page Fault - No Entry Found! Frame Dirty!\n";
    curPFs = hpt->getCurPF(src);
    curPFs++;
    hpt->setCurPF(src, curPFs);//update faults
    int faults = results.getFaults();
    faults++;
    results.setFaults(faults);
    writeDisk(replacedFrame, src);
    int w = results.getWriteD();
    w++;
    results.setWriteD(w);//had to write on disk
    readDisk(receivedData);
    int r = results.getReadD();
    r++;
    results.setReadD(r);//had to read data from disk
    writeRAM(receivedData, adjustedFrame, src);//write on the correct RAM frame
    receivedByte = readRAM(off, adjustedFrame, src); //give the byte to the process
  }
  else//the good case, 0 disk access, entry found and page is valid in the frame
  {
    //cout << "Found it at RAM frame " << frame << "\n";
    receivedByte = readRAM(off, adjustedFrame, src); //give the byte to the process
  }
  delete[] receivedData;
  delete[] victims;
  //cout << "process " << src << " gets byte " << receivedByte << "\n";
  return receivedByte;
}

void MemoryManagerUnit::print() const
{
  results.displayStats();
}
/*************************************************************************
Implementation File : HashPageTable.h
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Header file of HashPageTable.cpp containing definitions
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#ifndef __HashPageTable__
#define __HashPageTable__
//a PTE in the hash PT with 5 fields
class PageTableEntry
{
  private:
    int vpn;//virtual page number
    int pfn;//page frame number
    int pid;//process id
    bool dirtyBit;//modified or no 1=dirty, 0=clean
    bool validBit;//in RAM or no, 1 = valid, 0 = invalid
    
  public:
    PageTableEntry(int, int, int, bool, bool);//constructor
    ~PageTableEntry();//destructor
    int getPage() const;//get virtual page number
    int getFrame() const;//get page frame number
    int getPid() const;//get process id
    void setDirtyBit(const bool &);//set dirty bit
    bool getDirtyBit() const;//get dirty bit
    void setValidBit(const bool &);//set valid bit
    bool getValidBit() const;//get valid bit
};
//nodes contain PTEs and are linked for conflict resolution
class Node
{
  private:
    PageTableEntry * pte;//the PTE

  public:
    Node * next;//pointer to the next conflicting PTE
    Node(int, int, int, bool, bool);//constructor
    ~Node();//destructor
    int getVPN() const;//get PTE virtual page number
    int getPFN() const;//get PTE page frame number
    int getSrc() const;//get PTE process id
    void setD(const bool &);//set PTE dirty bit
    bool getD() const;//get PTE dirty bit
    void setV(const bool &);//set PTE valid bit
    bool getV() const;//get PTE valid bit
};
//a list of conflicting nodes implemented as a stack actually
class Chain
{
  private:
    Node * head;//the first node in the chain
    int id;//chain's id

  public:
    Chain();//constructor
    ~Chain();//destructor
    void setId(const int &);//set chain id
    void addNode(const int &, const int &, const int &, const bool &, const bool &);//adds a node on the chain
    int searchNode(const int &, int &, const bool &, const int &, int &);//searches the chain's nodes to find the needed PTE
    void flushNode(const int &, int *, const int &);//used by FWF replacement algorithm to flush nodes
    int findFrames() const;//find how many frames are still used by the processes on sim end
};
//the Hash PT where the PTEs reside and virtual addresses are mapped to physical RAM frames
class HashPageTable
{
  private:
    Chain * pteArray;//array of page table entries objects
    int size;//number of entries chains
    int maxFaults;//k maximum page faults before flush
    int curFaults1;//current faults of process1
    int curFaults2;//current faults of process2

  public:
    HashPageTable(int, int);//constructor
    ~HashPageTable();//destructor
    void addEntry(const int &, const int &, const int &, const int &, const bool &, const bool &);//add a new PTE after Cold PF
    int hashFunction(const int &) const;//distributes the virtual addresses to the frames by calculating the hash
    int find(const int &, const int &, int &, const bool &, const int &, int &) const;//looks for a PTE on the corresponding chain
    int getMaxPF() const;//get maximum page faults that can be endured before flushing the frames
    int getCurPF(const int &) const;//get page fault number from the needed process since last flush or simulation start
    void setCurPF(const int &, const int &);//set the page fault counter of the process
    void flushEntries(const int &, int *, const int &);//find victim pages to flush
    int findUsed() const;//get number of occupied frames by both processes
};

#endif
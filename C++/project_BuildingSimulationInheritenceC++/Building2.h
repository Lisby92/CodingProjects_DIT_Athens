/*************************************************************************
Implementation File	: Building2.h
Author Date			: V. Mavrommatis 17/12/2018
Purpose				: Header file of Building2.cpp containing definitions
Revision			: < Vasileios_Mavrommatis - 18/12/2018 >
**************************************************************************/
#ifndef __Building2__ 
#define __Building2__

//Class definition : Simulates the definition of space
class Space
{
	private:
		int cur_cap;//current capacity
		int max_cap;//max capacity
	
	public:
		Space(int, int);//constructor
		~Space();//destructor
		void get_cur(int &) const;//get current capacity
		void set_cur(const int &);//set current space capacity
		void get_max(int &) const;//get max space capacity
		void get_all(int &, int &) const;//get current and max space capacity
};

//Class definition : Simulates a citizen
class Citizen
{
	private:
		int floor_dest;//floor destination
		int office_dest;//office destination
		int priority;//unique for each citizen

	public:
		Citizen(int, int, int);//constructor
		~Citizen();//destructor
		void get_priority(int &) const;//get citizen priority
		void set_priority(const int &);//update citizen priority
		void get_floor(int &) const;//find floor destination
		void get_office(int &) const;//find office destination
};
//Class definition : Simulates an entry area
class Entry_Area : public Space
{
	public:
		Entry_Area(int, int);//constructor
		~Entry_Area();//destructor
		void get_cap(int &) const;//get current and max capacity
		void set_cap(const int &);//ser current capacity
};
//Class definition : Simulates an office
class Office : public Space
{
	private:
		int off_id;//unique for each office (1-10)
		int *list;//list of priorities waiting inside office
		
	public:
		Office(int, int, int);//constructor
		~Office();//destructor
		void get_id(int &) const;//get office id
		void set_cap(const int &);//update office current capacity
		void get_cur_cap(int &) const;//get office current capacity
		void get_max_cap(int &) const;//get office max capacity
		int enter(Citizen *);//sits the citizen given in the office
		int exit(const int &, int *, const int &);//exit from the office
};
//Class definition : Simulates a floor
class Floor : public Space
{
	private:
		int No_cap;//max office capacity
		int level;//current floor level
		Entry_Area *F_Entry;//floor entrance area
		Office **Offices;//an array of offices
		int *list;//list of waiting in floor entry area to enter an office

	public:
		Floor(int, int, int, int);//constructor with offices
		Floor(int, int, int);//constructor w/o offices
		~Floor();//destructor
		void get_cap(int &, int &) const;//get floor current and max capacity
		void get_max(int &) const;
		void set_cap(const int &);//update floor current capacity
		void get_cur_cap(int &) const;//get floor current capacity
		void get_w_cap(int &c_cap) const;//get floor entry area current capacity
		void set_w_cap(const int &c_cap);//update floor entry area current capacity
		void set_dropped(const int &);//sit the citizens who didnt get into an office in the waiting area
		void enter(Citizen **, int *, const int &);//lift doors open for citizens to enter the floor
		int exit(const int &, const int &, int *, const int &);//citizens done exit the current floor when the lift stops down
};
//Class definition : Simulates a ground floor
class Ground : public Floor
{
	private:
		int priority;//starts with 1
		int *waiting;//waiting list of citizens to board the lift

	public:
		Ground(int, int, int, int);//constructor
		~Ground();//destructor
		void get_cap(int &, int &) const;//get current and max capacity
		void get_cur_cap(int &) const;//get entry area current capacity
		void set_cur_cap(const int &);//update entry area current capacity
		void set_cap(const int &);//update current capacity
		void get_priority(int &) const;//get next priority for the current citizen
		void set_priority(const int &);//update priority machine to the next ticket
		void wait(Citizen *);//hands out the priority tickets
		void enter();//enter ground floor entry area
		void get_list(int *);//get list of waiting to board the lift
		void remove(const int &);//remove boarded passenger
		void exit(const int &);//exit the ground floor
};
//Class definition : Simulates an elevator
class Elevator : public Space
{
	private:
		int *Onboard;//list of passengers on the lift

	public:
		Elevator(int, int);//constructor
		~Elevator();//destructor
		void get_cap(int &, int &) const;//get current and max lift capacity
		void get_max_cap(int &) const;//get lift max capacity
		void get_cur_cap(int &) const;//get current lift capacity
		void set_cap(const int &);//update current lift capacity
		void get_onboard(int *);//get list of passengers
		void set_onboard(int *);//update list of passengers
		void print(const int &, const int &) const;//prints all the current passenger priorities(-1 == empty)
		int enter(Citizen *, int *);//all the citizens from the ground floor waiting area try enter the lift
		void exit(const int &);//passengers done leave the elevator
		int stop_up(Citizen **, int *, int *, int &, int &, int &);//stop up at next floor and drop passengers
		void stop_down(int *, int *);//stop down at next floor and gather done citizens
		int empty_all(int *, int *, int *);//removes done passengers from the list and updates capacity
};
//Class definition : Simulates a building
class Building : public Space
{
	private:
		int Nf_cap;//floor max capacity
		int Ng_cap;//ground floor max capacity
		int No_cap;//office max capacity
		int Nl_cap;//lift max capacity
		Floor **Floors;//the building floors
		Ground *Ground_Floor;//the ground floor
		Elevator *Lift;//the lift

	public:
		Building(int, int, int, int, int, int);//constructor
		~Building();//destructor
		void get_cap(int &, int &) const;//get building current and max capacity
		void get_cur_cap(int &) const;//get building current  capacity
		void set_cap(const int &);//update building capacity
		bool enter();//citizens from the outside world try enter the building
		void exit(int *);//happy citizens exit the building
		bool g_enter(Citizen *);//citizens inside the building try enter the ground floor
		void operate(Citizen **, int *);//simulates the building on L lift up and down cycles
};
//end of definitions
#endif

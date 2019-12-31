/*************************************************************************
Implementation File	: Building.cpp
Author Date			: V. Mavrommatis 24/11/2018
Purpose				: Implementation of building and subclass(es) functions
Revision			: < Vasileios_Mavrommatis - 1/12/2018 >
**************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits.h>
#include "Building.h"

using namespace std;
//functions of class : citizen
Citizen::Citizen(int fdest = 0, int odest = 0, int prio = 1)
	: floor_dest(fdest), office_dest(odest), priority(prio)
		{
			cout << "Citizen:: I'm going to floor " << fdest << " and office " << odest << "\n";
		}

Citizen::~Citizen()
{
	cout << "Citizen:: That took a while, ain't coming back soon!\n";
}

void Citizen::get_priority(int & my_priority) const
{
	my_priority = this->priority;
}

void Citizen::set_priority(const int &new_priority)
{
	this->priority = new_priority;
	cout << "Citizen:: I got priority ticket: " << new_priority << "!\n";
}

void Citizen::get_floor(int & dest) const
{
	dest = this->floor_dest;
}

void Citizen::get_office(int & dest) const
{
	dest = this->office_dest;
}
//functions of class : entry_area
Entry_Area::Entry_Area(int cur = 0)//can also add max capacity
	: cur_cap(cur)
		{
			cout << "Entrance:: The Entrance has been created!\n";
			cout << "Entrance:: Current Entrance capacity = " << this->cur_cap << "!\n";
		}

Entry_Area::~Entry_Area()
{
	cout << "Entrance:: End of waiting people!\n";
}

void Entry_Area::get_cap(int &c_cap) const
{
	c_cap = this->cur_cap;
}

void Entry_Area::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}
//functions of class : office
Office::Office(int No = 0, int cur = 0, int id = 0)
	: No_cap(No), cur_cap(cur), off_id(id)
		{
			cout << "Office[" << this->off_id << "]:: Office number [" << this->off_id << "] has been created!\n";
			cout << "Office[" << this->off_id << "]:: current capacity = " << this->cur_cap << " and max capacity = " << this->No_cap << "!\n";
			list = new int[No_cap];
			for(int i = 0; i < No_cap; i++)
				list[i] = -1;
		}

Office::~Office()
{
	delete[] list;
	cout << "Office[" << this->off_id << "]:: End of the work!\n";
}

void Office::get_id(int & id) const
{
	id = this->off_id;
}

void Office::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}

void Office::get_cur_cap(int &c_cap) const
{
	c_cap = this->cur_cap;
}

void Office::get_max_cap(int &m_cap) const
{
	m_cap = this->No_cap;
}

int Office::enter(Citizen *next)
{
	int c_cap, m_cap, id;
	int priority;

	get_id(id);
	get_cur_cap(c_cap);
	get_max_cap(m_cap);
	next->get_priority(priority);
	if(c_cap >= m_cap)//office full
	{
		cout << "Office[" << id << "]:: Please, wait outside for entrance in office number[" << id << "] citizen with priority: " << priority << "!\n";
		return 0;
	}
	else
	{
		c_cap++;
		set_cap(c_cap);
		cout << "Citizen:: Entering office number[" << id << "] citizen with priority: " << priority << "!\n";
		//find citizen on list
		for(int i = 0; i < m_cap; i++)
		{
			if(list[i] == -1)//found an empty spot inside office
			{
				list[i] = priority;
				break;
			}
		}
		return 1;
	}
}

int Office::exit(const int & cap, int * exit_list, const int & exit_cap)
{
	int random_finished;
	int counter = 0;
	int c_cap = 0;

	get_cur_cap(c_cap);
	random_finished = rand() % (c_cap + 1 - 0) + 1;//1 - office current capacity
	if(random_finished > cap)//more finished than the lift can carry
		random_finished = cap;//correct the number
	if(counter < random_finished)//can get more
	{
		for(int i = 0; i < this->No_cap; i++)
		{
			if(counter < random_finished)
			{
				if(list[i] != -1)//someone was sitting here
				{
					for(int j = 0; j < exit_cap; j++)
					{
						if(exit_list[j] == -1)//spot
						{
							exit_list[j] = list[i];//get him on the exit list
							list[i] = -1;//clear position
							counter ++; //increment found counter
							c_cap--; //decrement office cap
							set_cap(c_cap);//update office capacity
							break;
						}
					}
				}
			}
			else
				break;
		}
	}
	return counter;

}
//functions of class : ground floor
Ground::Ground(int Ng = 0, int cur = 0, int lvl = 0, int prio = 1)
	: Ng_cap(Ng), cur_cap(cur), level(lvl), priority(prio)
		{
			//create the entry area
			G_Entry = new Entry_Area(0);
			//create the waiting list
			waiting = new int[Ng_cap];
			for(int i = 0; i < Ng_cap; i++)
				waiting[i] = -1;
			cout << "Ground_Floor:: Ground Floor has been created with number [" << this->level << "]!\n";
			cout << "Ground_Floor:: current capacity = " << this->cur_cap << " and max capacity = " << this->Ng_cap << "!\n\n";
		}

Ground::~Ground()
{
	delete[] waiting;
	delete G_Entry;
	cout << "Ground_Floor:: End of service!\n";
}

void Ground::get_cap(int &c_cap, int &m_cap) const
{
	c_cap = this->cur_cap;
	m_cap = this->Ng_cap;
}

void Ground::get_cur_cap(int &c_cap) const
{
	G_Entry->get_cap(c_cap);
}

void Ground::set_cur_cap(const int &c_cap)
{
	G_Entry->set_cap(c_cap);
}

void Ground::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}

void Ground::get_priority(int & prio) const
{
	prio = this->priority;
}

void Ground::set_priority(const int &prio)
{
	this->priority = prio;
}

void Ground::wait(Citizen * next)
{
	int new_priority;

	get_priority(new_priority);
	next->set_priority(new_priority);
	for(int i = 0; i < Ng_cap; i++)
	{
		if(waiting[i] == -1)//empty
		{
			waiting[i] = new_priority;
			break;
		}
	}
	new_priority ++;
	set_priority(new_priority);
	cout << "Citizen:: Waiting for the lift\n";
}

void Ground::enter()
{
	int c_cap;

	cout << "Ground_Floor:: Citizen exited the ground floor and is moving to the ground floor waiting area!\n";
	G_Entry->get_cap(c_cap);
	c_cap++;
	G_Entry->set_cap(c_cap);
	cout << "Entrance:: Citizen entered the ground floor waiting area and is going to get priority ticket!\n";
}

void Ground::get_list(int * list)
{
	int pos = 0;
	for(int i = 0; i < Ng_cap; i++)
	{
		list[pos] = waiting[i];
		pos++;
	}
}

void Ground::remove(const int & mark)
{
	for(int i = 0; i < Ng_cap; i++)
	{
		if(waiting[i] == mark)
		{
			waiting[i] = -1;
			break;
		}
	}
}

void Ground::exit(const int & number)
{
	int cur_cap, max_cap;

	get_cap(cur_cap, max_cap);
	cout << "Ground Floor:: " << number << " Citizens are exiting the ground floor!\n";
	cur_cap -= number;
	set_cap(cur_cap);
}
//functions of class : floor
Floor::Floor(int Nf = 0, int No = 0, int cur = 0, int lvl = 0)
	: Nf_cap(Nf), No_cap(No), cur_cap(cur), level(lvl)
		{
			//create the entry area
			F_Entry = new Entry_Area(0);
			//create the 10 offices
			Offices = new Office*[10];
			for(int i = 0; i < 10; i++)
				Offices[i] = new Office(No_cap, 0, i + 1);
			cout << "Floor[" << this->level << "]:: A Floor has been created with number [" << this->level << "]!\n";
			cout << "Floor[" << this->level << "]:: Current capacity = " << this->cur_cap << " and max capacity = " << this->Nf_cap << "!\n";
			cout << "Floor[" << this->level << "]:: Ten offices 1~10 created successfully!\n";
			cout << "Floor[" << this->level << "]:: " << (10 * No_cap) << " Total citizens can be served!\n\n";
			list = new int[Nf_cap];
			for(int i = 0; i < Nf_cap; i++)
				list[i] = -1;
		}

Floor::~Floor()
{
	delete[] list;
	for(int i = 0; i < 10; i++)
        delete Offices[i];
    delete[] Offices;
    delete F_Entry;
	cout << "Floor[" << this->level << "]:: End of service!\n\n";
}

void Floor::get_cap(int &c_cap, int &m_cap) const
{
	c_cap = this->cur_cap;
	m_cap = this->Nf_cap;
}

void Floor::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}

void Floor::get_cur_cap(int &c_cap) const
{
	c_cap = this->cur_cap;
}

void Floor::get_w_cap(int &c_cap) const
{
	F_Entry->get_cap(c_cap);
}

void Floor::set_w_cap(const int &c_cap)
{
	F_Entry->set_cap(c_cap);
}

void Floor::set_dropped(const int & dropped)
{
	for(int i = 0; i < this->Nf_cap; i++)
	{
		if(list[i] == -1)//empty
		{
			list[i] = dropped;
			break;
		}
	}
}

void Floor::enter(Citizen **Passengers, int *dropped_list , const int &size)
{
	int dest;
	int cur_cap;
	int min;
	int result;
	int priority;
	int floor;
	int counter = 0;
	int pos;
	int prev = -2;
	//part 1
	for(int i = 0; i < Nf_cap; i++)
		if(list[i] != -1)
			counter++;
	//if people waiting already they surely have lower priority than the next to come 
	while(counter > 0)
	{
		counter--;
		min = INT_MAX;
		pos = -1;
		for(int i = 0; i < Nf_cap; i++)//find next minimum
		{
			if(list[i] < min && list[i] != -1)
			{
				if(list[i] > prev)
				{
					min = list[i];
					pos = i;
				}
			}
		}
		prev = min;
		Passengers[min-1]->get_office(dest);
		Passengers[min-1]->get_floor(floor);
		for(int j = 1; j < 11; j++)
		{
			if(dest != j)//not my office check next
				continue;
			else //found my office and im actually in the right floor
			{
				result = Offices[j-1]->enter(Passengers[min-1]);//attempt to enter
				if(result == 1)//i got in
				{
					F_Entry->get_cap(cur_cap);
					cur_cap--;;
					F_Entry->set_cap(cur_cap);
					list[pos] = -1;
					break;
				}
			}
		}
	}
	//part 2 open the lift door to unload fresh
	for(int i = 0; i < size; i++)
	{
		if(dropped_list[i] != -1)
		{
			Passengers[dropped_list[i]-1]->get_office(dest);
			Passengers[dropped_list[i]-1]->get_floor(floor);
			Passengers[dropped_list[i]-1]->get_priority(priority);
			for(int j = 1; j < 11; j++)
			{
				if(dest != j)//not my office check next
					continue;
				else if(floor != level)//not my floor i shouldnt be here
					break;
				else //found my office and im actually in the right floor
				{
					result = Offices[j-1]->enter(Passengers[dropped_list[i]-1]);//attempt to enter
					if(result == 0)//i didnt got in
					{
						F_Entry->get_cap(cur_cap);
						cur_cap++;;
						F_Entry->set_cap(cur_cap);
						set_dropped(priority);
						break;
					}
				}
			}

		}
	}
}

int Floor::exit(const int & c_cap, const int & floor, int * exit_list, const int & lift_max)
{
	int result;
	int counter = 0;
	//int cur_cap;
	//for all the floor offices
	for(int i = 0; i < 10; i++)
	{
		if(counter < c_cap)//if lift got room
		{
			result = Offices[i]->exit(c_cap, exit_list ,lift_max);
			if(result > 0)//office had people to exit
				counter+= result;

		}
		else
			break;
	}
	return counter;
}
//functions of class : elevator
Elevator::Elevator(int Nl = 0, int cur = 0)
	: Nl_cap(Nl), cur_cap(cur)
		{
			Onboard = new int[Nl_cap];
			//initialise list of priorities to -1
			for(int i = 0; i < Nl_cap; i++)
				Onboard[i] = -1;
			cout << "Elevator:: A lift has been created!\n";
			cout << "Elevator:: Lift's current capacity = " << this->cur_cap << " and max capacity = " << this->Nl_cap << "!\n\n";
		}

Elevator::~Elevator()
{
	delete[] Onboard;
	cout << "\nElevator:: No more ups and downs!\n\n";
}

void Elevator::get_cap(int &c_cap, int &m_cap) const
{
	c_cap = this->cur_cap;
	m_cap = this->Nl_cap;
}

void Elevator::get_max_cap(int &m_cap) const
{
	m_cap = this->Nl_cap;
}

void Elevator::get_cur_cap(int &c_cap) const
{
	c_cap = this->cur_cap;
}

void Elevator::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}

void Elevator::get_onboard(int * list)
{
	int lift_size;

	get_max_cap(lift_size);
	for(int i = 0; i < lift_size; i++)
		list[i] = Onboard[i];
}

void Elevator::set_onboard(int * list)
{
	int lift_size;

	get_max_cap(lift_size);
	for(int i = 0; i < lift_size; i++)
		Onboard[i] = list[i];
}

void Elevator::print(const int &floor, const int &direction) const
{
	int lift_size;

	cout << "Elevator:: Current Passengers : [";
	get_max_cap(lift_size);
	for(int i = 0; i < lift_size; i++)
	{
		cout << Onboard[i];
		if(i < lift_size - 1)
			cout << ", ";
	}
	cout << "] at Floor[" << floor << "]";
	if(direction == 1)
	{
		if(floor < 4)
			cout << ", lift going up!\n";
		else
			cout << ", lift reached the top floor!\n";
	}	
	else if(direction == -1)
		if(floor > 0)
			cout << ", lift going down!\n";
}

void Elevator::exit(const int & exited)
{
	int c_cap;

	get_cur_cap(c_cap);
	c_cap -= exited;
	set_cap(c_cap);
}

int Elevator::enter(Citizen * Next_Passenger, int * passenger_list)
{
	int c_cap, m_cap;
	int priority;
	get_cap(c_cap, m_cap);

	Next_Passenger->get_priority(priority);
	//check capacity
	if(c_cap >= m_cap)
	{
		cout << "Elevator:: You are not allowed to enter " << priority << "!\n";
		return 0;
	}
	else
	{
		cout << "Entrance:: Citizen has exited the ground waiting area and is boarding the lift!\n";
		for(int i = 0; i < m_cap; i++)
		{
			if(passenger_list[i] == -1)//sit here plz
			{
				passenger_list[i] = priority;
				break;
			}
		}
		c_cap++;
		set_cap(c_cap);
		cout << "Elevator:: Visitor in the lift with priority number: " << priority << "!\n";
		return 1;
	}
}

int Elevator::stop_up(Citizen ** Passengers, int * passenger_list, int * dropped_list, int &floor, int &cur_cap, int &max_cap)
{
	int priority;
	int drops = 0;
	int dest;
	int min = INT_MAX;
	int max = -1;
	int cur;
	//find minimum and maximum priority of currently boarded to get their dests
	for(int i = 0; i < this->Nl_cap; i++)
	{
		if(Onboard[i] != -1)//full seat
		{
			cur = Onboard[i];
			if(cur > max)
				max = cur;
			if(cur < min)
				min = cur;
		}	
	}
	if(max == -1 && min == INT_MAX)
		return drops;//lift empty
	//got the limits try unload next passenger based on priority
	for(int i = min; i <= max; i++)
	{
		Passengers[i-1]->get_priority(priority);
		Passengers[i-1]->get_floor(dest);
		if(dest != floor)//not my floor
			continue;
		else//is there room to get out//EDO EINAI TO LATHOS TROEI AKURO KAI MENEI EDO ENO UPARXEI XOROS APLA
		{
			if(cur_cap < max_cap)//there is room
			{
				//took from here
				for(int j = 0; j < this->Nl_cap; j++)//lets find his seat
				{
					if(passenger_list[j] == priority)//found him
					{
						dropped_list[drops] = priority;
						drops++;
						passenger_list[j] = -1;
						cout << "Citizen:: " << priority << " Entering floor number[" << floor << "]!\n";
						cur_cap++;
						break;
					}
				}		
			}
			else//floor is full
			{
				cout << "Elevator:: Sorry, priority " << priority <<",floor number " << floor << " is full\n";
				continue;
			}
		}
	}
	return drops;	
}

void Elevator::stop_down(int * passenger_list, int * exit_list)
{
	for(int i = 0; i < this->Nl_cap; i++)
	{
		if(passenger_list[i] == -1)//empty elevator seat
		{
			for(int j = 0; j < this->Nl_cap; j++)
			{
				if(exit_list[j] != -1)//valid exit target
				{
					passenger_list[i] = exit_list[j];
					exit_list[j] = -1;
					break;
				}
			}
		}
	}
}

int Elevator::empty_all(int * waiting, int * passenger_list, int * exit_list)
{
	int total = 0;
	int flag;
	int target;

	for(int i = 0; i < this->Nl_cap; i++)//check the list with done citizens
	{
		flag = 0;
		if(passenger_list[i] != -1)//avoid empty seats
		{
			target = passenger_list[i];
			for(int j = 0; j < this->Nl_cap; j++)
				if(waiting[j] == target)//not me im not done
					flag = 1;
			if(flag == 0)//picked correctly! he is done
			{
				total++;
				exit_list[i] = passenger_list[i];
				passenger_list[i] = -1;
			}
		}
	}

	return total;
}
//functions of class : building
Building::Building(int N = 0, int Nf = 0, int Ng = 0, int No = 0, int Nl = 0, int cur = 0)
	: N_cap(N), Nf_cap(Nf), Ng_cap(Ng), No_cap(No), Nl_cap(Nl), cur_cap(cur)
		{
			//create the ground floor
			Ground_Floor = new Ground(Ng_cap, 0, 0);
			//create the 4 floors
			Floors = new Floor*[4];
			for(int i = 0; i < 4; i++)
				Floors[i] = new Floor(Nf_cap, No_cap, 0, i + 1);
			//create the elevator
			Lift = new Elevator(Nl_cap, 0);
			//all floors and lift dynamically ready
			cout << "Building:: A new building is ready for serving citizens!\n";
			cout << "Building:: Building current capacity = " << this->cur_cap << " and max capacity = " << this->N_cap << "!\n\n";
		}

Building::~Building()
{	//detonate the building safely
	delete Lift;
	for(int i = 0; i < 4; i++)
        delete Floors[i];
    delete[] Floors;
    delete Ground_Floor;
	cout << "Building:: Service not available any longer. Go elsewhere!\n";
}

void Building::get_cap(int &c_cap, int &m_cap) const
{
	c_cap = this->cur_cap;
	m_cap = this->N_cap;
}

void Building::get_cur_cap(int &c_cap) const
{
	c_cap = this->cur_cap;
}

void Building::set_cap(const int &c_cap)
{
	this->cur_cap = c_cap;
}

bool Building::enter()
{
	int c_cap, m_cap;

	get_cap(c_cap, m_cap);
	if(c_cap >= m_cap)
	{
		cout << "Building:: Please, come tomorrow!\n";
		return 0;
	}
	else
	{
		c_cap++;
		set_cap(c_cap);
		cout << "Building:: Citizen got in!\n";
		return 1;
	}
}

void Building::exit(int * Done_Passengers)
{
	for(int i = 0; i < this->Nl_cap; i++)
	{
		if(Done_Passengers[i] != -1)
		{
			cout << "Building:: Citizen happily exiting the building!\n";
			cout << "Citizen:: I cannot believe I finished with priority just: " << Done_Passengers[i] << " !\n";
			Done_Passengers[i] = -1;
		}
	}
}

bool Building::g_enter(Citizen *next)
{
	int c_cap, m_cap, b_cap;
	Ground_Floor->get_cap(c_cap, m_cap);
	if(c_cap >= m_cap)
	{
		cout << "Building:: Ground floor full, plz wait!\n";
		return 0;
	}
	else
	{
		get_cur_cap(b_cap);//update building cap
		b_cap--;
		set_cap(b_cap);
		c_cap++;
		Ground_Floor->set_cap(c_cap);
		cout << "Building:: Citizen got in Ground floor!\n";
		Ground_Floor->enter();//try get in waiting area
		Ground_Floor->wait(next);//get priority ticket
		return 1;
	}
}

void Building::operate(Citizen ** Passengers, int * Done_Passengers)
{
	int *passenger_list;
	int *dropped_list;
	int *exit_list;
	int *waiting;
	int boarded = 0;
	int result;
	int dropped;
	int lift_size;
	int floor_c_cap;
	int g_wait_cap;
	int lc_cap, lm_cap;
	int cap;
	int cur_floor;
	int *ground_list;
	int min;
	int pos;
	//cycle starts
	Lift->get_max_cap(lift_size);
	passenger_list = new int[lift_size];//current list of passengers
	dropped_list = new int[lift_size];//list of passengers trying to enter a floor
	exit_list = new int[lift_size];//list of passengers who finished with their service
	waiting = new int[lift_size];//list of passengers who failed to enter a full floor and wait for the next cycle
	ground_list = new int[Ng_cap];
	for(int i = 0; i < lift_size; i++)//-1 = empty spot on each list
	{
		dropped_list[i] = -1;
		exit_list[i] = -1;
		waiting[i] = -1;
	}
	for(int i = 0; i < Ng_cap; i++)//initialised @ -1
		ground_list[i] = -1;
	Lift->get_onboard(passenger_list);//get current passenger list
	Ground_Floor->get_cur_cap(g_wait_cap);//get number of waiting for lift
	Ground_Floor->get_list(ground_list);
	for(int i = 0; i < g_wait_cap; i++)//try get on the lift
	{
		min = INT_MAX;//find minimum waiting in ground
		pos = -1;

		for(int j = 0; j < Ng_cap; j++)
		{
			if(ground_list[j] < min)
			{
				if(ground_list[j] != -1)//avoid empty
				{
					min = ground_list[j];
					pos = j;
				}
			}
		}
		result = Lift->enter(Passengers[min-1], passenger_list);//enter minimum
		if(result == 1)//passenger boarded
		{
			boarded += result;
			ground_list[pos] = -1;//erase from waiting
			Ground_Floor->remove(min);
		}
		ground_list[pos] = -1;//erase from waiting either way
	}
	g_wait_cap-= boarded;
	Ground_Floor->exit(boarded);//they on lift now
	Ground_Floor->set_cur_cap(g_wait_cap);//entrance fix
	Lift->set_onboard(passenger_list);//update passenger list
	Lift->print(0, 1);
	for(int i = 1; i <= 4; i++)
	{
		Floors[i-1]->get_w_cap(floor_c_cap);//get floor cap
		dropped = Lift->stop_up(Passengers, passenger_list, dropped_list, i, floor_c_cap, this->Nf_cap);//stop at next floor and attempt to unload
		Lift->set_onboard(passenger_list);//update list
		Floors[i-1]->enter(Passengers, dropped_list, this->Nl_cap);//enter on stop up
		Lift->print(i, 1);
		Lift->exit(dropped);//set elevator new cap based on dropped passengers both waiting in floor area and inside office
	}
	cout << "\n";
	for(int k = 0; k < this->Nl_cap; k++)//find the passengers that have to wait the next cycle to enter their floors/offices
		waiting[k] = passenger_list[k];
	Lift->set_onboard(waiting);//update with the passengers left
	for(int i = 4; i >= 1; i--)
	{
		Lift->get_cap(lc_cap, lm_cap);//get lift capacity
		cap = lm_cap - lc_cap;//number of seats available to board descending passengers who are done
		cur_floor = i - 1;
		result = Floors[i-1]->exit(cap, cur_floor, exit_list, this->Nl_cap);//find exit targets and stop down
		Lift->stop_down(passenger_list, exit_list);//at floor i
		Lift->set_onboard(passenger_list);//update based on boarded happy citizens
		Lift->print(i, -1);
		lc_cap += result;//count number of done passengers
		Lift->set_cap(lc_cap);//update lift capacity for next stop
	}
	result = Lift->empty_all(waiting, passenger_list, Done_Passengers);
	Lift->set_onboard(waiting);//update list with the ones that will be inside in the next cycle as priority passengers
	Lift->exit(result);
	Lift->print(0, 1);
	//free the arrays
	delete[] ground_list;
	delete[] waiting;
	delete[] exit_list;
	delete[] dropped_list;
	delete[] passenger_list;
	//cycle ends
}
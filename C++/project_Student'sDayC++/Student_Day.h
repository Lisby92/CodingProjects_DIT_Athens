/*************************************************************************
Implementation File	: Student_Day.h
Author Date			: V. Mavrommatis 1/12/2018
Purpose				: Header file of Student_Day.cpp containing definitions
Revision			: < Vasileios_Mavrommatis - 4/12/2018 >
**************************************************************************/
#ifndef __Student_Day__
#define __Student_Day__
/* Definition of class teacher : Simulates a teacher */
class Teacher
{
	private:
		int id;//teacher identifier(1-N)
		bool available;//availability of teacher

	public:
		Teacher(int, bool);//constructor
		~Teacher();//destructor
		void get_id(int &) const;//get teachers id to distinguish
		bool teacher_available() const;//teacher available?
		void set_available(const bool &);//set/////////////////////////
};
/* Definition of class Parent : Simulates a parent */
class Parent
{
	private:
		int *Teachers_2meet;//list of teachers
		int id;//parent identified(random)
		int tea_available;//number of teachers left to visit
		int left;//teachers left to meet

	public:
		Parent(int, int, int);//constructor
		~Parent();//destructor
		void get_list(int *) const;//get list of teachers parent wants to talk to
		void set_list(const int &);//set ////////////////////////////////////////
		void get_id(int &) const;//get parents id to praise or condemn
		void remove(const int &);//remove teacher after talking to him
		bool remaining() const;//more teachers remaining?
		bool find_on_list(const int &) const;//found input teacher on list of teachers?
};
/* Definition of class Queue : Simulates a parents queue */
class Queue
{
	private:
		int size;//queue size
		int front;//queue first element
		int rear;//queue last element
		int *Parents;//array of parent indexes handled with queue functions
		int current;//position of next parent in our priority queue
		int max;//biggest element of queue

	public:
		Queue(int, int, int, int, int, int);//constructor
		~Queue();//destructor
		bool empty() const;//is queue empty?
		bool full() const;/////////// full?
		void rear_enqueue(const int &);//parent enqueued at rear
		void front_enqueue(const int &);//parent enqueued at front
		int get_current() const;//get current parent
		void next_current();//set current to next parent
		void reset_current();//reset current to front parent
		void print() const;//print queue elements
		bool mood_roulette() const;//give parents a random mood
		void mood_express(const bool &)const;//express feelings of waiting parents
		void dequeue(const int &);//remove parent from the queue
		void get_max(int &) const;//get last parent to enter the queue for the first time
		void get_size(int &) const;//get current queue size
};
/* Definition of class Classroom : Simulates a classroom */
class Classroom
{
	private:
		int par_available;//counter of available parents
		int tea_available;//number of available teachers
		int *teachers;//array of teachers available
		Queue *queue;//the entry queue with the parents

	public:
		Classroom(int, int);//constructor
		~Classroom();//destructor
		void get_available(int &) const;//get available parents
		void set_available(const int &);//set /////////////////
		bool check_available() const;///check /////////////////
		void early_enter(const int &);//enter the first L early parents
		void start(Teacher **, Parent **);//driver function of the Student Day
};
#endif
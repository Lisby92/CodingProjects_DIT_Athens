/*************************************************************************
Implementation File	: Student_Day.cpp
Author Date			: V. Mavrommatis 1/12/2018
Purpose				: Implementation of Class and subclass(es) functions
Revision			: < Vasileios_Mavrommatis - 4/12/2018 >
**************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include "Student_Day.h"

using namespace std;
/* FUNCTIONS OF TEACHER */
Teacher::Teacher(int tea_id = 0, bool avail = 1)
: id(tea_id), available(avail)
{
	cout << "A teacher with id: " << id << " has been constructed!\n";
}

Teacher::~Teacher()
{
	cout << "A teacher with id: " << id << " has been destroyed after meeting all the parents!\n";
}

void Teacher::get_id(int & tea_id) const
{
	tea_id = id;
}

bool Teacher::teacher_available() const
{
	return (available == 1);
}

void Teacher::set_available(const bool & status)
{
	available = status;
}
/* FUNCTIONS OF PARENT */
Parent::Parent(int par_id = 0, int avail = 0, int le = 0)
: id(par_id), tea_available(avail), left(le)
{
	Teachers_2meet = new int[tea_available];
	for(int i = 0; i < tea_available; i++)
		Teachers_2meet[i] = 0;//list of teachers parent wants to meet initalised @ 0
	cout << "A parent with id: " << id << " has been constructed!\n";
}

Parent::~Parent()
{
	delete[] Teachers_2meet;
	cout << "A parent with id: " << id << " has been destroyed after meeting all the teachers!\n";
}

void Parent::get_list(int * list) const
{
	for(int i = 0; i < tea_available; i++)
		list[i] = Teachers_2meet[i];
}

void Parent::set_list(const int & random)
{
	int i = 0;
	int random_teacher;
	int flag;
	//chance of wanting to meet a teacher = random
	while(i < random)
	{
		flag = 0;
		random_teacher = rand() % (tea_available + 1 - 1) + 1;//teacher id generator(1~N)
		for(int k = 0; k < tea_available; k++)//exclude teachers allready chosen
			if(Teachers_2meet[k] == random_teacher)
				flag = 1;
		if(flag == 0)
		{
			for(int j = 0; j < tea_available; j++)
			{
				if(Teachers_2meet[j] == 0)//empty spot
				{
					Teachers_2meet[j] = random_teacher;
					left ++;
					cout << "A new random teacher was chosen with id : " << random_teacher << " !\n";
					i++;
					break;
				}
			}
		}	
	}
}

bool Parent::find_on_list(const int & t_id) const
{
	for(int i = 0; i < tea_available; i++)
		if(t_id == Teachers_2meet[i])
			return 1;
	return 0;	
}

void Parent::remove(const int & teach_id)
{
	for(int i = 0; i < tea_available; i++)
	{
		if(Teachers_2meet[i] == teach_id)
		{
			Teachers_2meet[i] = 0;
			left --;
			break;
		}
	}
}

bool Parent::remaining() const
{
	return(left > 0);
}

void Parent::get_id(int & par_id) const
{
	par_id = id;
}
/* FUNCTIONS OF QUEUE */
Queue::Queue(int sz = 0, int fr = -1, int re = -1, int cur = 0 ,int array_sz = 10, int m = 0)
: size(sz), front(fr), rear(re) ,current(cur), max(m)
{
	Parents = new int[array_sz];
	for(int i = 0; i < array_sz; i++)
		Parents[i] = -1;
	cout << "A Queue has been constructed!\n";
}

Queue::~Queue()
{
	delete[] Parents;
	cout << "A Queue has been destroyed!\n";
}

bool Queue::empty() const
{
	return (size == 0);
}

bool Queue::full() const
{
	return (size > 0);
}

void Queue::rear_enqueue(const int & position)
{
	cout << "Rear Enqueuing Parent: " << position << " \n";
	if(empty())
	{
		front = rear = 0;
		max = position;
	}
	else
	    rear++;
	Parents[rear] = position;
	size++;
	if(position > max)
		max = position;
}

void Queue::front_enqueue(const int & position)
{
	cout << "Front Enqueuing Parent: " << position << " \n";
	if(empty())
	{
		front = rear = 0;
		max = position;
	}
	else
	{
	    for(int i = size; i > 0; i--)
	    	Parents[i] = Parents[i-1];//shift right
	    rear++;

	}
	Parents[front] = position;
	size++;
	if(position > max)
		max = position;
}

void Queue::dequeue(const int & pos)
{
	int mark;

	cout<<"Dequeuing Parent: " << pos << endl;
	if(empty())
		cout<<"Error: Queue is Empty\n";
	else
	{
		for(int i = 0; i < size; i++)
		{
			if(pos == Parents[i])
			{
				mark = i;
				break;
			}
		}
		for(int i = mark; i < (size - 1); i++)//shift left
			Parents[i] = Parents[i+1];
		size--;
		rear--;
		if(empty())
		{
			rear = -1;
			front = -1;
		}
	}
}

int Queue::get_current() const
{
	return Parents[current];
}

void Queue::next_current()
{
	current++;
}

void Queue::reset_current()
{
	current = front;
}

void Queue::print() const
{
	cout<<"Queue: ";
	for(int i = 0; i <size; i++)
		cout << Parents[i] << " ";
	cout<<"\n\n";
}

bool Queue::mood_roulette() const
{
	int rand1 = rand();
	int rand2 = rand();

	if(rand1 > rand2)
		return 1;//to be praised!
	else
		return 0;//to be condemned!
}

void Queue::mood_express(const bool & m) const
{//had more prints for every parent in queue but filled too much space in output so all in 1
	if(full())//no need for loud thinking
	{
		if(m)
		//for(int i = 0; i < (size-1); i++)
		//cout << Parents[i] << " :: You are so polite, unlike the others!\n";
			cout << "SO POLITE!\n";
		else
		//for(int i = 1; i < size; i++)
			cout << "HOW RUDE!!\n";
			//cout << Parents[i] << " :: Hey, it's my turn!\n";	
	}
}

void Queue::get_max(int & mx) const
{
	mx = max;
}

void Queue::get_size(int & sz) const
{
	sz = size;
}
/* FUNCTIONS OF CLASSROOM */
Classroom::Classroom(int avail1 = 0, int avail2 = 0)
: par_available(avail1), tea_available(avail2)
{	//create the teachers list(array of integer positions)
	teachers = new int[tea_available];
	for(int i = 0; i < tea_available; i++)
		teachers[i] = 0;
	queue = new Queue(0, -1, -1, 0, par_available, 0);
	cout << "A classroom to host the Student_Day has been constructed!\n";
}

Classroom::~Classroom()
{
	delete queue;
    delete[] teachers;
    cout << "A classroom has been destroyed!\n";
}

void Classroom::get_available(int & status) const
{
	status = par_available;
}

void Classroom::set_available(const int & status)
{
	par_available = status;
}

bool Classroom::check_available() const
{
	return (par_available > 0);
}

void Classroom::early_enter(const int & L)
{	//enqueue the early parents
	for(int i = 0; i < L; i++)
	{
		queue->rear_enqueue(i);
		queue->print();
	}
}

void Classroom::start(Teacher ** Teachers, Parent ** Parents)
{
	int flag = 0;//exits loop
	int pos;//current parent
	int L;//new to come
	int avail;//available parents
	int last;//last entering queue for first time
	int R;//new parent expectancy
	int q_size;//current queue size
	//PART 1 : more parents expected to come
	while(flag == 0)
	{
		for(int i = 1; i <= tea_available; i++)//for all the teachers
		{
			if(Teachers[i-1]->teacher_available())//teacher is available
			{	//get next parrent from the queue
				pos = queue->get_current();
				if(Parents[pos]->find_on_list(i))//found him
				{
					Teachers[i-1]->set_available(0);//make him unavailable
					Parents[pos]->remove(i);
					queue->dequeue(pos);//temporarily?
					queue->print();
					get_available(avail);
					if(avail > 0)//more room
					{
						L = rand() % (avail + 1 - 1) + 1;//get random number of new parents on queue
						queue->get_max(last);
						for(int j = (last+1); j <= (L + last); j++)
						{
							queue->rear_enqueue(j);
							queue->print();
						}
						R = avail - L;
						set_available(R);
					}
					else//no more to come
						flag = 1;
					Teachers[i-1]->set_available(1);//teacher done talking
					if(Parents[pos]->remaining())//want to see more teachers
					{
						if(queue->mood_roulette())//parent is ok
						{
							cout << pos << ":: Thank you teacher " << i << " ! Allright guys, next up!\n";
							queue->rear_enqueue(pos);
							queue->mood_express(1);
						}
						else//parent misbehaves
						{
							cout << pos << ":: Thank you teacher " << i << " ! Uhhhm, now if i get to speak to...\n";
							queue->front_enqueue(pos);
							queue->mood_express(0);
						}
						queue->print();
					}
					else//parent finished
					{
						cout << pos << ":: Thank you teacher " << i << " !\n";
						cout << pos << ":: Finally I'm done! Time to find my kids...\n";
					}
					if(flag == 1)
					{
						queue->reset_current();
						break;
					}
				}
				else//not on my list get next
					queue->next_current();
			}
			queue->reset_current();
		}
		queue->reset_current();
	}
	get_available(avail);
	cout << avail << " parents expected to come!\n";
	queue->get_size(q_size);
	cout << q_size << " parents remaining in queue!\n";
	//PART 2 : no more parents coming
	flag = 0;
	if(q_size == 0)
	{
		flag = 1;
	}
	while(flag == 0)
	{
		for(int i = 1; i <= tea_available; i++)//for all the teachers
		{
			if(Teachers[i-1]->teacher_available())//teacher is available
			{	//get next parrent from the queue
				pos = queue->get_current();
				if(Parents[pos]->find_on_list(i))//found him
				{
					Teachers[i-1]->set_available(0);//make him unavailable
					Parents[pos]->remove(i);
					queue->dequeue(pos);//temporarily?
					queue->print();
					Teachers[i-1]->set_available(1);//teacher done talking
					if(Parents[pos]->remaining())//want to see more teachers
					{
						if(queue->mood_roulette())//parent is ok
						{
							cout << pos << ":: Thank you teacher " << i << " ! Allright guys, next up!\n";
							queue->rear_enqueue(pos);
							queue->mood_express(1);
						}
						else//parent misbehaves
						{
							cout << pos << ":: Thank you teacher " << i << " ! Uhhhm, now if i get to speak to...\n";
							queue->front_enqueue(pos);
							queue->mood_express(0);
						}
						queue->print();
						queue->get_size(q_size);
						if(q_size == 0)
						{
							flag = 1;
							break;
						}
					}
					else//parent finished
					{
						cout << pos << ":: Thank you teacher " << i << " !\n";
						cout << pos << ":: Finally I'm done! Time to find my kids...\n";
						queue->get_size(q_size);
						if(q_size == 0)
						{
							flag = 1;
							break;
						}
					}
				}
				else//not on my list get next
					queue->next_current();
			}
			queue->reset_current();
		}
		queue->reset_current();
	}
	
}
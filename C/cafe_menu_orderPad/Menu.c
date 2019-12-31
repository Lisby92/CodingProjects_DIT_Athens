/*************************************************************************
Implementation File	: Menu.c
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: ATD Menu with linked list and head
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#include <stdlib.h>
#include "Menu.h"

struct InfoMenu {			// o pliroforiakos komvos
	char* 	  Name; 	   // to onoma tou estiatoreiou
	MenuNodePtr Start;	   // deiktis ston proto komvo ths anoterhs kathgorias
	MenuNodePtr Current;   // deiktis ston trexonta komvo
	MenuNodePtr CutToPaste;// deiktis ston komvo pros metafora
};

struct MenuNode {	//o komvos gia kathgoria h piato
	MenuNodePtr Prev, Next, Up, Down; //prev, next = deiktes gia dipla synd.lista
	TStoixeiouMenu Data;		//up,down = oi deiktes gia kathgoria-ypokathgoria
};

//================ MENU_FUNCTIONS ===================//

MenuNodePtr menu_node_creation()
{	//Called by menu_creation(), menu_add_node(), menu_add_subcategory()
	//Creates and initializes a node and returns a pointer to its address
	MenuNodePtr new_node;

	new_node = malloc(sizeof(MenuNode));
	if(new_node == NULL)
	{
		printf("\nSorry insufficient RAM\n");
		exit(1);
	}
	new_node->Prev = NULL;
	new_node->Next = NULL;
	new_node->Up = NULL;
	new_node->Down = NULL;

	return new_node;
}

InfoMenuPtr menu_creation(char * name)//to name to diavazeis aptin main kai pernas deikti sto proto stoixeio tou string sthn sunarthsh
{	//BEFORE : Nothing
	//We call this function first so that all other options can be selected afterwards
	//Creates menus named after command line arguments client-side and returns a pointer to the address of their info node
	InfoMenuPtr new_menu;
	MenuNodePtr position;

	new_menu = malloc(sizeof(InfoMenu));
	if(new_menu == NULL)
	{
		printf("\nSorry insufficient RAM\n");
		exit(1);
	}
	position = menu_node_creation(); //allocate and initialiZe head node
	new_menu->Name = name;
	new_menu->Start = position;
	new_menu->Current = position;
	new_menu->CutToPaste = NULL;

	return new_menu;
}

int menu_empty_category(InfoMenuPtr info_ptr)
{	//BEFORE : A level must exist.
	//Checks for empty level, returns 1 if empty or 0 else.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;
	else
		return ((info_ptr->Current->Next == NULL) && (info_ptr->Current->Prev == NULL));
}

int menu_add_node(InfoMenuPtr info_ptr, TStoixeiouMenu V)//ta proionta exoun panta previous mexri kefali kai down null kai Up se komvo kathgoria kai next se epomeno proion idias kathgorias
{	//BEFORE : At least a head node must exist.
	//Adds a node at any point after Current_ptr in current lvl.
	//Current_ptr moves to the new node.
	if(info_ptr == NULL) //havent created menu yet = error
		return 1;
	else
	{
		MenuNodePtr new_node;

		new_node = menu_node_creation();
		TSSetValue(&new_node->Data, V);

		if(info_ptr->Current->Next == NULL) // current->next is empty
		{
			new_node->Prev = info_ptr->Current;	//new plate points to previous
			info_ptr->Current->Next = new_node; //next of current points to new plate
		}
		else // current->next isnt empty so have to update that node also
		{
			new_node->Next = info_ptr->Current->Next;
			new_node->Prev = info_ptr->Current;
			info_ptr->Current->Next->Prev = new_node;
			info_ptr->Current->Next = new_node;
		}
		new_node->Up = info_ptr->Current->Up; //update up pointers to upper lvl
		info_ptr->Current = new_node;	//current points to new node
		return 0; //all ok
	}
}

int menu_show_content(InfoMenuPtr info_ptr)
{	//BEFORE : Any kind of node must exist.
	//Prints content of TS struct.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;
	else if(info_ptr->Current->Prev == NULL) //if current points to a head node do nothing.
		return 2; //error 
	else
	{
		TSPrintValue(info_ptr->Current->Data);
		return 0; // all ok
	}
}

int menu_change_content(InfoMenuPtr info_ptr, TStoixeiouMenu V)
{	//BEFORE : A non-head node must exist.
	//Changes content of the current non-head node.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;	
	else if(info_ptr->Current->Prev == NULL) //current points to a head = error
		return 1;
	else
	{
		TSSetValue(&info_ptr->Current->Data, V);
		return 0;
	}
}

int menu_next(InfoMenuPtr info_ptr)
{	//BEFORE : A head node must exist.
	//Moves Current_ptr to the next available node in the same level.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Next != NULL) //there is next node
	{
		info_ptr->Current = info_ptr->Current->Next;
		return 0;
	}
	return 1; //there isnt next node
}

int menu_previous(InfoMenuPtr info_ptr)
{	//BEFORE : A head node must exist.
	//Moves Current_ptr to the previous available node in the same level.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev != NULL) //there is previous node
	{
		info_ptr->Current = info_ptr->Current->Prev;
		return 0;
	}
	return 1; //there isnt previous node
}

int menu_swap_next(InfoMenuPtr info_ptr)
{	//BEFORE : At least 2 non-head nodes must exist in same level.
	//Swaps position with the node right after Current_ptr.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev == NULL) //we are at head node so wont swap = error
		return 1;
	else if(info_ptr->Current->Next == NULL) // there is no next node to swap with
		return 2;
	else//we can swap the nodes
	{
		MenuNodePtr temp;
		if(info_ptr->Current->Next->Next == NULL)//the node we want to swap with is the last one
		{
			info_ptr->Current->Prev->Next = info_ptr->Current->Next;
			info_ptr->Current->Next->Prev = info_ptr->Current->Prev;
			info_ptr->Current->Next->Next = info_ptr->Current;
			info_ptr->Current->Prev = info_ptr->Current->Next;
			info_ptr->Current->Next = NULL;
		}
		else //the node we want to swap with is not the last
		{
			info_ptr->Current->Prev->Next = info_ptr->Current->Next;
			info_ptr->Current->Next->Prev = info_ptr->Current->Prev;
			info_ptr->Current->Next->Next->Prev = info_ptr->Current;
			temp = info_ptr->Current->Next->Next;
			info_ptr->Current->Next->Next = info_ptr->Current;
			info_ptr->Current->Prev = info_ptr->Current->Next;
			info_ptr->Current->Next = temp;
		}
		return 0;
	}
}

int menu_swap_previous(InfoMenuPtr info_ptr)
{	//BEFORE : At least 2 non-head nodes must exist in same level.
	//Swaps position with the node right before Current_ptr.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev == NULL)//we are at head node = error
		return 1;
	else if(info_ptr->Current->Prev->Prev == NULL)//we try to swap with a head node = error
		return 2;
	else //we can swap the nodes
	{
		MenuNodePtr temp;
		if(info_ptr->Current->Next == NULL) // the current node is the last node
		{
			info_ptr->Current->Next = info_ptr->Current->Prev;
			info_ptr->Current->Prev->Next = NULL;
			info_ptr->Current->Prev->Prev->Next = info_ptr->Current;
			temp = info_ptr->Current->Prev->Prev;
			info_ptr->Current->Prev->Prev = info_ptr->Current;
			info_ptr->Current->Prev = temp;
		}
		else // current node is not the last
		{
			info_ptr->Current->Next->Prev = info_ptr->Current->Prev;
			info_ptr->Current->Prev->Prev->Next = info_ptr->Current;
			temp = info_ptr->Current->Prev->Prev;
			info_ptr->Current->Prev->Prev = info_ptr->Current;
			info_ptr->Current->Prev->Next = info_ptr->Current->Next;
			info_ptr->Current->Next = info_ptr->Current->Prev;
			info_ptr->Current->Prev = temp;
		}
		return 0; //all ok
	}
}

int menu_add_sub_category(InfoMenuPtr info_ptr)
{	//BEFORE : A non-head node must exist.
	//Adds a head node at a new level beneath Current_ptr and updates it and the upper level also.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev == NULL) // we are at head so wont add = error
		return 1;
	else
	{
		MenuNodePtr head_node;

		head_node = menu_node_creation(); //we need a new head to link new subcategory.
		head_node->Up = info_ptr->Current;	//head node up points to current
		info_ptr->Current->Down = head_node; //current down points to head
		return 0;
	}
}
int menu_category_up(InfoMenuPtr info_ptr)
{	//BEFORE : An upper level we can ascend must exist.
	//Moves Current_ptr to an existing upper level.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Up != NULL)//this is a subcategory so there is Up
	{
		info_ptr->Current = info_ptr->Current->Up;
		return 0; //all ok
	}
	else
		return 1;//this is not a subcategory = error

}
int menu_category_down(InfoMenuPtr info_ptr)
{	//BEFORE : A sub-level we can descend must exist.
	//Moves Current_ptr to an existing  sub-level.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Down != NULL)//there is a sub-level
	{
		info_ptr->Current = info_ptr->Current->Down;
		return 0; //all ok
	}
	else
		return 1;//there is not a sub-level = error
}

int menu_print_chain(InfoMenuPtr info_ptr)
{	//BEFORE : A non-empty level's head must exist.
	//Prints the content of every node in this level along with every sub-level(s) each node has with recursion.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev != NULL)// not at head node = error
		return 1;

	if(info_ptr->Current->Next == NULL)// nothing to print as next
		return 2;

	MenuNodePtr temp, root;

	root = info_ptr->Current; //hold the root head where we start

	while(!menu_next(info_ptr))//while there are nodes in this lvl
	{
		menu_show_content(info_ptr);//print data
		if(info_ptr->Current->Down != NULL) //we can go further down
		{
			printf("\n");
			temp = info_ptr->Current; //hold beginning
			menu_category_down(info_ptr);//go down
			menu_print_chain(info_ptr); //recurse ya
			info_ptr->Current = temp; //go back to begin
		}
		if(info_ptr->Current->Next != NULL) //still more nodes to print
			printf("\n");
	}
	info_ptr->Current = root; //back to root head node
	return 0; //all ok exit
}	

int menu_delete_next(InfoMenuPtr info_ptr)
{	//BEFORE : A non-empty level's head must exist.
	//Deletes the node right after Current_ptr.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Next != NULL)// there is next node to delete
	{
		if(info_ptr->Current->Next->Down == NULL)// next node doesnt have subcategories
		{
			MenuNodePtr temp;
			temp = info_ptr->Current->Next;//to be deleted
			if(info_ptr->Current->Prev == NULL)//we are at head node
			{
				if(temp->Next == NULL)//there is only one node after head
					info_ptr->Current->Next = NULL;//head points to null
				else //there are at least 2 nodes
				{
					info_ptr->Current->Next = temp->Next;
					temp->Next->Prev = info_ptr->Current;
				}
			}
			else//we are not at head node
			{
				if(temp->Next == NULL)//the node to be deleted is last
					info_ptr->Current->Next = NULL;
				else //there are at least 2 nodes
				{
					info_ptr->Current->Next = temp->Next;
					temp->Next->Prev = info_ptr->Current;
				}
			}
			free(temp);
			temp = NULL;
			return 0; // all ok
		}
		else
			return 2; //next node has subcategories = error
	}
	else
		return 1; //current node has no next = error
}

int menu_delete_down(InfoMenuPtr info_ptr)
{	//BEFORE : At least one sub-level must exist.
	//Deletes all sub-levels beneath Current_ptr.
	//Current_ptr doesn't move.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;

	if(info_ptr->Current->Prev == NULL)
		return 1; //head nodes cant have subcategories
	else if(info_ptr->Current->Down == NULL)
		return 2; //current node doesnt have subcategories

	MenuNodePtr temp;

	if(info_ptr->Current->Down == NULL)//there isnt subcategory after this one
	{
		menu_category_down(info_ptr);//go down to subcategory's head
		temp = info_ptr->Current; //temp points to head
		while(temp->Next != NULL)//there is node to delete
			menu_delete_next(info_ptr);
		menu_category_up(info_ptr);//we done deleting all but head go up
		free(temp);
		temp = NULL;//free head
		info_ptr->Current->Down = NULL;//update upper category down
	}
	else//we can go further down
	{
		temp = info_ptr->Current->Down;//temp = head to first lvl subcategories
		menu_category_down(info_ptr);//go down to subcategory's head
		while(temp->Next != NULL)//we got work to do
		{
			menu_next(info_ptr);//go to first node after head
			menu_delete_down(info_ptr);//recurse ya
			menu_previous(info_ptr);//back to head
			menu_delete_next(info_ptr);//delete next
		}
		menu_category_up(info_ptr);
		info_ptr->Current->Down = NULL;//update upper category down
	}
	return 0; //all ok
}

int menu_cut_node(InfoMenuPtr info_ptr)
{	//BEFORE : A non-head node must exist.
	//Cuts node along with any sub-levels and updates both info-node CutToPaste_ptr, and previous or any next nodes of same level.
	//Current_ptr moves to previous node. 
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;
	else if(info_ptr->Current->Prev == NULL)
		return 1;//no point to cut a head node = error
	else//we can cut the node
	{
		if(info_ptr->Current->Next == NULL)//only one node exists or we try to cut last node
			info_ptr->Current->Prev->Next = NULL;  //previous becomes last ,or list = empty in case of one node
		else//we cutting a middlish node , both previous and next exist
		{
			info_ptr->Current->Prev->Next = info_ptr->Current->Next; //previous-next = next
			info_ptr->Current->Next->Prev = info_ptr->Current->Prev; //next-previous = previous
		}
		info_ptr->CutToPaste = info_ptr->Current;	//cuttopaste is current
		info_ptr->Current = info_ptr->Current->Prev; //current goes to previous
	}
	return 0;//all ok
}

int menu_paste_node(InfoMenuPtr info_ptr)
{	//BEFORE : A node must have been cut already.
	//Pastes the node we have cut right after Current_ptr and updates both info-node CutToPaste_ptr, and previous or any next nodes of same level.
	//Current_ptr moves to the node we just pasted.
	if(info_ptr == NULL) //havent created menu yet = error
		return 3;
	else if(info_ptr->CutToPaste == NULL)
		return 1;//no node has been cut...
	else//we can paste the node we have cut
	{
		if(info_ptr->Current->Next == NULL)//we want paste at end or as first in start
		{
			info_ptr->Current->Next = info_ptr->CutToPaste; //next is paste
			info_ptr->CutToPaste->Prev = info_ptr->Current; //paste's previous is current
			info_ptr->CutToPaste->Next = NULL;
		}
		else//we're pasting at middle
		{
			info_ptr->CutToPaste->Prev = info_ptr->Current; //update paste prev and next
			info_ptr->CutToPaste->Next = info_ptr->Current->Next;
			info_ptr->Current->Next->Prev = info_ptr->CutToPaste; //update the side nodes prev and next accordingly
			info_ptr->Current->Next = info_ptr->CutToPaste;
		}
		info_ptr->Current = info_ptr->CutToPaste;
		info_ptr->CutToPaste = NULL; //paste complete now empty it
	}
	return 0;//all ok
}

int menu_destruction(InfoMenuPtr * info_ptr)
{	//BEFORE : An info node must exist.
	//Destroys menu and frees all nodes ending with info node.
	if(*info_ptr == NULL) //havent created menu yet = error
		return 1;

	MenuNodePtr temp;

	(*info_ptr)->Current = (*info_ptr)->Start; //current points at start(1st head node)
	if((*info_ptr)->Current->Next != NULL)//menu is not empty
	{
		if((*info_ptr)->Current->Next->Down == NULL)//no subcategories
		{
			while((*info_ptr)->Current != NULL)		//while we havent reached end
			{
				temp = (*info_ptr)->Current;	 // temp = to be deleted
				(*info_ptr)->Current = (*info_ptr)->Current->Next; //go to next category/plate
				free(temp);
				temp = NULL;
			}
		}
		else//there are subcategories
		{
			while((*info_ptr)->Current != NULL)		//while we havent reached end
			{
				temp = (*info_ptr)->Current;	 // temp = to be deleted
				menu_delete_down(*info_ptr); //delete all subcategories down of current
				(*info_ptr)->Current = (*info_ptr)->Current->Next; //go to next category/plate
				free(temp);
				temp = NULL;
			}
		}
		(*info_ptr)->Start = NULL;
		(*info_ptr)->CutToPaste = NULL;
		free(*info_ptr);
		(*info_ptr) = NULL;
	}
	else //menu is empty
	{
		free((*info_ptr)->Current); //free the head 
		(*info_ptr)->Current = NULL;
		(*info_ptr)->Start = NULL;
		(*info_ptr)->CutToPaste = NULL;//nullify all
		free(*info_ptr);
		(*info_ptr) = NULL;//free info node
	}
	return 0;// all ok
}


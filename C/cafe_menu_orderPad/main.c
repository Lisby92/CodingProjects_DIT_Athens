/*************************************************************************
Implementation File	: main.c
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: Implementation of Palmreader Menus
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Configurations.h"
#include "Menu.h"
#include "TyposStoixeiouMenu.h"

//************ WLCM, DEAR CUSTOMER ! *************//

int main(int argc, char *argv[]) //arguments to pass from command line : first is name of restaurant, second name of cafeteria
{
	InfoMenuPtr 	FoodMenu = NULL;		//the restaurant's menu
	InfoMenuPtr		CafeMenu = NULL;		//the cafeteria's menu
	int 			option;					//choise between menu's available functions
	int 			result;	 				//error type handling
	TStoixeiouMenu 	V;

	do {
		option = get_option();
		switch (option) {
		case 1:
			FoodMenu = menu_creation(argv[1]);
			printf("\nFoodMenu ");
			puts(argv[1]); //prints menu name
			printf("Created successfully!");
			CafeMenu = menu_creation(argv[2]);
			printf("\nCafeMenu ");
			puts(argv[2]); //prints menu name
			printf("Created successfully!\n");
			break;
		case 2:
			if(which_menu()) //we want food menu
			{
				result = menu_empty_category(FoodMenu);
				if(result == 1)
					printf("\nCurrent Category is empty!\n");
				else if(result == 0)
					printf("\nCurrent Category is not empty!\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
			}
			else //we want cafe menu
			{
				result = menu_empty_category(CafeMenu);
				if(result == 1)
					printf("\nCurrent Category is empty!\n");
				else if(result == 0)
					printf("\nCurrent Category is not empty!\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
			}
			break;
		case 3:
			if(which_menu())
			{
				printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				while(TSReadValue(&V))
				{
					printf("\nError : Invalid input data");
					printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				}
				if(menu_add_node(FoodMenu, V))
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode added successfully.\n");
			}
			else
			{
				printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				while(TSReadValue(&V))
				{
					printf("\nError : Invalid input data");
					printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				}
				if(menu_add_node(CafeMenu, V))
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode added successfully.\n");
			}
			break;
		case 4:
			if(which_menu())
			{
				if(menu_show_content(FoodMenu) == 3)
					printf("\nError : Menu hasnt been created yet.\n");
			}
			else
			{
				if(menu_show_content(CafeMenu) == 3)
					printf("\nError : Menu hasnt been created yet.\n");
			}
			break;
		case 5:
			if(which_menu())
			{
				printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				while(TSReadValue(&V))
				{
					printf("\nError : Invalid input data");
					printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				}
				result = menu_change_content(FoodMenu, V);
				if(result == 1)
					printf("\nError : Current is at a head node.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode's content changed successfully.\n");
				break;
			}
			else
			{
				printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				while(TSReadValue(&V))
				{
					printf("\nError : Invalid input data");
					printf("\nEnter input : Name, Tag(0 = Plate, 1 = Category), Price(X.XX for plate, or 0 for Category)\n");
				}
				result = menu_change_content(CafeMenu, V);
				if(result == 1)
					printf("\nError : Current is at a head node.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode's content changed successfully.\n");
				break;
			}	
		case 6:
			if(which_menu())
			{
				result = menu_next(FoodMenu);
				if(result == 1)
					printf("\nError : Current node is last.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to next node successfully!\n");
			}
			else
			{
				result = menu_next(CafeMenu);
				if(result == 1)
					printf("\nError : Current node is last.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to next node successfully!\n");
			}
			break;
		case 7:
			if(which_menu())
			{
				result = menu_previous(FoodMenu);
				if(result == 1)
					printf("\nError : Current node has no previous.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to previous node successfully!\n");
			}
			else
			{
				result = menu_previous(CafeMenu);
				if(result == 1)
					printf("\nError : Current node has no previous.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to previous node successfully!\n");
			}
			break;
		case 8:
			if(which_menu())
			{
				result = menu_swap_next(FoodMenu);
				if(result == 1)
					printf("\nError : Current is at head node.\n");
				else if(result == 2)
					printf("\nError : There is no next node to swap with.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSwapped successfully.\n");
			}
			else
			{
				result = menu_swap_next(CafeMenu);
				if(result == 1)
					printf("\nError : Current is at head node.\n");
				else if(result == 2)
					printf("\nError : There is no next node to swap with.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSwapped successfully.\n");
			}
			break;
		case 9:
			if(which_menu())
			{
				result = menu_swap_previous(FoodMenu);
				if(result == 1)
					printf("\nError : Current is at head node.\n");
				else if(result == 2)
					printf("\nError : Previous is a head node.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSwapped successfully.\n");
			}
			else
			{
				result = menu_swap_previous(CafeMenu);
				if(result == 1)
					printf("\nError : Current is at head node.\n");
				else if(result == 2)
					printf("\nError : Previous is a head node.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSwapped successfully.\n");
			}
			break;
		case 10:
			if(which_menu())
			{
				result = menu_add_sub_category(FoodMenu);
				if(result == 1)
					printf("\nError : Can't add sub-category beneath head nodes.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSub-category added successfully!\n");
			}
			else
			{
				result = menu_add_sub_category(CafeMenu);
				if(result == 1)
					printf("\nError : Can't add sub-category beneath head nodes.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSub-category added successfully!\n");
			}
			break;
		case 11:
			if(which_menu())
			{
				result = menu_category_up(FoodMenu);
				if(result == 1)
					printf("\nError : There is not upper category.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to upper category successfully.\n");
			}
			else
			{
				result = menu_category_up(CafeMenu);
				if(result == 1)
					printf("\nError : There is not upper category.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to upper category successfully.\n");
			}
			break;
		case 12:
			if(which_menu())
			{
				result = menu_category_down(FoodMenu);
				if(result == 1)
					printf("\nError : There is not lower category.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to sub-category head successfully.\n");
			}
			else
			{
				result = menu_category_down(CafeMenu);
				if(result == 1)
					printf("\nError : There is not lower category.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMoved to  sub-category head successfully.\n");
			}
			break;
		case 13:
			if(which_menu())
			{
				result = menu_print_chain(FoodMenu);
				if(result == 1)
					printf("\nError : Current is not at a head node.\n");
				else if(result == 2)
					printf("\nError : Nothing to print.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				break;
			}	
			else
			{
				result = menu_print_chain(CafeMenu);
				if(result == 1)
					printf("\nError : Current is not at a head node.\n");
				else if(result == 2)
					printf("\nError : Nothing to print.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				break;
			}
		case 14:
			if(which_menu())
			{
				result = menu_delete_next(FoodMenu);
				if(result == 1)
					printf("\nError : There is no next node to delete.\n");
				else if(result == 2)
					printf("\nError : The node you are trying to delete has sub-categories.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nDeleted successfully!\n");
				break;
			}
			else
				result = menu_delete_next(CafeMenu);
				if(result == 1)
					printf("\nError : There is no next node to delete.\n");
				else if(result == 2)
					printf("\nError : The node you are trying to delete has sub-categories.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nDeleted successfully!\n");
				break;
		case 15:
			if(which_menu())
			{
				result = menu_delete_down(FoodMenu);
				if(result == 1)
					printf("\nError : Head nodes dont have sub-categories.\n");
				else if(result == 2)
					printf("\nCurrent node doesnt have any subcategories.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSub-categories deleted successfully!\n");
			}
			else
			{
				result = menu_delete_down(CafeMenu);
				if(result == 1)
					printf("\nError : Head nodes dont have sub-categories.\n");
				else if(result == 2)
					printf("\nCurrent node doesnt have any subcategories.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nSub-categories deleted successfully!\n");
			}
			break;
		case 16:
			if(which_menu())
			{
				result = menu_cut_node(FoodMenu);
				if(result == 1)
					printf("\nError : Head nodes cant be cut.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode was cut successfully.\n");
			}
			else
			{
				result = menu_cut_node(CafeMenu);
				if(result == 1)
					printf("\nError : Head nodes cant be cut.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode was cut successfully.\n");
			}
			break;
		case 17:
			if(which_menu())
			{
				result = menu_paste_node(FoodMenu);
				if(result == 1)
					printf("\nError : No node has been cut yet.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode pasted successfully.\n");
			}
			else
			{
				result = menu_paste_node(CafeMenu);
				if(result == 1)
					printf("\nError : No node has been cut yet.\n");
				else if(result == 3)
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nNode pasted successfully.\n");
			}
			break;
		case 18:
			if(which_menu())
			{
				if(menu_destruction(&FoodMenu))
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMenu destroyed successfully.\n");
			}
			else
			{
				if(menu_destruction(&CafeMenu))
					printf("\nError : Menu hasnt been created yet.\n");
				else
					printf("\nMenu destroyed successfully.\n");
			}
			break;							
		}
	}	while (option);
	return 0;
}
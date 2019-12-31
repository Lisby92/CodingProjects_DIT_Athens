/*************************************************************************
Implementation File	: Configurations.c
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: Handling/Configuring of Menu
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Configurations.h"
#define MAX_OPTIONS 18	//18 choises to handle menu items
#define MAX_MENUS 1     //we handle 2 menus(0,1)

void print_options() // prints the available menu options
{
	printf("\n");
	printf(" 0. Exit\n");
	printf(" 1. Create menu\n");
	printf(" 2. Check for empty category\n");
	printf(" 3. Add node in current category\n");
	printf(" 4. Print current plate/category content\n");
	printf(" 5. Change current plate/category content\n");
	printf(" 6. Go to next plate(same category)\n");
	printf(" 7. Go to previous plate(same category)\n");
	printf(" 8. Swap current node with next\n");
	printf(" 9. Swap current node with previous\n");
	printf("10. Add a new sub-category in menu\n");	
	printf("11. Go to upper category\n");
	printf("12. Go to lower sub-category\n");
	printf("13. Print content of category(and all lower sub-categories)\n");
	printf("14. Delete next node\n");
	printf("15. Delete all sub-categories\n");
	printf("16. Cut node\n");
	printf("17. Paste node\n");
	printf("18. Destroy Menu\n");
	printf("Enter your input (0-%d): ", MAX_OPTIONS);
	printf("\n");
}

int get_option()
{	
	//BEFORE : Nothing
	//Function to choose between the menu's available functions
	char buf[100];
	int option;

	option = -1;
	while (option < 0 || option > MAX_OPTIONS) {
		print_options();
		fgets(buf, sizeof(buf), stdin);
		sscanf(buf, "%d", &option);
	}
	return option;
}

int which_menu()
{
	//BEFORE : choise 1 : Create menu
	//TREATY : 0 = CafeMenu , 1 = Foodmenu , ...
	//Function to choose between the menus (currently we run 2)
	int option;

	option = -1;
	while (option < 0 || option > MAX_MENUS)
	{
		printf("\nChoose a menu-tag:\n");
		printf(" 0. CafeMenu\n");
		printf(" 1. FoodMenu\n");
		printf("Enter your input (0-%d): ", MAX_MENUS);
		scanf("%d", &option);
		getchar();
	}
	return option;
}
/*************************************************************************
Implementation File	: Menu.h
Author Date			: V. Mavrommatis 28/03/2018
Purpose				: Header file of Menu.c
Revision			: < Vasileios_Mavrommatis - 10/04/2018 >
**************************************************************************/
#ifndef __MENU__
#define __MENU__

#include "TyposStoixeiouMenu.h"

//================= ALIASING =================//

typedef struct MenuNode * MenuNodePtr;  // o typos deikti se komvo piato-cathgoria
typedef struct InfoMenu * InfoMenuPtr; // o typos deikti se pliroforiako komvo menu
typedef struct InfoMenu InfoMenu;
typedef struct MenuNode MenuNode;

//========== PROTOTYPE FUNCTIONS ============//
//============= TO HANDLE MENUS =============//

InfoMenuPtr menu_creation(char * name);
MenuNodePtr menu_node_creation();
int menu_empty_category(InfoMenuPtr info_ptr);
int menu_add_node(InfoMenuPtr info_ptr, TStoixeiouMenu V);
int menu_show_content(InfoMenuPtr info_ptr);
int menu_change_content(InfoMenuPtr info_ptr, TStoixeiouMenu V);
int menu_next(InfoMenuPtr info_ptr);
int menu_previous(InfoMenuPtr info_ptr);
int menu_swap_next(InfoMenuPtr info_ptr);
int menu_swap_previous(InfoMenuPtr info_ptr);
int menu_add_sub_category(InfoMenuPtr info_ptr);
int menu_category_up(InfoMenuPtr info_ptr);
int menu_category_down(InfoMenuPtr info_ptr);
int menu_print_chain(InfoMenuPtr info_ptr);
int menu_delete_next(InfoMenuPtr info_ptr);
int menu_delete_down(InfoMenuPtr info_ptr);
int menu_cut_node(InfoMenuPtr info_ptr);
int menu_paste_node(InfoMenuPtr info_ptr);
int menu_destruction(InfoMenuPtr * info_ptr);

#endif
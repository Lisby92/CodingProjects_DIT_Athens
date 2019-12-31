/*************************************************************************
Header File			: ch8_BSTpointer.h
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Simple Binary Search Tree with pointers functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#ifndef __CH8_BSTPOINTER__ 
#define __CH8_BSTPOINTER__

#include "typos_stoixeiouDDA.h"

/*diloseis tipon*/

typedef struct typos_komboy * typos_deikti;

/*diloseis synarthseon*/

/*dimiourgia/katastrofi dentrou*/
void Tree_dimiourgia(typos_deikti *tree);
void Tree_katastrofi(typos_deikti *tree);

/*Prakseis elegxou*/
int Tree_keno(typos_deikti tree);

/*Prakseis diaxeirisis */
void Tree_eisagogi(typos_deikti *tree, TStoixeiouDDA stoixeio, int *error);
void Tree_diagrafi(typos_deikti *tree, TStoixeiouDDA stoixeio, int *error);

/*Prakseis prosvasis */
typos_deikti Tree_apaidi(typos_deikti pateras);
typos_deikti Tree_dpaidi(typos_deikti pateras);
void Tree_pateras(typos_deikti tree, typos_deikti *pateras, typos_deikti paidi, int *error);

/*Prakseis anazitisis*/
void Tree_anazitisi(typos_deikti tree, TStoixeiouDDA stoixeio, typos_deikti *deiktis, int *found);
void Tree_periexomeno(typos_deikti tree, TStoixeiouDDA *periexomeno);

#endif /*#ifndef __CH8_BSTARRAY__ */


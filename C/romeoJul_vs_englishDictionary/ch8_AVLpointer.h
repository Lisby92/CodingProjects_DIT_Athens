/*************************************************************************
Header File			: ch8_AVLpointer.h
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: AVL Binary Search Tree with pointers functions
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
**************************************************************************/
#ifndef __CH8_AVLPOINTER__ 
#define __CH8_AVLPOINTER__

#include "typos_stoixeiouDDA.h"

/*diloseis tipon*/

typedef struct typos_komvou *typos_deikti; 

/*diloseis synarthseon*/

/*dimiourgia/katastrofi dentrou*/
void AVLTree_dimiourgia(typos_deikti *riza);
void AVLTree_katastrofi(typos_deikti *riza);

/*Prakseis elegxou*/
int	AVLTree_keno(typos_deikti riza);

/*Prakseis prosvasis*/

typos_deikti AVLTree_apaidi(typos_deikti p);
typos_deikti AVLTree_dpaidi(typos_deikti p);

void AVLTree_pateras(typos_deikti riza, typos_deikti *pateras, typos_deikti paidi, int *error);

/*Prakseis diaxeirisis */

void AVLTree_eisagogi(typos_deikti *riza,TStoixeiouDDA stoixeio,int *ypsilotero,int *error);
void AVLTree_diagrafi(typos_deikti *riza,TStoixeiouDDA stoixeio,int *ypsilotero,int *error);

/*Prakseis anazitisis*/
void AVLTree_anazitisi(typos_deikti riza, TStoixeiouDDA stoixeio, typos_deikti *deiktis, int *found);
void AVLTree_periexomeno(typos_deikti riza,TStoixeiouDDA *stoixeio);

#endif /*#ifndef __CH8_AVLPOINTER__ */


/***************************************************************************
Implementation File	: ch8_AVLpointer.c
Author Date			: V. Mavrommatis 04/05/2018
Purpose				: Implementation of AVL Binary Search Tree with pointers
Revision			: < Vasileios_Mavrommatis - 11/05/2018 >
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "ch8_AVLpointer.h"

#define FALSE 0
#define TRUE 1

#define printinfo FALSE /* an i printinfo ginei TRUE bgainoun miminata poy aforoun tis peristrofes tou dentrou*/

typedef enum{AY,IY,DY} paragon_isozygisis;

typedef struct typos_komvou{
	TStoixeiouDDA dedomena;
	typos_deikti	apaidi,dpaidi;
	paragon_isozygisis pi;
} typos_komvou;

/*Prakseis gia tin isozygisi tou AVL dentrou*/
void aristeri_peristrofi(typos_deikti *riza);
void dexia_peristrofi(typos_deikti *p);
void dexi_varos(typos_deikti *riza,int *ypsilotero, char parametros);
void aristero_varos(typos_deikti *riza,int *ypsilotero, char parametros);
void swap(typos_deikti *riza,typos_deikti *temp,int *ypsilotero);

void AVLTree_dimiourgia(typos_deikti *riza){
 /*	Pro: 		kamia
  *	Meta: 		Dimiourgia enos AVL dentrou */
	*riza = NULL;
	printf("AVL\n");
}

int	AVLTree_keno(typos_deikti riza){
/*	Pro: 		Dimiourgia enos AVL dentrou
  *	Meta:		epistrefei 1 an to dentro einai keno,diaforetika 0 */
	return ( riza == NULL );
}


typos_deikti AVLTree_apaidi(typos_deikti p){
/* Pro: 	o p deixnei ena kombo se ena AVL dentro	
  *	Meta:	epistrefei to aristero paidi pou deixnei o p */
     if(p!=NULL)
         return p->apaidi;
     else{
          return NULL;
     }
     
}


typos_deikti AVLTree_dpaidi(typos_deikti p){
/* Pro: 	o p deixnei ena kombo se ena AVL dentro	
  *	Meta:	epistrefei to deksi paidi pou deixnei o p */
     if(p!=NULL)
         return p->dpaidi;
     else{
          return NULL;
     }
}


void dexia_peristrofi(typos_deikti *p){
/*	Pro: 		o *p deixnei tin riza tou ipodentrou pou tha ginei deksia peristrofi 
  *	Meta:		exei ginei deksia peristrofi */
	typos_deikti prosoninos;
	if (AVLTree_keno(*p)) /*einai adinati i peristrofi enos kenou dentrou*/
		printf("Keno Dentro\n");
	else if (AVLTree_keno((*p)->apaidi)) /*einai adinato na ginei riza ena keno dentro*/
		printf("Keno aristero ypodentro\n");
	else
	{
		prosoninos = (*p)->apaidi;
		(*p)->apaidi = prosoninos->dpaidi;
		prosoninos->dpaidi = *p;
		*p = prosoninos;
	}
}


void aristero_varos(typos_deikti *riza,int *ypsilotero, char parametros) { 
/*	Meta:       Apokathistatai i isozigisi me tis LL kai LR peristrofes sto dentro "*riza"
  *             An exei auksithei to ipsos enos ipodentrou meta tin ektelesi ths sunarthshs tote
  *             to "*ypsilotero" einai 1 allios 0.H sunarthsh dexetai kai mia parametro(char parametros) me times
  *             'e' h 'd' pou dilonei an h synarthsh exei klhthei kata tin diarkeia eisagogis kombou h kata tin 
  *             diarkeia diagrafis kombou.Auto simvainei,dioti oi paragontes isozigisis ton komvon diaferoun se
  *             autes tis 2 energeies  */ 
	typos_deikti x; /*deiktis sto aristero ipodentro tis rizas*/
	typos_deikti w;

	x = (*riza)->apaidi;
	if (parametros=='e'){ /*periptosi opou i sinartisi exei klithei gia eisagogi kombou*/
		switch (x->pi)
		{
		case AY:/* apli LL peristrofi*/
			(*riza)->pi = IY;
			x->pi = IY;
            if(printinfo){  
		    	printf("dexia peristrofi: ");
		    	TSDDA_writeValue(stdout, (*riza)->dedomena);
		    	printf("\n");
             }
			dexia_peristrofi(riza);
			*ypsilotero = FALSE;  
			break;
		case IY:
			/*den simvainei i periptosi auti*/
			break;
		case DY:/*dipli LR  peristrofi */
			w = x->dpaidi;
			switch (w->pi)
			{
			case IY:
				(*riza)->pi = IY;
				x->pi = IY;
				break;
			case DY:
				(*riza)->pi = IY;
				x->pi = AY;
				break;
			case AY:
				(*riza)->pi = DY;
				x->pi = IY;
				break;
			}
			w->pi = IY;
            if(printinfo){   
              printf("aristeri peristrofi: ");
 	          TSDDA_writeValue(stdout, x->dedomena);
            }
			aristeri_peristrofi(&x);
			/*o x deixnei tora ton w*/
			(*riza)->apaidi = x;  
            if(printinfo) {
               printf(" --  dexia peristrofi:");
  	           TSDDA_writeValue(stdout, (*riza)->dedomena);
		    	printf("\n");
            }
			dexia_peristrofi(riza);
			*ypsilotero = FALSE;
		
		}
	}
	if (parametros=='d') /*periptosi opou i sinartisi exei klithei gia diagrafi kombou*/
	{
		switch (x->pi)
		{
		case AY:/*apli LL peristrofi*/
		case IY:
			if(x->pi==IY){
				(*riza)->pi = AY;
				x->pi = DY;  
					*ypsilotero = FALSE;  
			}
			else
			{
				(*riza)->pi = IY;
				x->pi = IY;
			}
			if(printinfo){
              printf("dexia peristrofi:");
  	          TSDDA_writeValue(stdout, (*riza)->dedomena);
  	          printf("\n");
            }
			dexia_peristrofi(riza);
		
			break;
		case DY:/*dipli LR peristrofi*/
			w = x->dpaidi;
			switch (w->pi)
			{
			case IY:
				(*riza)->pi = IY;
				x->pi = IY;
				break;
			case DY:
				(*riza)->pi = IY;
				x->pi = AY;
				break;
			case AY:
				(*riza)->pi = DY;
				x->pi = IY;
				break;
			}
			w->pi = IY; 
            if(printinfo){  
			   /* printf("aristeri peristrofi: "); */
               TSDDA_writeValue(stdout, x->dedomena);
            }
			aristeri_peristrofi(&x);
			/*o x deixnei tora ton w */
			(*riza)->apaidi = x; 
            if(printinfo){  
               /* printf(" --  dexia peristrofi:",(*riza)->dedomena); */
               TSDDA_writeValue(stdout, (*riza)->dedomena);
  	           printf("\n");
            }
			dexia_peristrofi(riza);
		}
	}
}

void aristeri_peristrofi(typos_deikti *p){
 /*	Pro: 		o *p deixnei tin riza tou ipodentrou pou tha ginei aristeri peristrofi
  *	Meta:		exei ginei aristeri peristrofi */
	typos_deikti prosoninos;
	if (AVLTree_keno(*p)) /*einai adinati i peristrofi enos kenou dentrou*/
		printf("Keno Dentro\n"); 
	else if (AVLTree_keno((*p)->dpaidi)) /*einai adinato na ginei riza ena keno dentro*/
		printf("Keno deksi ypodentro\n");
	else
	{
		prosoninos = (*p)->dpaidi;
		(*p)->dpaidi = prosoninos->apaidi;
		prosoninos->apaidi = *p;
		*p = prosoninos;
	}
}

void dexi_varos(typos_deikti *riza,int *ypsilotero, char parametros){
/*	Meta:		Apokathistatai i isozigisi me tis RR kai RL peristrofes sto dentro "*riza"
  *             An exei auksithei to ipsos enos ipodentrou meta tin ektelesi tis sinartisis tote
  *             to "*ypsilotero" einai 1 allios 0.H sunarthsh dexetai kai mia parametro(char parametros) me times
  *             'e' h 'd' pou dilonei an i sinartisi exei klithei kata tin diarkeia eisagogis kombou h kata tin
  *             diarkeia diagrafis kombou. Auto simvainei,dioti oi paragontes isozigisis ton kombon diaferoun se
  *             autes tis 2 energeies */
	typos_deikti x; /*deiktis sto deksi ipodentro tis rizas*/
	typos_deikti w;

	x = (*riza)->dpaidi;
	if(parametros=='e'){ /*periptosi opou h sunarthsh exei klithei gia eisagogi kombou*/
	switch (x->pi)
	{
		case DY:/*apli RR peristrofi*/
			(*riza)->pi = IY;
			x->pi = IY;
            if(printinfo){               
               printf("aristeri peristrofi:");
 	           TSDDA_writeValue(stdout, (*riza)->dedomena);
  	           printf("\n");
            }
			aristeri_peristrofi(riza);
			*ypsilotero = FALSE;  
			break;
		case IY:
			break;
		case AY:/*dipli RL peristrofi*/
			w = x->apaidi;
			switch (w->pi)
			{
			case DY:
				(*riza)->pi = AY;  
				x->pi = IY;
				break;
			case IY:
				(*riza)->pi = IY;
				x->pi = IY;
				break;
			case AY:
				(*riza)->pi = IY;  
				x->pi = DY;
				break;
			}
			w->pi = IY;	
			if(printinfo){
			   printf("dexia peristrofi:");
               TSDDA_writeValue(stdout, x->dedomena);
            }
			dexia_peristrofi(&x);
			/*o x deixnei tora ton w*/
			(*riza)->dpaidi = x;
			if(printinfo){
			   printf("aristeri peristrofi:");
               TSDDA_writeValue(stdout, (*riza)->dedomena);
  	           printf("\n");
            }
			aristeri_peristrofi(riza);
			*ypsilotero = FALSE;  
		
		}
	}
	if(parametros=='d') /*periptosi opou i sinartisi exei klithei gia diagrafi kombou*/
	{
	switch (x->pi)
	{
		case DY:/*apli RR peristrofi*/
		case IY:
			if(x->pi==IY){
				(*riza)->pi = DY;
				x->pi = AY; 
				*ypsilotero = FALSE;  
			}
			else{
			(*riza)->pi = IY;
			x->pi = IY;
			}
			if(printinfo){
               printf("aristeri peristrofi:");
               TSDDA_writeValue(stdout, (*riza)->dedomena);
  	           printf("\n");
            }
			aristeri_peristrofi(riza);
			
			break;

		case AY:/*dipli RL peristrofi*/
			w = x->apaidi;
			switch (w->pi)
			{
			case DY:
				(*riza)->pi = AY;  
				x->pi = IY;
				break;
			case IY:
				(*riza)->pi = IY;
				x->pi = IY;
				break;
			case AY:
				(*riza)->pi = IY;  
				x->pi = DY;
				break;
			}
			w->pi = IY;	
			if(printinfo){
			   printf("dexia peristrofi:");
               TSDDA_writeValue(stdout, x->dedomena);
            }
			dexia_peristrofi(&x);
			/*o x deixnei tora ton w*/
			(*riza)->dpaidi = x;
			if(printinfo){
			    printf("aristeri peristrofi:");
                TSDDA_writeValue(stdout, (*riza)->dedomena);
  	            printf("\n");
            }
			aristeri_peristrofi(riza);
		}
	}
}

void AVLTree_eisagogi(typos_deikti *riza,TStoixeiouDDA stoixeio, int *ypsilotero,int *error){
 /* Pro: 		Dimiourgia enos AVL dentrou 
  *	Meta:       Eisagogi tou kombou sto dentro efoson autos den iparxei idi.
  *             Episis ginontai oi katalliles energeies oste meta tin eisagogi tou kombou,
  *             to dentro na parameinei isozigismeno*/

	if (AVLTree_keno(*riza))
	{
		*riza = malloc(sizeof(struct typos_komvou));
		if(*riza==NULL){
           *error=1;
           return;
        }
		TSDDA_setValue(&((*riza)->dedomena),stoixeio);
		/* ((*riza)->dedomena)=stoixeio; */
		(*riza)->apaidi = NULL;
		(*riza)->dpaidi = NULL;
		(*riza)->pi = IY;
		*ypsilotero = TRUE;
	}
	else if (TSDDA_iso(stoixeio,(*riza)->dedomena)) {
		/* printf("O komvos yparxei idi sto AVL-dentro!\n"); */
		*error=1;
	} else if (TSDDA_mikrotero(stoixeio,(*riza)->dedomena)) 
	{			
		/*eisagogi sto aristero ipodentro*/
		AVLTree_eisagogi(&((*riza)->apaidi),stoixeio,ypsilotero,error);
		if (*ypsilotero)
		{
			switch((*riza)->pi)
			{
			case AY:
				aristero_varos(&(*riza),ypsilotero,'e');
				break;
			case IY:
				(*riza)->pi=AY;
				*ypsilotero = TRUE;   
				break;
			case DY: 
				(*riza)->pi = IY;
				*ypsilotero = FALSE;
				break;
			}
		}
	}
	else /*eisagogi sto deksi ipodentro*/
	{
		AVLTree_eisagogi(&((*riza)->dpaidi),stoixeio,ypsilotero,error);
		if (*ypsilotero)
		{
			switch((*riza)->pi)
			{
			case AY:
				(*riza)->pi = IY;
				*ypsilotero = FALSE;
				break;
			case IY:
				(*riza)->pi=DY;
				*ypsilotero = TRUE;  
				break;
			case DY:
				dexi_varos(&(*riza),ypsilotero,'e');
				break;
			}
		}
	}
}


void AVLTree_diagrafi(typos_deikti *riza,TStoixeiouDDA stoixeio,int *ypsilotero, int *error){
/*	Pro: 		Dimiourgia enos AVL dentrou
  *	Meta:		Diagrafi tou kombou me periexomeno "stoixeio"
                Episis ginontai oi katalliles energeies oste meta tin diagrafi tou
                kombou,to dentro na parameinei isozigismeno */ 
	typos_deikti prosorinos;
	
	*error=0;

	if(AVLTree_keno(*riza))
	{
		*error=1;
		return;
	}
	else if (TSDDA_mikrotero(stoixeio, (*riza)->dedomena) )/*anazitisi sto aristero ipodentro*/
	{
		AVLTree_diagrafi(&((*riza)->apaidi),stoixeio,ypsilotero, error);
		if (*ypsilotero)
		{
			switch((*riza)->pi)
			{
			case AY:
				(*riza)->pi = IY;
				break;
			case IY:
				(*riza)->pi=DY;
				*ypsilotero = FALSE;
				break;
			case DY:
				dexi_varos(&(*riza),ypsilotero,'d');
				break;
			}
		}
	}
	else if (TSDDA_megalytero(stoixeio,(*riza)->dedomena)) /*anazitisi sto deksi ipodentro*/
	{
		AVLTree_diagrafi(&((*riza)->dpaidi),stoixeio,ypsilotero,error);
		if (*ypsilotero)
		{
			switch((*riza)->pi)
			{
			case AY:
				aristero_varos(&(*riza),ypsilotero,'d');
				break;
			case IY:
				(*riza)->pi=AY;
				*ypsilotero = FALSE;
				break;
			case DY: 
				(*riza)->pi = IY;
				break;
			}
		}
	}
	else /*o kombos me periexomeno stoixeio vrethike*/
	{
		prosorinos=(*riza);

		if( prosorinos->dpaidi == NULL ) /*o kombos den exei deksi paidi*/
		{
			(*riza)=prosorinos->apaidi;
			*ypsilotero=TRUE;
			free(prosorinos);
		}
		else if( prosorinos->apaidi == NULL) /*o kombos den exei aristero paidi*/
		{
			(*riza)=prosorinos->dpaidi;
			*ypsilotero=TRUE;
			free(prosorinos);
		}
		else /*o kombos exei 2 paidia*/
		{
			/*to periexomeno tou kombou pros diagrafi antikathistatai apo to deksiotero stoixeio tou aristerou tou paidiou*/
			swap(&(prosorinos->apaidi),&prosorinos, ypsilotero);
			if (*ypsilotero)
			{
				switch((*riza)->pi)
				{
					case AY:
						(*riza)->pi = IY;
						break;
					case IY:
						(*riza)->pi=DY;
						*ypsilotero = FALSE;
						break;
					case DY:
						dexi_varos(&(*riza),ypsilotero,'d');
						break;
				}
			}

		} 
	}
}



void AVLTree_anazitisi(typos_deikti riza, TStoixeiouDDA stoixeio, typos_deikti *deiktis, int *found){
 /*	Pro: 		Dimiourgia enos AVL dentrou
  *	Meta:		epistrefei ena deikti ston kombo pou periexei "stoixeio" diaforetika NULL */
    *found=0;
    
	if (AVLTree_keno(riza)){ /*keno dentro*/
		*deiktis=NULL;
		return;
    }
	if (TSDDA_iso(stoixeio, riza->dedomena)) {
        *deiktis=riza;                     
		*found=1;
    }
	else if (TSDDA_mikrotero(stoixeio,riza->dedomena) ) /*anazitisi sto aristero ipodentro*/
		 AVLTree_anazitisi(riza->apaidi, stoixeio,deiktis,found);
	else	/*anazitisi sto deksi ipodentro*/
		 AVLTree_anazitisi(riza->dpaidi, stoixeio,deiktis,found);

}


/*anadromiki synartisi gia diagrafi olwn twn komvwn*/
void AVLTree_katastrofi(typos_deikti *riza){
 /*	Pro: 		Dimiourgia enos AVL dentrou 
  *	Meta:		Diagrafi olon ton kombon toy dentrou */
	int ypsilotero;
	int error;
	if(!AVLTree_keno(*riza))
	{    /*metadiatetagmeni diadromi sto dentro*/
         AVLTree_katastrofi(&(*riza)->apaidi);
		 AVLTree_katastrofi(&(*riza)->dpaidi);

		
		AVLTree_diagrafi(& (*riza), (*riza)->dedomena,&ypsilotero,&error);
		free(*riza);
		*riza=NULL;	
	}
}


void AVLTree_periexomeno(typos_deikti riza, TStoixeiouDDA *stoixeio){
/*	Pro: 		Dimiourgia enos AVL dentrou 
 *	Meta:	    Epistrefei sto "stoixeio" tin timi ton dedomenon pou iparxoun ston kombo pou deixnei i "riza"*/
     if(!AVLTree_keno(riza))
        /* *stoixeio=&(riza->dedomena); */
        TSDDA_setValue(stoixeio,riza->dedomena);
}


void AVLTree_pateras(typos_deikti riza, typos_deikti *pateras, typos_deikti paidi, int *error){
/* Pro: 		to paidi deixnei ena kombo se ena AVL dentro	
  *	Meta:		epistrefei ton patera ston "pateras" tou kombou pou deixnei to paidi */ 
    if(AVLTree_keno(riza)||AVLTree_keno(paidi)||TSDDA_iso(paidi->dedomena, riza->dedomena)){
		*pateras=NULL;
		*error=1;
		return;
	}
	if(paidi==riza->apaidi||paidi==riza->dpaidi)
		*pateras=riza;
	else if(TSDDA_mikrotero(paidi->dedomena, riza->dedomena))
		AVLTree_pateras(riza->apaidi, pateras, paidi, error);
	else if(TSDDA_megalytero(paidi->dedomena, riza->dedomena))
		AVLTree_pateras(riza->dpaidi, pateras, paidi, error); 	 
}



void swap(typos_deikti *riza,typos_deikti *temp,int *ypsilotero){
/*Voithitiki sunarthsh gia tin diagrafi kombou apo to dentro.
  Antalassei to periexomeno tou "temp" me to deksiotero paidi tis "riza"*/
	typos_deikti delnode;

	delnode=(*riza);

	if((*riza)->dpaidi != NULL)
	{
		swap(&((*riza)->dpaidi),temp,ypsilotero);
		if (*ypsilotero)
		{
			switch((*riza)->pi)
			{
			case AY:
				aristero_varos(&(*riza),ypsilotero,'d');
				break;
			case IY:
				(*riza)->pi=AY;
				*ypsilotero = FALSE;
				break;
			case DY: 
				(*riza)->pi = IY;
				break;
			}
		} 

	}
	else
	{
		delnode=(*riza);
		(*temp)->dedomena=(*riza)->dedomena;
		(*riza)=(*riza)->apaidi;
		if(printinfo){
           printf("swap with [ ");
           TSDDA_writeValue(stdout, delnode->dedomena);
  	           printf("]\n");
        }
		free(delnode);
		*ypsilotero=TRUE;
	}
}


/*************************************************
Arxeio ylopoihshs	: ch6_Simulation.c
Syggrafeas			: Y. Cotronis 07/03/2016
Skopos				: Ylopoihsh ouras prosomoiwshs
Revision 			:  <Your Name - Date>
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ch6_QArray.h"
#include "TStoixeiouOuras.h"
#include "Tameio.h"

int main(void)
{	TOuras      	oura;				 /*oura pelatwn-aytokinhtvn */
	float			pithanotita_afiksis; /*pi8anothta afikshs*/
	unsigned int	xronos_eksipiretisis;/*xronos e3yphrethshs enos pelath*/ 
	unsigned int	xronos_prosomoiosis; /*synolikos xronos prosomoiwshs*/
	int 			arithmos_tameiwn;    /*arithmos tameiwn prosomoiwshs*/
	unsigned int	xronos;				 /*roloi prosomoiwshs*/
	unsigned int	xronos_anamonis;	 /*synolikos xronos anamonhs*/
	unsigned int 	close_time;			 /*xronos leitourgerias susthmatos*/
	unsigned int 	extra_time = 0;		 /*epipleon xronos meta to telos leitourgeias gia eksipiretisi olwn twn pelatwn*/
	TSOuras		    Pelatis;		 	 /* o pelaths sthn oyra*/
	float			mesos_xronos;		 /*mesos xronos anamonhs tvn pelatvn*/
	float			randomAfixi = 0.0;
	int			    randomE3ipiretisi = 0;
	time_t			t;
	TTameio			Tameio;              /*monada eksypiretisis - tameio*/

	printf("Dwste arithmo tameiwn (>=1)\nUnits of time wrario leitourgeias (>=0)\nUnits of time prosomoiwshs (>=0)\nPi8anothta afikshs in unit of time (0~1)\nKai megisto xrono e3yphrethshs (>=0) in unit of times )\n");
	scanf("%d %u %u %f %u",&arithmos_tameiwn,&close_time,&xronos_prosomoiosis,&pithanotita_afiksis,&xronos_eksipiretisis);
	getchar();
	PelatisSetXronoEksipiretisis(&Pelatis, xronos_eksipiretisis);
	printf("H Trapeza 8a leitourghsei gia %4u units of time.\n", close_time);
	printf("H prosomoiwsh 8a diarkesei %4u units of time.\n",xronos_prosomoiosis);
	printf("H pi8anothta afikshs pelath se ena unit of time einai: %4.2f.\n",pithanotita_afiksis);
	printf("H megisth diarkeia e3yphrethshs enos pelath einai %d units of time .\n", PelatisGetXronoEksipiretisis(&Pelatis));

	OuraDimiourgia(&oura);
	struct TTameio *tameio_ptr;
	struct TTameio Tameia[arithmos_tameiwn];  //orismos enos pinaka me arithmos_tameiwn stoixeia domes tupou TTameio//
	tameio_ptr = Tameia;
	xronos = 0;
	xronos_anamonis = 0;
	for(i = 0; i < arithmos_tameiwn; i++)
	{
		TameioDimiourgia();
	}

	srand(time(&t));
	while(xronos < xronos_prosomoiosis)
	{
		randomAfixi = (float)rand()/(float)RAND_MAX;
		if(randomAfixi < pithanotita_afiksis)
		{
			PelatisSetXronoEisodou(&Pelatis, xronos);
			if (!OuraProsthesi(&oura, Pelatis))
			{
              printf("H oyra einai mikrh! H prosomoivsh stamataei \n");
              getchar();
              return 0;
        	}
    	} 
    	if((xronos >= close_time) && (randomAfixi < pithanotita_afiksis))   /*Pelaths hrthe ,alla h trapeza den dexete allous pelates*/
    	{
    		OuraApomakrynsh(&oura,&Pelatis);	/*afairese apthn oura tous ektos orariou pelates pou erxontai*/
    	}
    		/* Tamias-Elegxos */
    	
		if(TameioFree(Tameio)) /*eley8eros tamias*/
		{
			if(!OuraKeni(oura))			/*yparxei pelaths*/
			{	OuraApomakrynsh(&oura,&Pelatis);		/*phgainei sto tamio*/
    			randomE3ipiretisi = (float)rand()/(float)RAND_MAX*100;	/*float 1~100 assigned on int generator*/
    			randomE3ipiretisi %= xronos_eksipiretisis + 1;			/*0 mexri max diarkeia eksipiretisis*/
    			TameioNewCustomer(&Tameio, randomE3ipiretisi);			//neos pelaths sto tameio me ton xrono eksipiretisis tou//
    			if(xronos >= close_time)	/*Katametrhsh extra time meta to kleisimo */
    			{
 					extra_time += randomE3ipiretisi;
    			}
				xronos_anamonis += xronos - PelatisGetXronoEisodou(Pelatis);	/*ypologismos xronoy paramonhs sthn oura*/
			}
			else
			{
				TameioNoWork(&Tameio); // to tameio einai adranes //
			}
		}	
		else
		{
			TameioInService(&Tameio);		/*o tamias apasxolhmenos me pelath */
			TameioBusy(&Tameio);
		}
		xronos++; /*o xronos prosomoivshs ay3anetai kata 1 unit of time*/
	} /*while( xronos < xronos_prosomoiosis )*/
	/*end of simulation time*/
	printf("Den eksipiretithikan %d pelates meta to telos ths prosomoioshs\n", OuraGetSize(oura));
	if(!TameioGetArithmosPelatwn(Tameio))
		mesos_xronos = 0.0;
	else
		mesos_xronos = ((float)xronos_anamonis)/((float)TameioGetArithmosPelatwn(Tameio));
	
	printf("O pragmatikos xronos leitourgeias einai %u\n", (extra_time + close_time));
	printf("Xreiastikan epipleon %u extra units of time\n", extra_time);
	printf("Eksyphreth8hkan %d pelates\n",TameioGetArithmosPelatwn(Tameio));
	printf("O mesos xronos anamonhs htan %4.2f units of time.\n",mesos_xronos);

	for(i = 0; i < arithmos_tameiwn; i++)
	{
		printf("To tameio #%d eksupiretise %d pelates\n",i, );												//arithmos pelaton pou eksupiretithikan kai oi xronoi adraneias//
		printf("Htan apasxolhmeno gia %d units of time, kai htan adranes gia %d units of time\n", , );		//kai apasxolisis gia ta tameia pou leitourgisan //
	}
	//printf("O tamias htan adranhs gia %d units of time\n", TameioGetInactiveXronos(Tameio));
	//printf("O tamias htan apasxolhmenos gia %d units of time\n", TameioGetBusyXronos(Tameio));
    getchar();
	return 0;
}


/*************************************************************************
Implementation File	: Tameio.c
Author	Date		: Y. Cotronis 07/03/2016
Purpose 			: Ylopoihsh tameiou
Revision			:  <Your Name - date>
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "Tameio.h"

void TameioDimiourgia(TTameio *tameio)
{
	tameio->TimeBusy = 0;
	tameio->TimeInactive = 0;
	tameio->ArithmosPelaton = 0;
	tameio->EnapomenonXronos = 0;
}

void TameioNewCustomer(TTameio *tameio, int duration)
{
	tameio->ArithmosPelaton ++;
	tameio->EnapomenonXronos = duration;
}

void TameioInService(TTameio *tameio)
{
	tameio->EnapomenonXronos --;
}

void TameioNoWork(TTameio *tameio)
{
	tameio->TimeInactive ++;
}

void TameioBusy(TTameio *tameio)
{
	tameio->TimeBusy ++;
}

int TameioFree(TTameio tameio)
{
	if(tameio.EnapomenonXronos == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int TameioGetArithmosPelatwn(TTameio tameio)
{
	return tameio.ArithmosPelaton;
}

int TameioGetInactiveXronos(TTameio tameio) 
{
	return tameio.TimeInactive;
}

int TameioGetBusyXronos(TTameio tameio)
{
	return tameio.TimeBusy;
}
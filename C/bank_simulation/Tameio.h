/**********************************************+**************************
Header File Name 		: Tameio.h
Author	Date			: Y. Cotronis 07/03/2016
Purpose					: Header file of ATD - Tameio
Revision				:  <Your Name - Date>
**************************************************************************/

#ifndef __Tameio__ 
#define __Tameio__

typedef struct{
	unsigned int TimeBusy;
	unsigned int TimeInactive;
	unsigned int ArithmosPelaton;
	unsigned int EnapomenonXronos;
} TTameio;

void TameioDimiourgia(TTameio *tameio);
//αρχικοποιεί τα μέλη του struct ταμείο //
void TameioNewCustomer(TTameio *tameio, int duration);
// αυξάνει τον μετρητή πελατών και αρχικοποιεί εναπομείναντα χρόνο εξυπηρέτησης //
void TameioInService(TTameio *tameio);
// μειώνει χρόνο εξυπηρέτησης κατά 1 //
void TameioNoWork(TTameio *tameio);
// αυξάνει χρόνο αδράνειας κατά 1 //
void TameioBusy(TTameio *tameio);
// αυξάνει χρόνο απασχόλησης κατά 1 //
int TameioFree(TTameio tameio);
// ελέγχει αν είναι διαθέσιμο //
int TameioGetArithmosPelatwn(TTameio tameio);
// επιστρέφει τον αριθμό πελατών //
int TameioGetInactiveXronos(TTameio tameio); 
// επιστρέφει τον χρόνο αδράνειας του //
int TameioGetBusyXronos(TTameio tameio);
// epistrefei ton xrono pou einai apasxolimeno to tameio //
#endif
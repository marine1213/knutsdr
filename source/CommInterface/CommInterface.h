#ifndef _COMM_INTERFACE_H_
#define _COMM_INTERFACE_H_


#include "../Common.h"


#define	KEY_CALC_DATA	0
#define	KEY_COMM_DATA	1

#define CHAR_SEPERATOR	','
#define CHAR_END_STR	'\0'
#define CHAR_END_COMM	'\r'


//------------------- Function thread ------------------
void receiveDataFunctionThread();
void decodeDataFunctionThread();


//---------------- Decode communication data ------------
float* extractNumber(char *c);
void decodeReceivedData(char *input);

//---------------- RF communication setup ---------------
int	getFD();
bool	getRfAbility();
bool	checkRfAbility();
void	closeRf();

//------------ RF communication thread status -----------
void 	setOpeningRf(bool setting);
bool 	isOpeningRf();

//----------------------------------------------------------

#endif
#ifndef _CALC_INTERFACE_H_
#define _CALC_INTERFACE_H_


#include "../Common.h"
#include "../CmdInterface/CmdInterface.h"


//----------------------------------------------------------
void    keepGoingForwardThread();
void 	changDirectionThread();

//----------------- Calculation Helper ---------------------
float directionDetermining(Point *target);
//----------------------------------------------------------

#endif
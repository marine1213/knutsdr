#include "CommInterface.h"

//============= wireless parameters ===============
#define	device	"/dev/ttyUSB0"
#define	baud  	9600
bool 	rfAbility = false;
int 	fd;
//=================================================


/*
 * return FD
 */
int getFD(){
    return fd;
}

/*
 * return RF ability result
 */
bool getRfAbility(){
    return rfAbility;
}

/*
 * check RF ability
 */
bool checkRfAbility(){
	rfAbility = true;
    if((fd = serialOpen(device, baud))<0)
	rfAbility = false;
    return rfAbility;
}

/*
 * close RF module
 */
void closeRf(){
    serialClose(fd);
}
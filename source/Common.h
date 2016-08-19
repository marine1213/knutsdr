#ifndef _COMMON_H_
#define _COMMON_H_


#include <wiringSerial.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>


#include "selfDriving_Session.h"

using namespace std;


#define ONE_MILLION	1000000
#define ONE_THOUSAND	1000

#define MAX_EXEC_TIME	10 * ONE_THOUSAND
#define EXEC_TIME_LIM1	6 * ONE_THOUSAND
#define EXEC_TIME_LIM2	3 * ONE_THOUSAND
#define EXEC_TIME_LIM3	ONE_THOUSAND

#define TIME_FULL_LEFT	600
#define TIME_FULL_RIGHT	700
#define TIME_FORWARD	1000
#define TIME_BACKWARD	1500

#define KEY_POSITION	4
#define KEY_AUTO	5

enum WorkingMode{
	DEBUG_MODE,
	RELEASE_MODE,
}; 

enum InputType{
	KEYBOARD,
	WIRELESS	
};

enum WheelState {
	WH_UNKNOWN = -1,
	LEFT	   = 2,
	RIGHT 	   = 3,
};

enum MovingState {
	MV_UNKNOWN,
	STOPPED,
	MOVING,
};

enum HLvCmd {
	NO_HLvCmd ,
	HFORWARD  ,
	HBACKWARD ,
	HTURN_L	  ,
	HTURN_R	  ,
	HTURN_C	  ,
	HEXIT,
}; 


enum LLvCmd {
	NO_LLvCmd = -1,
	FORWARD   = 21,
	BACKWARD  = 22,
	TURN_L	  = 23,
	TURN_R	  = 24,
	EXIT,
};

//------------------ Global Variables -------------------
WorkingMode 	getMode();
InputType	getTypeOfInput();
bool		isAutoDriving();
void		setAutoDriving(bool auto);

//--------------------------- log ----------------------- 
void 	log(const char* log);

//---------------- Cmd - Comm Interface ----------------- 
bool 	getRfAbility();
bool	wirelessInputOpened();
void 	pushToCmd(char *c);

//--------------- Comm - Calc Interface ----------------- 
Session* 	getSession();
queue<Point>* 	getPosition();
Point		getMap();
MovingState	getMovingState();
void		setMovingState(MovingState state);

//-------------------------------------------------------

#endif
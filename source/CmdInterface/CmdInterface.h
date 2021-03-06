#ifndef _CMD_INTERFACE_H_
#define _CMD_INTERFACE_H_


#include "../Common.h"


//------------------ scenario for testing ---------------
bool	autoDrivingCase();	//Test auto driving
bool 	hcmdCase();		// Test high level cmd
bool 	basicCase();		// Test low level cmd

//------------------- App Life Circle --------------------
int 	init();
void 	onExit();

//------------------ High Level Command ------------------
void 	turnWithWheelState(WheelState st,int fwTime, int delay = 100);
void	searchForWheelState(WheelState newState);
void 	searchForCenterPoint();
HLvCmd 	decodeHLvCmd(char* c);

//------------------- Low Level Command ------------------
bool  	exec(char* c, int sleepTime = 0);
void 	decodeLLvCmd(char* c);
int	getExecTime(char *c);
void 	resetCmdState();
void 	resetTurnCmdState();

//-------------------------- Time ------------------------
long	checkTime(struct timespec last,long exceedingTime);
long 	deltaTime(struct timespec last);

//-------------------------- IOPi ------------------------
bool 	isInputLocked(char* c);
void 	openInput(char* c);
void 	getInput(char* c);

//----------------------- Wheel State ---------------------
void	centerPointItr();		// interrupt cb
void 	showWheelState(WheelState state);
void 	changeWheelState(LLvCmd command);
void 	setMaxWheelState(LLvCmd command);
void	setWheelState(WheelState newState);
void	setWheelState(int newState);
WheelState	getWheelState();
WheelState 	convertWheelState(int s);

//----------------- get global parameters ------------------
void		setMode(WorkingMode m);
void 		setTypeOfInput(InputType it);
long		getExceedingTime();
void		setExceedingTime(long eTime);		
LLvCmd		getLlCmd();
void		setLlCmd(LLvCmd cmd);
HLvCmd		getHlCmd();
void		setHlCmd(HLvCmd cmd);
char* 		getInputData();
struct timespec	getLastExec();
//----------------------------------------------------------
#endif
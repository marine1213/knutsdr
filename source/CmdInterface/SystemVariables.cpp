#include "CmdInterface.h"


//--------------------------------
WorkingMode 	mode;
//--------------------------------
WorkingMode getMode(){
    return mode;
}
void setMode(WorkingMode m){
    mode = m;
}
//--------------------------------
InputType	cmdInput;
//--------------------------------
InputType getTypeOfInput(){
    return cmdInput;
}
void setTypeOfInput(InputType it){
    cmdInput = it;
}
//--------------------------------
int 	exceedingTime	= MAX_EXEC_TIME;
//--------------------------------
long getExceedingTime(){
    return exceedingTime;
}
void setExceedingTime(long newExceedingTime){
    exceedingTime = newExceedingTime;
}
//--------------------------------
char 	c[16];
//--------------------------------
char* getInputData(){
    return c;
}
//--------------------------------
bool movingPhase;
//--------------------------------
bool isInitMovingPhase(){
	return movingPhase;
}
void setMovingPhase(bool mvPhase){
	movingPhase = mvPhase;
}
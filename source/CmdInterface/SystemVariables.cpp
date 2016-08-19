#include "CmdInterface.h"


WorkingMode 	mode;
InputType	cmdInput;

int 	exceedingTime	= MAX_EXEC_TIME;
char 	c[16];



WorkingMode getMode(){
    return mode;
}
void setMode(WorkingMode m){
    mode = m;
}
InputType getTypeOfInput(){
    return cmdInput;
}
void setTypeOfInput(InputType it){
    cmdInput = it;
}
long getExceedingTime(){
    return exceedingTime;
}
void setExceedingTime(long newExceedingTime){
    exceedingTime = newExceedingTime;
}
char* getInputData(){
    return c;
}
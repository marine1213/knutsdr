#include "CmdInterface.h"

LLvCmd 		llCmd;
struct timespec last;

/*
 * Official cmd to interact with toy car
 */
bool exec(char* c, int sleepTime){
	clock_gettime(CLOCK_REALTIME, &last);
	decodeLLvCmd(c);
	if(getLlCmd() == EXIT)
		return true;
	while (1){
		if(checkTime(last, getExceedingTime())){
			resetCmdState();
			break;
		}
	}
	delay(sleepTime);
	return false;
}

/*
 * Read string llCmd and current state
 * Do the llCmd as instructed
 */
void decodeLLvCmd(char* c){
	setLlCmd( NO_LLvCmd );
	char buffer[20];
	int time;
	switch(c[0]){
	    case 'L':	// turn left
		    setLlCmd( TURN_L );
		    time = getExecTime(c);
		    setExceedingTime( time );
		    if(time >= TIME_THRES_LEFT)
			setWheelState(LEFT2);
		    sprintf(buffer,"Turn left %d ms", getExceedingTime());
		    log(buffer);
	    break;
	    case 'R':	// turn right
		    setLlCmd( TURN_R );
		    time = getExecTime(c);
		    setExceedingTime( time );
		    if(time >= TIME_THRES_RIGHT)
			setWheelState(RIGHT2);
		    sprintf(buffer,"Turn right %d ms", getExceedingTime());
		    log(buffer);
	    break;
	    case 'F':	// go forward
		    setLlCmd( FORWARD );
		    setExceedingTime( getExecTime(c) );
		    sprintf(buffer,"Go forward %d ms", getExceedingTime());
		    log(buffer);
	    break;
	    case 'B':	// go backward
		    setLlCmd( BACKWARD );
		    setExceedingTime( getExecTime(c) );
		    sprintf(buffer,"Go backWard %d ms", getExceedingTime());
		    log(buffer);
	    break;
	    default:	// set no llCmd
		// clear current settings, wait for another input
		openInput(c);
	    break;
	}
	if(getLlCmd() != NO_LLvCmd)
		digitalWrite(getLlCmd(), 1);
}

/*
 * Get time for executing operation
 */
int getExecTime(char *c){
	if(strlen(c) >1){
	    int time = atoi(c+1);
	    if(time < MAX_EXEC_TIME){
		return time;
	    }
	}
    return getExceedingTime();
}

/*
 * resetCmdState of all output port
 */
void resetCmdState(){
	digitalWrite(TURN_L, 0);
	digitalWrite(TURN_R, 0);
	digitalWrite(FORWARD, 0);
	digitalWrite(BACKWARD, 0);
	setExceedingTime(EXEC_TIME_TURN);
}

/*
 * reset turn cmdState of turning output port
 */
void resetTurnCmdState(){
	digitalWrite(TURN_L, 0);
	digitalWrite(TURN_R, 0);
}

/*
 * return last moment when the low level cmd is executed
 */
struct timespec getLastExec(){
	return last;
}

/*
 * return last executed low level cmd
 */
LLvCmd getLlCmd(){
    return llCmd;
}

/*
 * set executed low level cmd
 */
void setLlCmd(LLvCmd cmd){
    llCmd = cmd;
}
#include "CmdInterface.h"

WheelState 	state;

/*
 * Change the state of the driving wheel
 * based on previous command
 */
void changeWheelState(LLvCmd command){
    if(command == TURN_R)
	state = RIGHT;
    if(command == TURN_L)
	state = LEFT;
}


/*
 * Show current state of driving wheel
 */
void showWheelState(WheelState state){
  if(getMode() == DEBUG_MODE){
	cout<<"\nCurrent state of driving wheel: ";
	switch(state){
	    case LEFT:
		cout<< "LEFT";
		break;
	    case RIGHT:
		cout<< "RIGHT";
		break;
	    default:
		cout<< "UNKNOWN_STATE";
		break;
	}
	cout<<endl;
  }
}

WheelState getWheelState(){
    return state;
}
void setWheelState(WheelState s){
    state = s;
}
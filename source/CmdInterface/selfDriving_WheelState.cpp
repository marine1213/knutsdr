#include "CmdInterface.h"

WheelState 	state;

/*
 * Change the state of the driving wheel
 * based on previous command
 */
void changeWheelState(LLvCmd command){
    int intState = state;
    if(command == TURN_R)
	intState++;
    if(command == TURN_L)
	intState--;
    setWheelState(intState);
    cout<<"change wheel state = "<< getWheelState()<<endl;
}

void setMaxWheelState(LLvCmd command){
    if(command == TURN_R)
	setWheelState(RIGHT2);
    if(command == TURN_L)
	setWheelState(LEFT2);
}

void setWheelState(WheelState s){
    state = s;
}
WheelState convertWheelState(int stt){
    switch(stt){
	case LEFT1: return LEFT1;
	case CENTER: return CENTER;
	case RIGHT1: return RIGHT1;
	default: return (stt>1)?RIGHT1:(stt<-1)?LEFT1:CENTER;
    }
}
void setWheelState(int stt){
    state = convertWheelState(stt);
}

WheelState getWheelState(){
    return state;
}
/*
 * Show current state of driving wheel
 */
void showWheelState(WheelState state){
  if(getMode() == DEBUG_MODE){
	cout<<"\nCurrent state of driving wheel: ";
	switch(state){
	    case LEFT2:
		cout<< "LEFT2";
		break;
	    case LEFT1:
		cout<< "LEFT1";
		break;
	    case CENTER:
		cout<< "CENTER";
		break;
	    case RIGHT1:
		cout<< "RIGHT1";
		break;
	    case RIGHT2:
		cout<< "RIGHT2";
		break;
	    default:
		cout<< "UNKNOWN_STATE";
		break;
	}
	cout<<endl;
  }
}

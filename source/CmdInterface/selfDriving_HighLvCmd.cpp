#include "CmdInterface.h"

HLvCmd hlCmd;
int interruptId 	= 1;
int triggerId 		= 0;
bool isTestingItr	= false;

int testingCount;

/*
 * called when sensor triggers interrupt
 */
void centerPointItr(){
	if(triggerId == interruptId){
	    interruptId++;
	    changeWheelState(getLlCmd());
	    resetTurnCmdState();		// problem A
	//============= calculate time ===================
	    long period = deltaTime(getLastExec());
	    cout<<"Exec Time = "<< period <<" ms"<<endl;
	//===============================================
	    openInput(getInputData());
	}
	if(isTestingItr){
	    log(" Testing-> itr is called");
	    testingCount++;
	    cout<<testingCount<<endl;
	}
}

/*
 * turn the wheel until next interrupt is triggered
 * return true if the wheel is stopped by interrupt
 * return false if the wheel is stopped by timeout
 */
bool turnWheelToNextItr(char *turnCmd){
	interruptId = triggerId;

	struct timespec lastTimeStamp;
	clock_gettime(CLOCK_REALTIME, &lastTimeStamp);

	while(true){
	    exec(turnCmd, 50);
	    if(interruptId > triggerId){
		triggerId 	+= interruptId;
		return true;
	    }
	    if(checkTime(lastTimeStamp, EXEC_TIME_TURN)){
		setMaxWheelState(getLlCmd());
		triggerId 	+= interruptId;
		return false;
	    }
	}

}

/*
 * turn the wheel to desired wheel state
 */
void searchForWheelState(WheelState newState){
	char LLvCmdList[4][6] 	= {"R110", "L105","R130", "L130"};
	int  offsetId;
	int  intNewState = newState;

	cout<<"current wheel state = "<< getWheelState()<<endl;
	if(getWheelState() < intNewState){
	    offsetId = 0;
	} else if(getWheelState() > intNewState){
	    offsetId = 1;
	} else {
	    log(" The wheel has already been at desired state"); 
	    return;
	}

// detect hard to get over position L1,R1 CENTER
	bool firstTimeTriggered = false;
	int currSt = getWheelState();
	if(abs(currSt) < 2){
	    // solution 2: do full turn before turning to middle position
	    char fullTurnCmd = {"L300","R700","R300"};
	    int  fullTurnId = currSt + 1;
	    exec(fullTurnCmd[fullTurnId],100);

	    // solution 1a: increasing pwm a bit but returned fail result
	    // for many tests
	    offsetId += 2;
	    firstTimeTriggered = true;
	}
	
	// stop infinite loop when the wheel stops at desired state
	// or time for turning is up
	while(getWheelState() != newState){
	    bool stoppedBySensor = turnWheelToNextItr(LLvCmdList[offsetId]);

	    if(firstTimeTriggered){
	    // solution 1b: increasing pwm a bit but returned fail result
	    // for many tests
		    log(" turn second time for center");
		offsetId -= 2;
		firstTimeTriggered = false;
		continue;
	    }

	    if(!stoppedBySensor){
		log("Itr for sensor did not work or...");
		log("	Wheel state went to last position");
		break;
	    }
	}
}

/*
 * search for center point
 * old function to compatible with previous cmd code version
 */
void searchForCenterPoint(){
	searchForWheelState(CENTER);
}

/*
 * convert wheel state code in cmd to wheel state
 * example: 	L1 -> WheelState LEFT1
 * 		R1 -> WheelState RIGHT2
 * return CENTER if there is any exception occurs
 */
WheelState decodeDesiredWheelState(char *c){
    int type = 0;
    if(strlen(c) >= 2){
	int temp = abs(atoi(c+1));
	if(temp)
	    switch(c[0]){
		case 'L':    type = 0 - temp;   break;
		case 'R':    type = temp;    	break;
	    }
    }
    return convertWheelState(type);
}

/*
 * Read string hlCmd 
 * Do the hlCmd as instructed
 */
HLvCmd decodeHLvCmd(char* c){
	hlCmd 		= NO_HLvCmd;
	int len 	= strlen(c);
	int delayTime 	= 80;
	bool turnItr	= false;

	switch(c[0]){
	    case 'O':	// test center point sensor
		isTestingItr = !isTestingItr;
		if(isTestingItr){
		    log("Test center point mode is true");
		    testingCount = 0;
		}
		else
		    log("Test center point mode is false");
	    break;
	    case 'A':	// enable auto driving
		if(!isAutoDriving()){
		    log(" unlock auto driving");
		    setAutoDriving(true);
		}
		    piUnlock(KEY_AUTO_GO);
		    piUnlock(KEY_AUTO_CALC);
	    break;
	    case 'U':	// go to center point
		searchForCenterPoint();
	    break;
	    case 'T':	// turn with interrupt
		if(1){
		    WheelState wState = decodeDesiredWheelState(c+1);
		    searchForWheelState(wState);
		}
	    break;
	    case 'L':	// turn left
		    hlCmd = HTURN_L;
		    if(len==1)
			setExceedingTime(TIME_FULL_LEFT);
		    exec(c,delayTime);
	    break;
	    case 'R':	// turn right
		    hlCmd = HTURN_R;
		    if(len==1)
			setExceedingTime(TIME_FULL_RIGHT);
		    exec(c,delayTime);
	    break;
	    case 'F':	// go forward
		    hlCmd = HFORWARD;
		    if(len==1)
			setExceedingTime(TIME_FORWARD);
		    exec(c,delayTime);
	    break;
	    case 'B':	// go backward
		    hlCmd = HBACKWARD;
		    if(len==1)
			setExceedingTime(TIME_BACKWARD);
		    exec(c,delayTime);
	    break;
	    case 'E': 	// exit
		hlCmd = HEXIT;
		log("Exit");
	    break;
	    case 'D':	// change debug mode
		if(getMode() == DEBUG_MODE){
		    log("Turn off debug mode");
		    setMode(RELEASE_MODE);
		}
		else{
		    log("Turn on debug mode");
		    setMode(DEBUG_MODE);
		}
	    break;
	    case 'W':	// change input mode
		if(!getRfAbility()){
		    cout<<"Unable to use RF module"<<endl;
		}else if(getTypeOfInput() == KEYBOARD){
		    log("Input from wireless module");
		    setTypeOfInput(WIRELESS);
		}
		else{
		    log("Input from keyboard");
		    setTypeOfInput(KEYBOARD);
		}
	    break;
	}
    return hlCmd;
}
/*
 * return dedcoded high level cmd
 */
HLvCmd getHlCmd(){
    return hlCmd;
}
/*
 * return set decoded high level cmd
 */
void setHlCmd(HLvCmd cmd){
    hlCmd = cmd;
}
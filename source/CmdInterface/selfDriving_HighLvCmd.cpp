#include "CmdInterface.h"

HLvCmd hlCmd;
bool isCenterPointFound 	= false;
bool isSearchingCenterPoint 	= false;

void centerPointItr(){
	if(isSearchingCenterPoint){
	    isCenterPointFound = true;
	    resetCmdState();
	//============= calculate time ===================
	    long period = deltaTime(getLastExec());
	    cout<<"Exec Time = "<< period <<" ms"<<endl;
	//===============================================
	    openInput(getInputData());
	}
	else 
	    changeWheelState(getLlCmd());
}

void searchForCenterPoint(){
	char LLvCmdList[4][6] 	= {"L1000","R100", "R1000","L110"};
	int  offsetId		= 0;
	int  delayTime 		= 100;

	isCenterPointFound 	= false;
	isSearchingCenterPoint 	= false;

	struct timespec lastTimeStamp;
	clock_gettime(CLOCK_REALTIME, &lastTimeStamp);

	// find suitable llCmd list
	if(getWheelState() == RIGHT)
		offsetId = 2;
	// init state of wheel
	    exec(LLvCmdList[offsetId], delayTime); 
	    isSearchingCenterPoint = true;
	    char* inputData = getInputData();
	    inputData[0] = 's';

	while(!isCenterPointFound){
	    exec(LLvCmdList[offsetId + 1], delayTime);
	    if(checkTime(lastTimeStamp, EXEC_TIME_LIM2)){
		resetCmdState();
		break;
	    }
	}
	isSearchingCenterPoint 	= false;
}

/*
 * Read string hlCmd 
 * Do the hlCmd as instructed
 */
HLvCmd decodeHLvCmd(char* c){
	hlCmd = NO_HLvCmd;
	int len = strlen(c);
	int delayTime = 100;

	switch(c[0]){
	    case 'A':
		if(isAutoDriving()){
		    log(" stop auto driving");
		    setAutoDriving(false);
		}
		else{
		    log(" unlock auto driving");
		    setAutoDriving(true);
		    piUnlock(KEY_AUTO);
		}
	    break;
	    case 'U':	// go to center point
		searchForCenterPoint();
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
	    case 'W':	// change debug mode
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

HLvCmd getHlCmd(){
    return hlCmd;
}
void setHlCmd(HLvCmd cmd){
    hlCmd = cmd;
}
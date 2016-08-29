#include "./CmdInterface/CmdInterface.h"
#include "./CommInterface/CommInterface.h"
#include "./CalcInterface/CalcInterface.h"

bool calculating = false;
bool isCalculating(){
	return calculating;
}
bool autoDriving;
bool isAutoDriving(){
	return autoDriving;
}
void setAutoDriving(bool autoParam){
	autoDriving = autoParam;
}

//------------------ Communication Thread ---------------
PI_THREAD (commThread)
{
    receiveDataFunctionThread();
}

PI_THREAD (decoThread)
{
    decodeDataFunctionThread();
}
//-------------------- Calculation thread ----------------
PI_THREAD (speedThread)
{
//    keepGoingForwardThread();
}


PI_THREAD (calcThread)
{
    changDirectionThread();
}

/*
 * initialization
 */
int init(){
	setTypeOfInput( KEYBOARD );
	setMode( DEBUG_MODE );
	setLlCmd( NO_LLvCmd );
	setHlCmd( NO_HLvCmd );
	setWheelState( CENTER );
	setAutoDriving(false);
	setMovingState(STOPPED);

	if(wiringPiSetup() == -1){
	    cout<< "unable to start wiringPi" <<endl;
	    return 0;
	}
	if(wiringPiISR(0, INT_EDGE_FALLING, &centerPointItr)<0){
	    cout<< "unable to setup interrupt" <<endl;		
	}

	// setup output to control the wheel
	pinMode(FORWARD, OUTPUT);
	pinMode(BACKWARD, OUTPUT);
	pinMode(TURN_L, OUTPUT);
	pinMode(TURN_R, OUTPUT);
	// finish setup IO
	cout<<"PIN setting done"<<endl;
	// resetCmdState all output port
	resetCmdState();

	setOpeningRf(checkRfAbility());
	if(!getRfAbility()){
	    log(" Unable to open serial device");
	} else{
	    int threadCommRs = piThreadCreate(commThread);
	    if(threadCommRs!=0)
	 	log(" Wireless communication thread didn't start");
	    int threadDecoRs = piThreadCreate(decoThread);
	    if(threadDecoRs!=0)
	 	log(" Decoding communication thread didn't start");
	}

	calculating = true;
	int threadSpeedRs = piThreadCreate(speedThread);
	if(threadSpeedRs!=0)
	    log(" Speed mesuring thread didn't start");

	int threadCalcRs  = piThreadCreate(calcThread);
	if(threadCalcRs!=0)
	    log(" Calculation thread didn't start");

	return true;
}

/*
 * close used resource
 */
void onExit(){
     setOpeningRf(false);
     closeRf();
     calculating = false;
}
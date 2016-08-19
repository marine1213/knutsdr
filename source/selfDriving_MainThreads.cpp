#include "./CmdInterface/CmdInterface.h"
#include "./CommInterface/CommInterface.h"

bool calculating = false;
Point stoppedPoint;

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
	Point prevPoint;
	queue<Point> *position = getPosition();
	float speed = 0;

    	struct timespec last;
	clock_gettime(CLOCK_REALTIME, &last);

	while(calculating){
	    if(position->size() > 1){
		/*Point prevPt = position->front();
		Point nextPt = position->back();
		float movLen = prevPt.distance(&nextPt);
		speed += movLen;*/
	    }else{
		setMovingState(STOPPED);
		log("speed thread locked");
		piLock(KEY_POSITION);
	    }
	    /*if(long dtime = checkTime(last,1000) && speed){
		cout<<"speed = "<<(speed/(float)dtime)<<" pixels/s"<<endl;
		speed = 0;
		clock_gettime(CLOCK_REALTIME, &last);
	    }*/
	}
}

float directionDetermining(Point *target){
	char cmdList[2][6] = {"F1000", "B1000"};
	log("direct - center point");
	searchForCenterPoint();		// sync direction
	log("direct - forward");
	exec(cmdList[0],100);		// make it move a bit
	log("direct - wait for moving");
	while(getMovingState() == STOPPED);
	log("direct - wait for stopping");
	while(getMovingState() == MOVING);
	Point firstPt = getPosition()->back();	// save current point
	cout<<"first = "<<firstPt.x<<" - "<<firstPt.y<<endl;
	exec(cmdList[1],100);		// make it move second time
	log("direct - wait for moving");
	while(getMovingState() == STOPPED);
	log("direct - wait for stopping");
	while(getMovingState() == MOVING);
	Point lastPt = getPosition()->back();	// save next point
	cout<<"last = "<<lastPt.x<<" - "<<lastPt.y<<endl;
	
	float direction = lastPt.angle(&firstPt,target);
	return direction;
}

PI_THREAD (calcThread)
{
	char cmdList[4][6] = {"L700","R700","F1000","F3000"};
	float limRight  = 3;
	float limLeft 	= -3;
    while(calculating){
	if(isAutoDriving()){
	    if(getSession() && getSession()->target.size()){
		log("auto drive");
		// analyze current information
		Point target = getSession()->target.back();
		float angle = directionDetermining(&target);

		cout<<"angle = "<<angle<<endl;
		// Go forward
		if(angle <= limRight && angle >= limLeft){
		    exec(cmdList[3],100);
		    float d = getPosition()->back().distance(&target);
		    cout<<" Distance to next target: "<<d<<endl;
		    if(d<5)
			cout<<" Arrived"<<endl;
		}else 

		// turn based on target
		if(angle > limRight){ 
		    exec(cmdList[1],100);	// Turn Right
		    exec(cmdList[2],100);	// Forward
		}else if(angle < limLeft){
		    exec(cmdList[0],100);	// Turn Left
		    exec(cmdList[2],100);	// Forward
		}
	    }else
		setAutoDriving(false);
	}else{
	    log("lock auto thread");
	    piLock(KEY_AUTO);
	}
    }
}

bool autoDriving;
bool isAutoDriving(){
	return autoDriving;
}
void setAutoDriving(bool autoParam){
	autoDriving = autoParam;
}

/*
 * initialization
 */
int init(){
	setTypeOfInput( KEYBOARD );
	setMode( DEBUG_MODE );
	setLlCmd( NO_LLvCmd );
	setHlCmd( NO_HLvCmd );
	setWheelState( WH_UNKNOWN );
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
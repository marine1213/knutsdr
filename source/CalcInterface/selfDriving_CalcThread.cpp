#include "CalcInterface.h"


void keepGoingForwardThread(){
	Point prevPoint;
	QArray *position = getPosition();

    	struct timespec last;
	clock_gettime(CLOCK_REALTIME, &last);

	char cmdList[1][6] = {"F3000"};
	while(isCalculating()){
	    if(!isInitMovingPhase()){
		piLock(KEY_MOV2CALC);
	    }
	    if(isAutoDriving())
		exec(cmdList[0],1000);
	    else{
		log("lock auto go thread: permission");
		piLock(KEY_AUTO_GO);
		continue;
	    }

	    if(position->size() <= 1){
		setMovingState(STOPPED);
		log("lock auto go thread: position data");
		piLock(KEY_POSITION);
	    }
	}
}

void changDirectionThread(){
	float limRight  = 10;
	float limLeft 	= -10;
    while(isCalculating()){
	if(isAutoDriving()){
	    if(getSession() && getSession()->target.size()){
		log("auto drive");
		// analyze current information
		Point target = getSession()->target.back();
		float angle = directionDetermining(&target);

		cout<<"angle = "<<angle<<endl;

		// Go forward
		float d = getPosition()->back(0).distance(&target);
		cout<<" Distance to next target: "<<d<<endl;
		if(angle <= limRight && angle >= limLeft){
		    if(d<300)
			resetCmdState();
		    else
			turnWithWheelState(CENTER,1500);
		}else 

		// turn based on target
		if(angle > limRight){ 
		    turnWithWheelState(RIGHT1,1500);
		}else if(angle < limLeft){
    		    turnWithWheelState(LEFT1,1500);
		}
	    }else
		setAutoDriving(false);
	}else{
	    log("lock auto calc thread");
	    piLock(KEY_AUTO_CALC);
	}
    }
}

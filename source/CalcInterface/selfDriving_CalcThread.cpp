#include "CalcInterface.h"

// stopped using this function
void keepGoingForwardThread(){
	Point prevPoint;
	QArray *position = getPosition();

    	struct timespec last;
	clock_gettime(CLOCK_REALTIME, &last);

	char cmdList[1][6] = {"F2000"};
	while(isCalculating()){
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
	char cmdList[4][6] = {"L700","R700"};
	float limRight  = 3;
	float limLeft 	= -3;
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
		    if(d<170)
			resetCmdState();
		    else
			searchForCenterPoint();
		}else 

		// turn based on target
		if(angle > limRight){ 
		    exec(cmdList[1],100);	// Turn Right
		    searchForCenterPoint();
		}else if(angle < limLeft){
		    exec(cmdList[0],100);	// Turn Left
		    searchForCenterPoint();
		}
	    }else
		setAutoDriving(false);
	}else{
	    log("lock auto calc thread");
	    piLock(KEY_AUTO_CALC);
	}
    }
}

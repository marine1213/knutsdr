#include "CalcInterface.h"

float directionDetermining(Point *target){
	
	Point firstPt = getPosition()->back(1);	// save current point
	cout<<"first = "<<firstPt.x<<" - "<<firstPt.y<<endl;
	Point lastPt = getPosition()->back(0);	// save next point
	cout<<"last = "<<lastPt.x<<" - "<<lastPt.y<<endl;
	
	float direction = lastPt.angle(&firstPt,target);
	return direction;
}

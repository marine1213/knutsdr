#include "CommInterface.h"

MovingState mvState = STOPPED;
Point		map;
QArray 		position(2);
Session *currentSession = NULL; // information about current session

Point	*prevPosition;
/*
 * extract number from the comm string with folowing rule:
 * {.. SEPERATOR + NUMBER1 + SEPERATOR + NUMBER2 +.. +NULL}
 */
float* extractNumber(char *c){
	// counting character and seperator
	int countSeperator = 0, i;
	for(i = 0; c[i] != CHAR_END_COMM && c[i] != CHAR_END_STR;i++)
	    countSeperator += c[i] == CHAR_SEPERATOR ? 1 : 0;

	// alloccate memory for output
	float *output = (float*) calloc(countSeperator + 1, sizeof(float));
	memset(output,0,sizeof(output));
	output[0] = countSeperator;

	// extract numbers
	int frontId = -1, backId = -1;
	int outputId = 1;
	for(int j = 0; j < i+1; j++){
	    if(c[j] == CHAR_SEPERATOR || c[j] == CHAR_END_STR){
		c[backId = j] = CHAR_END_STR;
		if(frontId != backId && frontId != -1){
		    output[outputId++] = atof(c+frontId);
		}
		frontId = backId + 1;
	    }
	}
	return output;
}

void addQueue(float* num, queue<Point> &q){
    if(num[0] < 2)
	log("Not enough values for a point");
    else{
    	q.push(Point(num[1],num[2]));
    }	
}

/*
 * decode data from input string
 * S: start new session, discard current session
 * T: target point, follows with session
 * P: position of toy car
 * I: Extra information (Map, Warning,...)
 * M: Message is showed for user, indexed in session if available
 */
void decodeReceivedData(char *input){
	char* c = (char*)malloc(sizeof(input));
	memset(c,0,sizeof(c));
	strcpy(c,input);

	switch(c[0]){
	    case 'S':
		if(currentSession)
			delete currentSession;		
		log("Init new session");
		currentSession = new Session(c+2);
	    break;
	    case 'T':
		if(currentSession){
		    float *num = extractNumber(c);
		    cout<<"Received Target: ["<< num[1]<<" , "<<num[2]<<"]"<<endl;
		    addQueue(num,currentSession->target);
		    free(num);
		    free(c);
		} else{
		    log("Saving Target Error: There is no working session");
		}
		if(!isAutoDriving()){
		    log(" unlock auto driving");
		    setAutoDriving(true);
		    setMovingPhase(true); // let car run straight some time
		    piUnlock(KEY_MOV2CALC);
		}
		    setMovingPhase(true);
		    piUnlock(KEY_AUTO_GO);
		    piUnlock(KEY_AUTO_CALC);
	    break;
	    case 'P':
		if(1){
		    setMovingPhase(false); // stop car from running wildly
		    float *num = extractNumber(c);
		    cout<<"Received Position: ["<< num[1]<<" , "<<num[2]<<"]"<<endl;
 		    if(num[0] < 2)
			log("Not enough values for a point");
    		    else{
		    	position.add(Point(num[1],num[2]));
		    }

		    Point newPosition = position.back(0);
		    if(!prevPosition){
			mvState = MOVING;
		    }else {
			if(newPosition.distance(prevPosition) == 0)
			    mvState = STOPPED;
		    	else
			    mvState = MOVING;
		    }
		    piUnlock(KEY_POSITION);

		    prevPosition = &newPosition;
		    free(num);
		    free(c);
		}
	    break;
	    case 'I':
		if(currentSession){
		    currentSession->inf.push(c+1);
		} else{
		    log("Saving Information Error: There is no working session");
		}
	    break;
	    case 'M':
		cout<<"MSG:"<<c+1<<endl;
		if(currentSession){
		    currentSession->msg.push(c+1);
		} else{
		    log("Saving Message Error: There is no working session");
		}
	    break;
	    default:
		if(wirelessInputOpened()){
		    log("push data to cmd");
		    pushToCmd(c);
		}
		else
		    free(c);
	    break;
	}
}

/*
 * return list of position
 */
QArray* getPosition(){
    return &position;
}

/*
 * return current session information
 */
Session* getSession(){
    return currentSession;
}

/*
 * return current map
 */
Point getMap(){
    return map;
}

/*
 * return moving state
 */
MovingState getMovingState(){
    return mvState;
}

/*
 * return moving state
 */
void setMovingState(MovingState state){
    mvState = state;
}

#include "CommInterface.h"


bool openingRfComm;		// flag about state of comm thread
queue<char*> inputDataPool;	// temporary storage of comm data

/*
 * detail of receiving data in commThread
 */
void receiveDataFunctionThread(){
	piLock(KEY_COMM_DATA);	// no data is received, lock decode thread
	log("Waiting for signal");
	char *c = (char*)malloc(16*sizeof(char));
	memset(c,0,sizeof(c));
	int i = 0;
 	while(openingRfComm)
 	{
	  if(serialDataAvail(getFD())){
	    char newChar = serialGetchar(getFD());
  	    if(newChar == CHAR_END_COMM || i > 15){
		c[i] = CHAR_END_STR;
		inputDataPool.push(c);
		piUnlock(KEY_COMM_DATA); // unlock current decode thread
		c = (char*)malloc(16*sizeof(char));
		memset(c,0,sizeof(c));
		i = 0;
	    }else
	    	c[i++] = newChar;
	  }
 	}

}

/*
 * detail of decoding data in decoThread
 */
void decodeDataFunctionThread(){
	while(openingRfComm){
	    if(inputDataPool.size() == 0){
		log("decode rf data locked");
		piLock(KEY_COMM_DATA);	// this thread is locked here
	    }
	    else{
		char* c = inputDataPool.front();
		inputDataPool.pop();
		decodeReceivedData(c);
		free(c);
	    }
	}
}

/*
 * return opening RF state
 */
bool isOpeningRf(){
    return openingRfComm;
}

/*
 * set opening state
 */
void setOpeningRf(bool setting){
    openingRfComm = setting;
}
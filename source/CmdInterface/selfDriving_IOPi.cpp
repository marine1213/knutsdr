#include "CmdInterface.h"

#define KEY_REVOKE_REMOTE_CMD 	2

/*
 * Open keyboard input for command
 */
void openInput(char* c){
	if(c[0]!='c') {
		c[0] = 'c';
		log("Input unlocked");
	}
}

/*
 * Open keyboard input for new command
 */
bool isInputLocked(char* c){
	return c[0] != 'c';
}

bool wirelessInputOpened(){
    return (getTypeOfInput() == WIRELESS);	
}

char * temp = NULL;
void pushToCmd(char *c){
	log(c);
	temp = c;
	piUnlock(KEY_REVOKE_REMOTE_CMD);
}

bool threadCalled;
PI_THREAD(revokeWirelessCmdThread){
    cout<<"Input any key to take control by keyboard"<<endl;
    while(1){
	char t[20];
	memset(t,0,20);
	cin>>t;
	if(t[0] != '\0'){
		setTypeOfInput(KEYBOARD);
		temp = (char*)malloc(sizeof(char));
		temp[0] = (char)0;
		piUnlock(KEY_REVOKE_REMOTE_CMD);
		break;
	}
    }
    threadCalled = false;
}

/*
 * Wait for command from keyboard input
 */
void getInput(char* c){
    InputType cmdInput = getTypeOfInput();
    if(cmdInput == KEYBOARD){
	    if(threadCalled){
		cout<<"Input any key to take keyboard control"<<endl;
		while(threadCalled)
			piLock(KEY_REVOKE_REMOTE_CMD);	// locked 2 times here
		piUnlock(KEY_REVOKE_REMOTE_CMD);		// unlock the second time
	    }
	cout<<"Waiting input Command..."<<endl;
	cin>>c;
    } else if(cmdInput == WIRELESS){
//==============================================================
	temp 	 	= NULL;
	while(!temp){
	    if(!threadCalled){
		threadCalled = true;
		//log("start new thread");
		int threadRs = piThreadCreate(revokeWirelessCmdThread);
		if(threadRs)
	    	    log(" Wireless Input Command thread didn't start");
	    }
	    delay(100);	// wait for sub thread lock first
	    piLock(KEY_REVOKE_REMOTE_CMD);
	    if(temp){
		strcpy(c,temp);
		log("Redirecting wireless data to input cmd...");
	    }
	}    
	free(temp);
//==============================================================
    }
    log("Input locked");
}

/*
 * Show log if current working mode
 * is debug mode
 */
void log(const char *log){
    if(getMode() == DEBUG_MODE)
	cout<<"Log: "<<log<<endl;
}
#include "CmdInterface.h"



bool hcmdCase(){
	getInput(getInputData());
	if(decodeHLvCmd(getInputData()) == HEXIT)
		return false;
	openInput(getInputData());
	return true;
}

bool basicCase(){
	getInput(getInputData());
	if(exec(getInputData()))
		return false;
	openInput(getInputData());
	return true;
}


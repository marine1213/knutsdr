#include "CmdInterface/CmdInterface.h"


int main(void)
{

	if(!init())
	    return 0;
	openInput(getInputData());
		
//	searchForCenterPoint();
	while(1)
	{	
//		if(!basicCase())    break;
		if(!hcmdCase())    break;
	}
	onExit();
	return 0;
}





#include "CmdInterface.h"


/*
 * Return different of time from now to last marked time
 */ 
long deltaTime(struct timespec last){
    struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	long nsec = now.tv_nsec - last.tv_nsec;
	time_t sec = now.tv_sec - last.tv_sec;
	long period = sec * ONE_THOUSAND + nsec / ONE_MILLION;
    return period;
}

/*
 * compare running time with exceeding time
 * If different <= 0 then return 0
 * If different > 0 then return different time
 */
long checkTime(struct timespec last,long exceedingTime){
    long runTime  = deltaTime(last);
    long overTime = runTime - exceedingTime;
    return overTime <= 0? 0 : overTime;
}

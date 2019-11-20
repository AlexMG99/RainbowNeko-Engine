#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "Globals.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Resume_Timer();



	uint32 Read() const;
	float ReadSec() const;

private:

	bool       running;
	uint32	started_at;
	uint32  stopped_at;
};

#endif //__j1TIMER_H__

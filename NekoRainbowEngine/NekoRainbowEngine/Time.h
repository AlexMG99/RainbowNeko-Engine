#ifndef TIME_H_
#define TIME_H_

#include "Timer.h"


class Time
{

public:

	static void Start();
	static void PreUpdate(float dt);
	static void Update();


	static void Play();
	static void Pause();
	static void Resume();
	static void Stop();


public:

	static float delta_t;
	static float scale_t;
	static float time;
	static Timer game_timer;
	static bool paused;
	static bool running;

};

#endif 

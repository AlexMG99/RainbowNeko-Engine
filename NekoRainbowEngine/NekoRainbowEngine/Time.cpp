#include "Time.h"

float Time::delta_t;
float Time::scale_t = 1;
float Time::time;

Timer Time::game_timer;


bool Time::oneframe = false;
bool Time::paused = false;
bool Time::running = false;


void Time::Start()
{
	running = true;
	game_timer.Start();
	time = 0;
}

void Time::PreUpdate(float dt)
{
	delta_t = running ? dt : 0;

	if (running)
	{
		time = game_timer.Read() / 1000.0f;
	}
}

void Time::Update()
{
}

void Time::Play()
{
	running = true;
}

void Time::Pause()
{
	paused = true;
	game_timer.Stop();
	
}

void Time::Resume()
{
	paused = false;
	game_timer.Resume_Timer();
	
}

void Time::Stop()
{
	running = false;
	game_timer.Stop();
}

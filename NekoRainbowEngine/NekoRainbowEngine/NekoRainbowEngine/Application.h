#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTest.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleTest* scene_test;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;

	int					capped_ms = -1;

private:

	std::list<Module*> list_modules;

	PerfTimer			ptimer;
	uint64				frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	uint				frame_rate = 0;
	float				dt = 0.0f;
	float				avg_fps = 1.00f;
	bool				frame_capped = true;

	std::string			log;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetAvgFPS();
	float GetAvgMs();
	void CapFPS(float frame_r);

	void Log(const char* log_text);
	const char* GetLog();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;
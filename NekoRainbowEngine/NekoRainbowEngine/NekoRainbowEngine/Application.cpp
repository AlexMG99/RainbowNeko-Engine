#include "Application.h"
#include "imgui/imgui.h"

Application::Application()
{
	PERF_START(ptimer);

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_test = new ModuleTest(this);
	viewport = new ModuleViewport(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	importer = new ModuleImporter(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(importer);
	
	// Scenes
	AddModule(viewport);
	AddModule(scene_test);

	// Renderer last!
	AddModule(renderer3D);

	PERF_PEEK(ptimer);
}

Application::~Application()
{

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		delete* item;
		*item = nullptr;
	}

	list_modules.clear();
}

bool Application::Init()
{
	PERF_START(ptimer);

	bool ret = true;

	// Call Init() in all modules
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Start();
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	frame_new_count++;
	last_sec_frame_count++;

	dt = (float)ms_timer.Read();
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;

	}

	uint32 last_frame_ms = ms_timer.Read();

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		//LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}

	avg_fps = float(frame_count) / startup_time.ReadSec();
	curr_fps = float(frame_new_count) / new_startup_time.ReadSec();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PreUpdate(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->Update(dt);
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && ret; item++)
	{
		ret = (*item)->CleanUp();
	}

	PERF_PEEK(ptimer);

	return ret;
}

float Application::GetAvgFPS()
{
	return avg_fps;
}

float Application::GetAvgMs()
{
	return dt;
}

uint Application::GetCurrFPS()
{
	return curr_fps;
}

void Application::CapFPS(float frame_r)
{
	frame_new_count = 0;
	new_startup_time.Start();
	capped_ms = 1000 / frame_r;
}

void Application::Log(const char* log_text)
{
	log.append(log_text);

	scene_test->Log(log_text);
}

const char * Application::GetLog()
{
	return log.c_str();
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
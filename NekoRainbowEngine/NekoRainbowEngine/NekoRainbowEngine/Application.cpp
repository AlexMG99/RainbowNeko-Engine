#include "Application.h"
#include "imgui/imgui.h"
#include "Parson/parson.h"

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

	InitializeJSONDoc();
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Load();
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("-------------- Application Start --------------");
	
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Start();
	}

	PERF_PEEK(ptimer);
	return ret;
}

update_status Application::Load()
{
	update_status ret = UPDATE_CONTINUE;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Load();
	}
	return ret;
}

update_status Application::Save()
{
	update_status ret = UPDATE_CONTINUE;
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; item++) {
		ret = (*item)->Save();
	}
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

	json_value_free(settings_doc);
	json_value_free(credits_doc);

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

void Application::Log(ConsoleText text)
{
	logs.push_back(text);
}

std::list<ConsoleText> Application::GetLogs() const
{
	return logs;
}

void Application::ClearLogs()
{
	logs.clear();
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::InitializeJSONDoc()
{
	settings_doc = json_parse_file("Settings/win_config.json");
	credits_doc = json_parse_file("Settings/win_about.json");

	if (!settings_doc) {
		settings_doc = json_value_init_object();
		json_serialize_to_file(settings_doc, "Settings/win_config.json");
	}

	if (!credits_doc) {
		credits_doc = json_value_init_object();
		json_serialize_to_file(credits_doc, "Settings/win_about.json");
	}
}

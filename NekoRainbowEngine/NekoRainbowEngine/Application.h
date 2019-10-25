#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleViewport.h"
#include "ModuleImporter.h"

typedef struct json_value_t  JSON_Value;
struct ConsoleText;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleEditor* editor;
	ModuleViewport* viewport;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleImporter* importer;

	int					capped_ms = -1;
	JSON_Value* settings_doc = nullptr;
	JSON_Value* credits_doc = nullptr;

private:

	std::list<Module*> list_modules;

	PerfTimer			ptimer;
	uint64				frame_count = 0;
	uint64				frame_new_count = 0;
	Timer				startup_time;
	Timer				new_startup_time;
	Timer				ms_timer;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	uint				frame_rate = 60;
	float				dt = 0.0f;
	float				avg_fps = 1.00f;
	float				curr_fps = 60.00f;

	std::list<ConsoleText> logs;

public:

	Application();
	~Application();

	bool Init();
	update_status Load();
	update_status Save();
	update_status Update();
	bool CleanUp();

	float GetAvgFPS();
	float GetAvgMs();
	uint GetCurrFPS();
	void CapFPS(float frame_r);

	void Log(ConsoleText text);
	std::list<ConsoleText> GetLogs() const;
	void ClearLogs();

private:

	void AddModule(Module* mod);
	void InitializeJSONDoc();
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;
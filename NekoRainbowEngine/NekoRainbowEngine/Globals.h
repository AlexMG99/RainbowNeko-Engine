#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

//LOG
enum ConsoleTextType;

#define LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__);
#define C_INFO(format, ...) _log(ConsoleTextType::CONSOLE_INFO, __FILE__, __LINE__, format, __VA_ARGS__);
#define C_SUCCES(format, ...) _log(ConsoleTextType::CONSOLE_SUCCESS, __FILE__, __LINE__, format, __VA_ARGS__);
#define C_WARNING(format, ...) _log(ConsoleTextType::CONSOLE_WARNING, __FILE__, __LINE__, format, __VA_ARGS__);
#define C_ERROR(format, ...) _log(ConsoleTextType::CONSOLE_ERROR, __FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

void _log(const char file[], int line, const char* format, ...);
void _log(ConsoleTextType type, const char file[], int line, const char * format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define ASSETS_FOLDER "./Assets/"
#define ASSETS_META_FOLDER "/Assets/Meta/"
#define SETTINGS_FOLDER "/Settings/"
#define LIBRARY_FOLDER "/Library/"
#define LIBRARY_SCENE_FOLDER "./Library/Scenes/"
#define LIBRARY_TEXTURES_FOLDER "/Library/Textures/"
#define LIBRARY_MESH_FOLDER "/Library/Meshes/"
#define LIBRARY_MODELS_FOLDER "/Library/Models/"

typedef unsigned int uint;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_SIZE 1
#define VSYNC true

// Deletes a buffer
#define RELEASE( x )		\
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete x;             \
	  x = nullptr;          \
	    }                   \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )  \
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete[] x;           \
	  x = nullptr;          \
	    }                   \
                            \
    }

#define RELEASE_LIST(x)											\
{																\
	for (auto it_list = x.begin(); it_list != x.end();)			\
	{															\
		it_list = x.erase(it_list);								\
	}															\
	x.clear();													\
}																\

#define RELEASE_ARRAY_LIST(x)									\
{																\
	for (auto it_list = x.begin(); it_list != x.end();)			\
	{															\
		RELEASE(*it_list);										\
		it_list = x.erase(it_list);								\
	}															\
	x.clear();													\
}																\


// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) //LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())
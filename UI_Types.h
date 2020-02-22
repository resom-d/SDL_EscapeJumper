#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "UI_Types.h"

// Handle the Widget Status
typedef enum WidgetState
{
	Focused = 0,
	Active = 1 << 0,
	Visible = 1 << 1,
	InputActive = 1 << 2
};

typedef enum WidgetEventType
{
	Activate = 0,
	MouseDown,
	
};

// Basic Widget Event
typedef void(*WidgetEventCallback)(void*, void*);


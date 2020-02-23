#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "UI_Types.h"
using namespace std;

const Uint32 UI_EDITOR_EVENT_TYPE = 40000;

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

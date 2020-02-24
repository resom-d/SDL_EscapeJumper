#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "UI_Types.h"
using namespace std;

const Uint32 GAME_EVENT_TYPE = 40000;
const Uint32 EDITOR_EVENT_TYPE = 40001;
const Uint32 UI_EDITOR_EVENT_TYPE = 40002;

// Handle the Widget Status
typedef enum WIDGET_STATE
{
	Focused = 0,
	Active = 1 << 0,
	Visible = 1 << 1,
	InputActive = 1 << 2
};

typedef enum WIDGET_ALIGNMENT
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER

};

typedef enum WIDGET_STRETCHMODE
{
	STRETCH_FIT = 0,
	STRETCH_KEEP
};

typedef enum WIDGET_ORIENTATION
{
	HORIZONTAL = 0,
	VERTICAL
};

typedef enum EDITOR_ACTION
{
	EDITOR_DRAWMODE = 1,
	EDITOR_ERASEMODE,
	EDITOR_SCROLL_RIGHT,
	EDITOR_SCROLL_LEFT,
	EDITOR_SCROLL_BLOCK_LEFT,
	EDITOR_SCROLL_BLOCK_RIGHT,
	EDITOR_SCROLL_BLOCK_START,
	EDITOR_SCROLL_BLOCK_END,
	EDITOR_BORDERDRAWMODE,
	EDITOR_SET_FILL_COLOR,
	EDITOR_SET_BORDER_COLOR
};

typedef enum WIDGET_EVENT_TYPE
{
	Activate = 0,
	MouseDown,
	
};


class Userdata
{
public:
	Uint16 ColorIndex;
	SDL_Color Color;
};
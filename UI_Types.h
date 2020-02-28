#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_Extras.h"
#include <iostream>
#include <list>
#include <string>

using namespace std;

const Uint32 GAME_EVENT_TYPE = 40000;
const Uint32 EDITOR_EVENT_TYPE = 40001;
const Uint32 UI_EDITOR_EVENT_TYPE = 40002;

// Handle the Widget Status
enum class WIDGET_STATE
{
	Focused = 0,
	Active = 1 << 0,
	Visible = 1 << 1,
	InputActive = 1 << 2
};
		  
enum class WIDGET_ALIGNMENT
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER

};
		  
enum class WIDGET_STRETCHMODE
{
	FIT = 0,
	KEEP
};
		  
enum class WIDGET_ORIENTATION
{
	HORIZONTAL = 0,
	VERTICAL
};
		  
enum class EDITOR_ACTION
{
	NONE = 0,
	DRAWMODE,
	ERASEMODE,
	SCROLL_RIGHT,
	SCROLL_LEFT,
	SCROLL_BLOCK_LEFT,
	SCROLL_BLOCK_RIGHT,
	SCROLL_BLOCK_START,
	SCROLL_BLOCK_END,
	BORDERDRAWMODE,
	SET_FILL_COLOR,
	SET_BORDER_COLOR,
	SET_TILEINDEX
};
		  
enum class WIDGET_EVENT_TYPE
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
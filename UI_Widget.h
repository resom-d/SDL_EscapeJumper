#pragma once
#include <SDL.h>

// Handle the Widget Status
typedef enum WidgetState
{
	Focused = 0,
	Active = 1 << 0,
	Visible = 1 << 1, 
	InputActive = 1 << 2
};

// Event Callback Prototype

class UI_Widget
{
public:
	UI_Widget();

	virtual void OnInit(SDL_Renderer* renderer) = 0;
	virtual void OnLoop() = 0;
	virtual void OnEvent() = 0;
	virtual void OnRender() = 0;
	virtual void OnCleanup() = 0;

	// Widget Events
	static void ActivatedCallback(UI_Widget *, void*);
	virtual void Activated(UI_Widget*, void*)=0;
	

	static void DeactivatedCallback(UI_Widget*, void*);
	virtual void Deactivated(UI_Widget*, void*) = 0;
	
	virtual void Focused(UI_Widget*, void*) = 0;
	virtual void LostFocus(UI_Widget*, void*) = 0;
	
	virtual void Shown(UI_Widget*, void*) = 0;
	virtual void Hidden(UI_Widget*, void*) = 0;

	WidgetState State = Visible ;
	SDL_Rect DisplayRect;
};

typedef void(*WidgetEvent)(UI_Widget*, void*);

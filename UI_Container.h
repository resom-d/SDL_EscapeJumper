#pragma once
#include "UI_Button.h"
#include <algorithm>
using namespace std;

class UI_Container : 	public UI_Widget
{
public:
	WIDGET_ORIENTATION Orientation = HORIZONTAL;

	virtual void OnInit(SDL_Renderer*);
	virtual void OnInit(SDL_Renderer*, Uint16 actionCode);
	virtual void OnRender();
	virtual void OnEvent(SDL_Event*);

	virtual void AddChild(UI_Button);

private:
	list<UI_Button> _children;
};


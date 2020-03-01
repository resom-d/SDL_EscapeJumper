#pragma once
#include "UI_Types.h"
#include "UI_Control.h"
using namespace std;


class UI_Container : 	public UI_Widget
{
public:
	UI_Container();
	~UI_Container();

	WIDGET_ORIENTATION Orientation = WIDGET_ORIENTATION::HORIZONTAL;

	virtual void OnInit(SDL_Renderer*);
	virtual void OnInit(SDL_Renderer*, UI_ACTION actionCode);
	virtual void OnRender();
	virtual void OnEvent(SDL_Event*);
	virtual void OnMouseMove(SDL_MouseButtonEvent event);
	virtual void OnCleanUp(void);
	virtual void AddChild(UI_Control);

private:
	list<UI_Control> _children;
};


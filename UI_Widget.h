#pragma once
#include "UI_Types.h"
#include "SDL_AdditionalPrimitives.h"
#include <SDL.h>
#include <iostream>
#include <list>
using namespace std;

class UI_Widget
{
public:
	UI_Widget();

	WidgetState State = Visible ;
	SDL_Rect DisplayRect;
	Uint16 BorderWidth;
	SDL_Color FillColor;
	SDL_Color BorderColor;
	SDL_Color FillColorActive;
	SDL_Color BorderColorActive;
	SDL_Color FillColorHover;
	SDL_Color BorderColorHover;
	
	void OnInit(SDL_Renderer* renderer);
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender();
	void OnCleanup();
	
	void ConnectEvent(WidgetEventType, WidgetEventCallback);
	void DisconnectEvent(WidgetEventType, WidgetEventCallback);

	void OnMouseButtonDown(SDL_MouseButtonEvent button);

private:
	SDL_Renderer* _renderer;

protected:
	list<WidgetEventCallback> _onClickCallbacks;
	list<WidgetEventCallback> _onHoverCallbacks;
	list<WidgetEventCallback> _onKeyDownCallbacks;
	list<WidgetEventCallback>::iterator _onEventIter;
};



#pragma once
#include "UI_Types.h"
#include "SDL_AdditionalPrimitives.h"
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <list>
using namespace std;

typedef enum WIDGET_ALIGNMENT
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER
};


class UI_Widget
{
public:
	UI_Widget();
	
	WidgetState State = Visible ;
	Uint16 ActionCode;
	SDL_Rect DisplayRect;
	Uint16 BorderWidth;
	Uint16 Margin=0, Padding=0;
	SDL_Color FillColor = { 200,200,200,255 };
	SDL_Color BorderColor = {0, 0, 0,255 };
	SDL_Color FillColorActive;
	SDL_Color BorderColorActive;
	SDL_Color FillColorHover;
	SDL_Color BorderColorHover;	
	WIDGET_ALIGNMENT HorizontalAlignment;
	WIDGET_ALIGNMENT VerticalAlignment;
	WIDGET_ALIGNMENT HorizontalContentAlignment;
	WIDGET_ALIGNMENT VerticalContentAlignment;

	// Methods
	virtual void OnInit(SDL_Renderer* renderer);
	virtual void OnInit(SDL_Renderer* renderer, Uint16 actionCode);
	virtual void OnLoop();
	virtual void OnEvent(SDL_Event*);
	virtual void OnRender();
	virtual void OnCleanup();

	virtual void OnMouseButtonDown(SDL_MouseButtonEvent button);

private:

protected:
	SDL_Renderer* _renderer;
	SDL_Color _fillColor;
	SDL_Color _borderColor;
	SDL_Event _event;
	Uint32 _eventType;

};



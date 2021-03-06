#pragma once
#include "UI_Types.h"

using namespace std;


class UI_Widget
{
public:
	UI_Widget();
	
	WIDGET_STATE State = WIDGET_STATE::Visible ;
	bool IsActive;
	bool IsVisible;
	bool IsEnabled;
	bool IsRepeatActive;
	Uint16 RepeatStartDelay;
	Uint16 RepeatDelay;
	SDL_Event Event;
	Uint32 EventType;
	UI_ACTION ActionCode;
	Userdata UserData;
	SDL_Rect DisplayRect;
	Uint16 BorderWidth;
	Uint16 Margin=0, Padding=0;
	SDL_Color FillColor = { 200,200,200,255 };
	SDL_Color BorderColor = {0, 0, 0,255 };
	SDL_Color FillColorActive = { 0,255,0,255 };
	SDL_Color BorderColorActive{ 255,255,255,255 };
	SDL_Color FillColorHover;
	SDL_Color BorderColorHover;	
	WIDGET_ALIGNMENT HorizontalAlignment;
	WIDGET_ALIGNMENT VerticalAlignment;
	WIDGET_ALIGNMENT HorizontalContentAlignment;
	WIDGET_ALIGNMENT VerticalContentAlignment;
	WIDGET_STRETCHMODE HorizontalStretch;
	WIDGET_STRETCHMODE VerticalStretch;

	// Methods
	virtual void OnInit(SDL_Renderer* renderer);
	virtual void OnInit(SDL_Renderer* renderer, UI_ACTION actionCode);
	virtual void OnLoop();
	virtual void OnEvent(SDL_Event*);
	virtual void OnRender();
	virtual void OnCleanup();
	virtual void OnMouseButtonDown(SDL_MouseButtonEvent button);
	virtual void OnMouseButtonUp(SDL_MouseButtonEvent button);
	virtual void OnMouseMove(SDL_MouseButtonEvent event);

private:

protected:
	SDL_Renderer* _rend;
	SDL_Color _fillColor;
	SDL_Color _borderColor;
   bool	_mouseIsDown;
   Uint32 _repeatStartTimeCatch;
   Uint32 _repeatTimeCatch;
   Uint32 _repeatTime;
   Uint32 _repeatTime_1n;
};



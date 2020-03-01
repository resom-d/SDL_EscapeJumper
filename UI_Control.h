#pragma once
#include "UI_Types.h"
#include "UI_Widget.h"
using namespace std;

class UI_Control :	public UI_Widget
{
public:
	UI_Control();	

	virtual void OnInit(SDL_Renderer* rend);
	virtual void OnInit(SDL_Renderer* rend, string text);
	virtual void OnInit(SDL_Renderer* rend, SDL_Texture* surf);
	virtual void OnInit(SDL_Renderer* rend, SDL_Texture* surf,  string text);
	virtual void OnEvent(SDL_Event* event);
	virtual void OnRender(void);
	virtual void OnCleanup(void);
	
	virtual void OnMouseButtonDown(SDL_MouseButtonEvent event);
	virtual void OnMouseMove(SDL_MouseButtonEvent event);

private:
	SDL_Texture* _texture = nullptr;
	string _text = "";

};


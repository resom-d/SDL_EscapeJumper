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
	virtual void OnInit(SDL_Renderer* rend, SDL_Texture* tex);
	virtual void OnRender(void);
	virtual void OnCleanup(void);


private:
	SDL_Texture* _texture = nullptr;
	string _text = "";

};


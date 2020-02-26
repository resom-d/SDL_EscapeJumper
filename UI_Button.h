#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "UI_Widget.h"
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

class UI_Button :	public UI_Widget
{
public:
	UI_Button();
	

	virtual void OnInit(SDL_Renderer* rend, string text);
	virtual void OnInit(SDL_Renderer* rend, SDL_Texture* surf);
	virtual void OnInit(SDL_Renderer* rend, SDL_Texture* surf,  string text);

	virtual void OnRender(void);
	virtual void OnCleanup(void);

private:
	SDL_Texture* _texture = nullptr;
	string _text = "";

};


#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
using namespace std;

class GameComponentBase
{
public :
	GameComponentBase();
	~GameComponentBase();

	SDL_Rect DisplayRect;

	virtual void OnInit(SDL_Renderer* rend);
	virtual void OnLoop(void) = 0;
	virtual void OnEvent(SDL_Event* event) = 0;
	virtual void OnRender() = 0;
	virtual void OnCleanup() = 0;

protected:
	SDL_Renderer* _rend;
	
};


#pragma once
#include<stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TextScroller
{
public:
	TextScroller();

	SDL_Rect DestRect;

	void OnInit(SDL_Renderer* rend, string msg, TTF_Font* font, SDL_Color clr, Uint16 speed);
	void OnLoop();
	void OnRender();
	void OnCleanUp();
	SDL_Point GetSurfaceSize();

private:

protected:
	string _theMessage;
	SDL_Renderer* _renderer;
	SDL_Texture* _texMessage;	
	TTF_Font *_font;
	SDL_Color _fontColor = { 0, 0, 0, 255 };
	Uint16 _speed=80;
	int _currentPosition=0;
	SDL_Point _surfaceSize;
	
};


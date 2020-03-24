#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "SDL_Extras.h"
using namespace std;

typedef struct CharItem
{
	char Content;
	Uint16 Width;
	Uint16 Height;
	SDL_Point ScreenPos;
	SDL_Texture* Texture;
};

class TextScroller
{
public:
	TextScroller();

	SDL_Rect DisplayRect;

	void OnInit(SDL_Renderer* rend, string msg, TTF_Font* font, SDL_Color clr, Uint16 speed);
	void OnLoop();
	void OnRender();
	void OnCleanUp();
	SDL_Point GetSurfaceSize();

private:

protected:
	string _theMessage;
	CharacterTextureMap _charMap;
	SDL_Renderer* _rend;
	TTF_Font *_font;
	SDL_Color _fontColor = { 0, 0, 0, 255 };
	Uint16 _speed=1;
	int _currentPosition=0;
	list<CharItem> _screenItems;
	Uint16 _lastItemWidth;
	Uint16 _charIndex;
	
		
};


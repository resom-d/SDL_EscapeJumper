#pragma once
#include<stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

class TextScroller
{
public:
	TextScroller();

	void OnInit(SDL_Renderer* rend, const char* msg, TTF_Font* font, SDL_Color clr, Uint16 speed, SDL_Rect *destRect);
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:

protected:
	char* theMessage;
	SDL_Renderer* _renderer;
	SDL_Texture* txtTexture;
	SDL_Rect surfaceSize;
	TTF_Font *font;
	SDL_Color fontColor = { 0, 0, 0, 255 };
	Uint16 speed=80;
	SDL_Rect destRect;
	int currentPosition=0;
	Uint32 ticks, ticks_1n;
	bool isEnabled = false;
};


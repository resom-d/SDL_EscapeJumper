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

	void OnInit(SDL_Renderer* rend, const char* msg, TTF_Font* font, SDL_Color clr, Uint16 speed, Uint16 yPos);
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:

protected:
	char* theMessage;
	SDL_Renderer* renderer;
	SDL_Surface* txtSurface;
	SDL_Texture* txtTexture;
	TTF_Font *font;
	SDL_Color fontColor = { 0, 0, 0, 255 };
	Uint16 speed=80;
	Uint16 y_pos;
	int currentPosition=0;
	Uint32 ticks, ticks_1n;
	SDL_Rect surfaceSize;
	bool isEnabled = false;
};


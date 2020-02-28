#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include "UI_Types.h"
#include "GameTypes.h"

using namespace std;

class SDL_Extras
{
public:
	static CharacterMap SDL_GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font);
	
	static void SDL_RenderStringAt(SDL_Renderer* rend, string text, SDL_Point p, CharacterMap chars, Uint16 size, SDL_Rect* clipRect);

	static void SDL_RenderSetPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	static void SDL_RenderDrawCircle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	static void SDL_RenderFillCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	static void SDL_RenderDrawBorder(SDL_Renderer* rend, SDL_Rect rect, Uint16 borderWidth, SDL_Color color);

	static void SDL_RenderSetDrawColor(SDL_Renderer* rend, SDL_Color col);

	static SDL_Texture* SD_RenderLoadTexture(SDL_Renderer* renderer, std::string path);
};


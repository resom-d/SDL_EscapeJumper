#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
using namespace std;

class SDL_AdditionalFunctions
{
public:
	static map<char, SDL_Texture*> GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font);
	static void SDL_RenderStringAt(SDL_Renderer* rend, string text, SDL_Point p, map<char, SDL_Texture*> chars, Uint16 size);


};


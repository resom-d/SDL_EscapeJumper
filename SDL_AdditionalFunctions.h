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


};


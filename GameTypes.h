#pragma once
#include <SDL.h>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

typedef unordered_map<char, SDL_Texture*> CharacterTextureMap;
typedef list<SDL_Color> ColorPalette;
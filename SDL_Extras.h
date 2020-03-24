#pragma once
#include <SDL.h>
#include <boost/filesystem.hpp>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <sstream>
#include "UI_Types.h"
#include "GameTypes.h"
#include "Size2D.h"

using namespace std;
using namespace boost::filesystem;

	CharacterTextureMap SDL_GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font, SDL_Color color);
	
	void SDL_RenderStringAt(SDL_Renderer* rend, string text, SDL_Point p, CharacterTextureMap chars, Uint16 size, SDL_Rect* clipRect);

	void SDL_RenderSetPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	void SDL_RenderDrawCircle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	void SDL_RenderFillCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	void SDL_RenderDrawBorder(SDL_Renderer* rend, SDL_Rect rect, Uint16 borderWidth, SDL_Color color);

	void SDL_RenderSetDrawColor(SDL_Renderer* rend, SDL_Color col);
		
	void CreateTilemap(path sdir, path ddir, string filename, Uint16 cols, Uint16 rows, Size2D outsize, SDL_Color* fillcolor);
	
	void CreateWidgetTexture(SDL_Renderer* rend, string filePath, SDL_Texture* destTex, SDL_Rect srcRect, SDL_Rect destRect, double rot, SDL_RendererFlip flip);

	SDL_Texture* SDL_LoadTexture(SDL_Renderer* rend, path filename);

	list<string> SplitString(string sourceString, const char splitter);

	list<path> GetFilesInDirectory(path dir);
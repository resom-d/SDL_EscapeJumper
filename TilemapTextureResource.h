#pragma once
#include <SDL.h>
#include <string>
#include "Size2D.h"
using namespace std;

class TileMapTextureResource
{
public:
	TileMapTextureResource();
	~TileMapTextureResource();
	string Path;
	SDL_Texture* Texture;
	Uint16 Cols;
	Uint16 Rows;
	Uint16 MaxIndex;
	Size2D Tilesize;

	virtual void SaveTextureResourceDescription(string filename);
	virtual void LoadTextureResourceDescription(string filename);
	virtual string toString();

};


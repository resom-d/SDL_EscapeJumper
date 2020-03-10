#pragma once
#include <SDL.h>
#include <string>
#include <list>
#include "Size2D.h"
using namespace std;


enum class TileType
{
	None = 0,
	Background,
	Coin,
	World,
	Door,
	Water,
	Fire,
	Teleport,
	Level_Goal,
	HealhUp
};

typedef struct TilemapTile
{
	TileType Type = TileType::Background;
	Uint16 ResourceIndex = 0;
	Uint16 TileIndex = 0;
	Uint16 FillColor = 0;
	Uint16 BorderColor = 0;
	bool Visible = false;
	bool InView = true;
};

typedef struct TilemapSetup
{
	int Rows = 0;
	int Cols = 0;
	Uint16 BlockSize = 50;
	Uint16 BlockSpacing = 0;
	Uint16 DisplayCols = 20;
	Uint16 DisplayRows = 10;
	SDL_Rect DisplayRect = { 1, 1, 1,1 };
	int ScreenOffsX;
	SDL_Color Background;
};

typedef TilemapTile** TileMap;

class TileMapTextureResource
{
public:
	TileMapTextureResource();
	~TileMapTextureResource();
	string Path;
	TileType Type = TileType::None;
	SDL_Texture* Texture;
	Size2D Tilesize;
	Uint16 Cols;
	Uint16 Rows;
	Uint16 MaxIndex;

	virtual void SaveTextureResourceDescription(string filename);
	virtual void LoadTextureResourceDescription(string filename);
	virtual string toString();

};

typedef list<TileMapTextureResource> TileMapTextureResourceList;

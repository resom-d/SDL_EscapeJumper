#pragma once
#include "GameTypes.h"
#include "SDL_Extras.h"
using namespace std;

typedef struct Size2D
{
	int w , h;

};

typedef struct Size3D
{
	int w, h, d;

};

typedef struct TileMapTextureResource
{
	string Path;
	SDL_Texture* Texture;
	Uint16 Cols;
	Uint16 Rows;
	Uint16 MaxIndex;
	Size2D Tilesize;
	
};

typedef list<TileMapTextureResource> TileMapTextureResourceList;

enum class TileType
{
	None=0,
	Background,
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
};

typedef struct TilemapSetup
{
	int Rows = 100;
	int Cols = 1000;
	Uint16 BlockSize = 35;
	Uint16 BlockSpacing = 1;
	Uint16 DisplayCols = 40;
	Uint16 DisplayRows = 20;
	SDL_Rect DisplayRect = { 1, 1, 1,1 };
	int ScreenOffsX;
	SDL_Color Background;
};

typedef TilemapTile** TileMap;


class GameMap
{
public:
	GameMap();
	~GameMap();

	TilemapSetup Setup;
	TileMap TileMap;
	ColorPalette ColorPallete;
	TileMapTextureResourceList TextureResources;

	virtual void OnInit(SDL_Renderer* rend);
	virtual void OnRender(SDL_Point blockpos, SDL_Point scrollpos);
	virtual void OnCleanUp();

	virtual void InitMap();
	virtual void SetTileInMap(MapCoords, TilemapTile settings);
	virtual void FillArea(MapCoords p1, MapCoords p2, MapCoords offs, TilemapTile org);
	virtual TilemapTile GetTileAt(Uint16 col, Uint16 row);

private:

protected:
	SDL_Renderer* _rend;
};


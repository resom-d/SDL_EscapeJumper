#pragma once
#include <sstream>
#include "GameTypes.h"
#include "SDL_Extras.h"
#include "TilemapTextureResource.h"
using namespace std;

typedef list<TileMapTextureResource> TileMapTextureResourceList;

enum class TileType
{
	None = 0,
	Background,
	World,
	Door,
	Water,
	Fire,
	Teleport,
	Level_Goal,
	HealhUp
};

enum class EngineViewMode
{
	Game,
	Editor
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


class GameMap
{
public:
	GameMap();
	~GameMap();

	TilemapSetup Setup;
	TileMap TileMap;
	ColorPalette ColorPallete;
	TileMapTextureResourceList TextureResources;
	EngineViewMode ViewMode;
	SDL_Point ScrollPosition = { 0,0 };
	SDL_Point BlockPosition = { 0,0 };
	int ScrollSpeed = 1;
	int ScrollXInDelay = 0;
	int ScrollXOutDelay = 0;
	int ScrollYInDelay = 0;
	int ScrollYOutDelay = 0;
	bool LevelDone = false;
	
	virtual void OnInit(SDL_Renderer* rend);
	virtual void OnRender();
	virtual void OnLoop(void);
	virtual void OnCleanUp(void);

	virtual void InitMap(void);
	void ResetScroller(void);
	void ResetInView(void);
	virtual void SaveMap(string filename);
	static GameMap LoadMap(SDL_Renderer* rend, string filename);
	virtual void SetTileInMap(SDL_Point coords, TilemapTile settings);
	virtual void FillArea(SDL_Point p1, SDL_Point p2, SDL_Point offs, TilemapTile org);
	virtual TilemapTile GetTileAt(Uint16 col, Uint16 row);

private:

protected:
	SDL_Renderer* _rend;
};


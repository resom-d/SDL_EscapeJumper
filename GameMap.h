#pragma once
#include <sstream>
#include "GameTypes.h"
#include "SDL_Extras.h"
#include "TilemapTextureResource.h"
using namespace std;

enum class EngineViewMode
{
	Game,
	Editor
};

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


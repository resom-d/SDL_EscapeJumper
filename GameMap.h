#pragma once
#include "GameTypes.h"
#include "SDL_Extras.h"

class GameMap
{
public:
	GameMap();
	~GameMap();

	MatrixSetup Setup;
	ColorPalette ColorPallete;
	
	virtual void OnInit(SDL_Renderer* rend);
	virtual void OnRender(SDL_Point blockpos, SDL_Point scrollpos);
	virtual void OnCleanUp();

	virtual void InitMap();
	virtual void SetTileInMap(MapCoords, MatrixTile settings);
	virtual void FillArea(MapCoords p1, MapCoords p2, MapCoords offs, MatrixTile org);
private:

protected:
	SDL_Renderer* _rend;
	GameMatrix _map;
};


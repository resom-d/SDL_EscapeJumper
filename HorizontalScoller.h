#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <list>
#include "SDL_Extras.h"
#include "GameTypes.h"
#include "GameMap.h"
using namespace std;

class HorizontalScroller
{

public:
	HorizontalScroller();

	int ColumnPosition = 0;
	SDL_Point ScrollPosition = { 0,0 };
	SDL_Point BlockPosition = { 0,0 };
	int ScrollSpeed = 1;
	bool LevelDone = false;
	// Game functions
	void OnInit(SDL_Renderer* renderer,  GameMap* map);
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:
	SDL_Renderer* _rend;
	GameMap* _map;
		

};


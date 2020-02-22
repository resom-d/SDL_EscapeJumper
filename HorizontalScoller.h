#pragma once

#include "GameTypes.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <list>
using namespace std;

class HorizontalScoller
{

public:
	HorizontalScoller();

	int ColumnPosition = 0;
	int ScrollPosition = 0;
	int ScrollSpeed = 1;
	MatrixSetup* MapSetup;
	MatrixRectItem** MapMatrix;
	list<SDL_Color> ColorPalette;
	bool LevelDone = false;
	// Game functions
	void OnInit(SDL_Renderer* renderer, MatrixSetup* setup, MatrixRectItem** map);
	void InitScollerContent();
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:
	SDL_Renderer* _renderer;
		

};


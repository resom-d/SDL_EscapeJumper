#pragma once

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "GameTypes.h"

class HorizontalScoller
{

public:
	HorizontalScoller();

	int ColumnPosition = 0;
	int ScrollPosition = 0;
	int ScrollSpeed = 1;
	MatrixSetup* MapSetup;
	MatrixRectItem** MapMatrix;
	
	// Game functions
	void OnInit(SDL_Renderer* renderer, MatrixSetup* setup, MatrixRectItem** map);
	void InitScollerContent();
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:
	SDL_Renderer* _renderer;
		

};


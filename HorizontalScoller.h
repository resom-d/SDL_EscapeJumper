#pragma once

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

typedef struct MatrixSetup
{
	int Rows = 100;	
	int Cols = 1000;
	Uint16 BlockSize=35;
	Uint16 BlockSpacing = 1;
	Uint16 DisplayColumns = 40;
	Uint16 DisplayRows = 20;
	SDL_Rect DisplayRect = {1, 1, 1,1};
	int ScreenOffsX;
} ;

typedef struct MatrixItem
{
	SDL_Color Color;
};

class HorizontalScoller
{

public:
	HorizontalScoller();

	int ColumnPosition = 0;
	int ScrollPosition = 0;
	int ScrollSpeed = 1;
	MatrixSetup MatrixSetup;
	MatrixItem ColumnContainer[1000][20];
	
	// Game functions
	void OnInit(SDL_Renderer* renderer);
	void InitScollerContent();
	void OnLoop();
	void OnRender();
	void OnCleanUp();

private:
	SDL_Renderer* _renderer;
		

};


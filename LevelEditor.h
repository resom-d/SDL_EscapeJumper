#pragma once
#include "GameTypes.h"
#include <SDL.h>
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

class LevelEditor
{
public:

	LevelEditor();

	EditorMode Mode = EditorMode_Draw;
	MatrixSetup* MapSetup;
	MatrixRectItem** MapMatrix;
	list<MatrixRectItem> DrawCollection;
	list<SDL_Color> ColorPalettte;
	SDL_Color ActiveFillColor;
	SDL_Color ActiveBorderColor;
	SDL_Point ActiveIndex;
	int ColumnPosition = 0;
	int RowPosition = 0;
	int ScrollPosition = 0;
	int ScrollSpeed = 1;

	void OnInit(SDL_Window* win, SDL_Renderer* rend, MatrixRectItem** map, MatrixSetup* setup);

	void OnLoop();

	void OnRender();

	void OnEvent(SDL_Event* event);

	void OnCleanUp();

	void OnLoadMap();

	void OnSaveMap();

	void OnClearMap();

	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);

	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);

	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);

	void OnLeftButtonUp(int mX, int mY);
	void OnLeftButtonDown(int mX, int mY);
	void OnRightButtonUp(int mX, int mY);
	void OnRightButtonDown(int mX, int mY);
	void OnMiddleButtonUp(int mX, int mY);
	void OnMiddleButtonDown(int mX, int mY);

private:
	SDL_Window* _appWindow;
	SDL_Renderer* _renderer;
	int _outputHeight, _outputWidth;
	bool _drawActive;
	int _colorIndexFill;
	int _colorIndexBorder;
};
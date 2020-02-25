#pragma once
#include "GameTypes.h"
#include "UI_Editor.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
using namespace std;

class LevelEditor
{
public:

	LevelEditor();

	UI_Editor UI;
	EDITOR_ACTION Mode = EDITOR_DRAWMODE;
	MatrixSetup* MapSetup;
	MatrixRectItem** MapMatrix;
	list<MatrixRectItem> DrawCollection;
	list<SDL_Color> ColorPalette;
	SDL_Rect DisplayRect;
	Uint16 UI_Height = 200;
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
	
	void OnLeftButtonUp(int mX, int mY);
	void OnLeftButtonDown(int mX, int mY);
	void OnRightButtonUp(int mX, int mY);
	void OnRightButtonDown(int mX, int mY);
	void OnMiddleButtonUp(int mX, int mY);
	void OnMiddleButtonDown(int mX, int mY);
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);

private:
	SDL_Window* _appWindow;
	SDL_Renderer* _renderer;
	int _outputHeight, _outputWidth;
	bool _drawActive;
	bool _eraseActive;
	
	SDL_Point _blockdrawStart;
	SDL_Point _blockdrawEnd;
	SDL_Point _blockdrawStartScreen;
	SDL_Point _blockdrawEndScreen;

	int _colorIndexFill;
	int _colorIndexBorder;
	list<SDL_Color>::iterator _colorPalletIterator;
	
	void FillArea(SDL_Point p1, SDL_Point p2, SDL_Point offset, Uint16 cf, Uint16 cb);

};
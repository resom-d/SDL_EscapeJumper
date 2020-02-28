#pragma once
#include <SDL.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "GameTypes.h"
#include "GameMap.h"
#include "UI_Editor.h"

using namespace std;

class LevelEditor
{
public:

	LevelEditor();

	UI_Editor UI;
	EDITOR_ACTION Mode = EDITOR_ACTION::DRAWMODE;
	SDL_Rect DisplayRect;
	Uint16 UI_Height = 200;
	SDL_Point ActiveIndex;
	int ColumnPosition = 0;
	int RowPosition = 0;
	int ScrollPosition = 0;
	int ScrollSpeed = 1;

	void OnInit(SDL_Window* win, SDL_Renderer* rend, GameMap* map, CharacterMap charMap);

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
	SDL_Renderer* _rend;
	GameMap* _map;
	CharacterMap _charMap;

	int _outputHeight, _outputWidth;
	bool _drawActive;
	bool _eraseActive;
	
	SDL_Point _blockdrawStart;
	SDL_Point _blockdrawEnd;
	SDL_Point _blockdrawStartScreen;
	SDL_Point _blockdrawEndScreen;

	int _colorIndexFill;
	int _colorIndexBorder;
		
};
#pragma once
#include <SDL.h>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "GameTypes.h"
#include "GameMap.h"
#include "UI_Editor.h"
#include "UI_MapConfig.h"
#include "SDL_Extras.h"

using namespace std;

class LevelEditor
{
public:

	LevelEditor();

	GameMap Map;
	list<path> Levels;
	UI_Editor UI;
	UI_ACTION Mode = UI_ACTION::DRAWMODE;
	SDL_Rect DisplayRect;
	Uint16 UI_Height = 200;
	int TileIndex = 0;
	/*int ResourceIndex = 0;*/
	int ColumnPosition = 0;
	int RowPosition = 0;
	SDL_Point ScrollPosition = { 0,0 };
	int ScrollSpeed = 1;
	
	void OnInit(SDL_Renderer* rend, CharacterTextureMap charMap);

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
	void ScrollMap(SDL_Point p);

private:
	SDL_Window* _tilemapWindow;
	SDL_Renderer* _rend;
	CharacterTextureMap _charMap;
	int _level = 0;
	int _colorIndexFill;
	int _colorIndexBorder;
	int _outputHeight;
	int _outputWidth;
	bool _drawActive;
	bool _eraseActive;
	bool _mapScrollMidlleMouse;
	
	SDL_Point _mapScrollDiff;

	SDL_Point _blockdrawStart;
	SDL_Point _blockdrawEnd;
	SDL_Point _blockdrawStartScreen;
	SDL_Point _blockdrawEndScreen;


};
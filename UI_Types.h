#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <list>
#include <string>
#include "SDL_Extras.h"
#include "UI_Styles.h"
#include "TilemapTextureResource.h"
using namespace std;

const Uint32 GAME_EVENT_TYPE = 40000;
const Uint32 EDITOR_EVENT_TYPE = 40001;
const Uint32 UI_EDITOR_EVENT_TYPE = 40002;

// Handle the Widget Status
enum class WIDGET_STATE
{
	Focused = 0,
	Active = 1 << 0,
	Visible = 1 << 1,
	InputActive = 1 << 2
};

enum class WIDGET_ALIGNMENT
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
	CENTER

};

enum class WIDGET_STRETCHMODE
{
	FIT = 0,
	KEEP
};

enum class WIDGET_ORIENTATION
{
	HORIZONTAL = 0,
	VERTICAL
};

enum class UI_ACTION
{
	NONE = 0,
	QUIT_GAME,
	GO_MAINSCREEN,
	GO_GAME,
	GO_EDITOR,
	GO_EDITOR_CONFIG,
	GO_EDITOR_NEWMAP,
	GO_EDITOR_TESTMODE,
	CLOSE_EDITOR_CONFIG,
	CLOSE_EDITOR_NEW_MAP,
	LOADMAP,
	SAVEMAP,
	EDIT_LOAD_PREV,
	EDIT_LOAD_NEXT,
	DRAWMODE,
	ERASEMODE,
	SCROLL_TO,
	BORDERDRAWMODE,
	EDITOR_TOGGLEGRID,
	SET_FILL_COLOR,
	SET_BORDER_COLOR,
	SET_TILEINDEX,
	PREV_RESOURCEINDEX,
	NEXT_RESOURCEINDEX,
	PREV_TILEMAP,
	NEXT_TILEMAP,
	SET_PLAYER_AVATAR,
	ADD_TILEMAP,
	SET_TILETYPE_BGND,
	SET_TILETYPE_COIN,
	SET_TILETYPE_ENERGY,
	SET_TILETYPE_DAMAGE,
	REPLACE_TILEMAP,
	MAP_NEW,
	MAP_CLEAR,
	MAP_DELETE
};

enum class WIDGET_EVENT_TYPE
{
	Activate = 0,
	MouseDown,

};

class Userdata
{
public:
	Userdata();
	Uint16 ColorIndex = 0;
	Uint16 ResourceIndex = 0;
	Uint16 TileIndex = 0;
	SDL_Point Scrollposition;
	SDL_Texture* Texture = nullptr;
	TileMapTextureResource* TilemapResource = nullptr;
	int NewMapCols;
	int NewMapRows;
	string* NewMapName;
};
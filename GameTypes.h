#pragma once
#include <SDL.h>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

enum class GameState
{
	Idle,
	MainScreen,
	LevelEdit,
	PlayerEdit,
	Running,
	Paused,
	Highscore,
	GameOver

};

enum class MotionState
{
	None = 0,
	Plus,
	Minus
};

enum class TileType
{
	Background = 0,
	Deadly,
	Damage,
	Teleport
};

typedef struct GameProperties
{
	SDL_Rect WindowFrame;
};

typedef struct MatrixSetup
{
	int Rows = 100;
	int Cols = 1000;
	Uint16 BlockSize = 35;
	Uint16 BlockSpacing = 1;
	Uint16 DisplayCols = 40;
	Uint16 DisplayRows = 20;
	SDL_Rect DisplayRect = { 1, 1, 1,1 };
	int ScreenOffsX;
	SDL_Color Background;
};

class MatrixTile
{
public:
	MatrixTile();
	~MatrixTile();

	TileType Type;
	Uint16 TileIndex;
	Uint16 FillColor;
	Uint16 BorderColor;
	bool Visible;
};

typedef unordered_map<char, SDL_Texture*> CharacterMap;
typedef list<SDL_Color> ColorPalette;
typedef pair<Uint16, Uint16> MapCoords;

typedef MatrixTile** GameMatrix;
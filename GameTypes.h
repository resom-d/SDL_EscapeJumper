#pragma once
#include <SDL.h>
#include <iostream>

typedef struct GameProperties
{
	SDL_Rect WindowFrame;
} ;

typedef enum GameState
{
	GameState_Idle,
	GameState_MainScreen,
	GameState_LevelEdit,
	GameState_PlayerEdit,
	GameState_Running,
	GameState_Paused,
	GameState_Highscore,
	GameState_GameOver

};

typedef enum MotionState
{
	None = 0,
	Plus,
	Minus
};

typedef struct MatrixSetup
{
	int Rows = 100;
	int Cols = 1000;
	Uint16 BlockSize = 35;
	Uint16 BlockSpacing = 1;
	Uint16 DisplayColumns = 40;
	Uint16 DisplayRows = 20;
	SDL_Rect DisplayRect = { 1, 1, 1,1 };
	int ScreenOffsX;
	SDL_Color Background;
};

typedef struct MatrixRectItem
{
	int BorderColor;
	int FillColor;
};

typedef enum EditorMode
{
	EditorMode_Select,
	EditorMode_Draw,
	EditorMode_Erase,
	EditorMode_Copy,
	EditorMode_Move,
	EditorMode_Fill
};

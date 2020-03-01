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

typedef struct GameProperties
{
	SDL_Rect WindowFrame;
};

typedef unordered_map<char, SDL_Texture*> CharacterTextureMap;
typedef list<SDL_Color> ColorPalette;


#pragma once
#include <SDL.h>
#include <string>
#include "GameEvents.h"
#include "GameTypes.h"
#include "GameMap.h"

enum class MotionState
{
	None = 0,
	Plus,
	Minus
};

class JumperPlayer : GameEvents
{
public:
	std::string TextureSourcePath;

	SDL_Renderer* _rend;
	SDL_Texture* Texture;
	SDL_Point MaxPosition;
	SDL_Point MinPosition;
	SDL_Rect DisplayRect;
	Uint16 HorizontalTiling;
	Uint16 VerticalTiling;
	Uint16 HorizontalTileSize;
	Uint16 VerticalTileSize;
	Uint16 NumberOfTiles;
	Uint16 AnimationFrame;
	Uint16 AnimationRate;
	int Speed;
	bool GameOver;
	bool Landed = true;
	MotionState MotionHor;
	MotionState MotionVer;
	
	std::string Name;
	int Score;

	int OnInit(SDL_Renderer* rend, GameMap* map);
	int OnLoop();
	void OnEvent(SDL_Event* Event);
	int OnRender();
	int OnCleanup();
	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);
	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);

private:
	GameMap* _map;
	void OnCollisionCheck();
};


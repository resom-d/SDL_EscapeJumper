#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameEvents.h"
#include "GamePlayer.h"
#include "HorizontalScoller.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "MainUI.h"
#include"JumperPlayer.h"

typedef struct
{
	SDL_Rect WindowFrame;


} GameProperties;

typedef enum GameState
{
	Idle,
	GameOver,
	Running,
	Paused
};

class GameEngine : public GameEvents
{

public:
	GameEngine();

	GameProperties Properties;
	Uint16 GlobalFrameRate;

	bool OnInit();

	void OnEvent(SDL_Event* Event);

	int OnExecute();

	void OnLoop();

	void OnRender();

	void OnCleanup();

	void OnCollisionCheck();

	void OnInitPlayer();

	void OnGameRestart();

	// Overrides from ZF_DemoEvents

	void OnExit();

	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);

	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);


private:
	bool _appIsRunning;

	GameState GameStatus = Running;
	SDL_Window* _appWindow;
	SDL_Renderer* _renderer;
	TTF_Font* _uiFont;

	MainUI _mainUI;
	JumperPlayer _player;
	HorizontalScoller _scroller;

	SDL_Joystick* _gamePad = NULL;

protected:

};
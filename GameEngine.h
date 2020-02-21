#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "GameTypes.h"
#include "GameEvents.h"
#include "GamePlayer.h"
#include "HorizontalScoller.h"
#include "MainUI.h"
#include"JumperPlayer.h"
#include "LevelEditor.h"

class GameEngine : public GameEvents
{

public:
	GameEngine();

	GameProperties Properties;
	bool _appIsRunning;
	Uint16 GlobalFrameRate;
	GameState GameStatus = GameState_Running;
	SDL_Window* AppWindow;
	SDL_Renderer* Renderer;
	MainUI MainUI;
	LevelEditor Editor;
	MatrixSetup MapSetup;
	MatrixRectItem** Map;
	JumperPlayer Player;
	HorizontalScoller Scroller;
	SDL_Joystick* GamePad = NULL;

	bool OnInit();

	void OnEvent(SDL_Event* Event);

	int OnExecute();

	void OnLoop();

	void OnRender();

	void OnPostRender();

	void OnCleanup();

	void OnCollisionCheck();

	void OnInitPlayer();

	void OnGameRestart();

	// Overrides from ZF_DemoEvents

	void OnExit();

	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);

	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);

	
private:

protected:

};
#include "GameEngine.h"
#include <SDL.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>

GameEngine::GameEngine()
{
	GlobalFrameRate = 60;
	Properties.WindowFrame.x = 50;
	Properties.WindowFrame.y = 50;
	Properties.WindowFrame.w = 1600;
	Properties.WindowFrame.h = 900;
}

int GameEngine::OnExecute()
{
	if (OnInit() == false) return -1;

	SDL_Event AppEvent;
	int timeDiff;
	int timerFPS_n = SDL_GetTicks();
	int timerFPS_1n = SDL_GetTicks();

	while (_appIsRunning)
	{
		//grab events	
		while (SDL_PollEvent(&AppEvent)) OnEvent(&AppEvent);

		// execute all logic
		OnLoop();
		// do all rendering
		OnRender();

		// make sure we are running at a constant frame rate
		timerFPS_n = SDL_GetTicks();
		timeDiff = timerFPS_n - timerFPS_1n;
		timerFPS_1n = timerFPS_n;
		if (timeDiff < 1000 / GlobalFrameRate) SDL_Delay(1000 / GlobalFrameRate - timeDiff);
	}

	// don't leave a messy place
	OnCleanup();

	return 0;
};

bool GameEngine::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 	return false;
	if (TTF_Init() == -1) return false;
	if ((_uiFont = TTF_OpenFont("SigmarOne-Regular.ttf", 32)) == nullptr) return false;

	if ((_appWindow = SDL_CreateWindow(
		"Zehnfinger's Game-Engine++ Running",
		Properties.WindowFrame.x,
		Properties.WindowFrame.y,
		Properties.WindowFrame.w,
		Properties.WindowFrame.h,
		SDL_WINDOW_SHOWN)
		) == nullptr) return false;

	if ((_gameRenderer = SDL_CreateRenderer(_appWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	_appIsRunning = true;
	
	player = GamePlayer(_gameRenderer);
	player.Properties.Name = "Mollmops";
	player.Properties.TextureSourcePath = "Spritesheet_Alien_01.png";
	player.Properties.HorTiling = 4;
	player.Properties.VerTiling = 4;


	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);
};

void GameEngine::OnLoop()
{

};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawColor(_gameRenderer, 80, 80, 80, 255);
	SDL_RenderClear(_gameRenderer);
	SDL_RenderPresent(_gameRenderer);
}

void GameEngine::OnCleanup()
{

	SDL_DestroyWindow(_appWindow);
	SDL_free(_gameRenderer);

	SDL_Quit();
};

// Overrides from GameEvents

void GameEngine::OnExit()
{
	_appIsRunning = false;
}

void GameEngine::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	// global keys
	if (sym == SDLK_ESCAPE) _appIsRunning = false;

	// player keys
	/*if (sym == SDLK_d && !_playerLeft) _playerRigth = true;
	if (sym == SDLK_a && !_playerRigth) _playerLeft = true;
	if (sym == SDLK_s && !_playerUp) _playerDown = true;
	if (sym == SDLK_w && !_playerDown) _playerUp = true;
	if (sym == SDLK_RIGHT && !_playerRotLeft) _playerRotRight = true;
	if (sym == SDLK_LEFT && !_playerRotRight) _playerRotLeft = true;*/
}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	/*if (sym == SDLK_d) _playerRigth = false;
	if (sym == SDLK_a) _playerLeft = false;
	if (sym == SDLK_s) _playerDown = false;
	if (sym == SDLK_w) _playerUp = false;
	if (sym == SDLK_RIGHT && !_playerRotLeft) _playerRotRight = false;
	if (sym == SDLK_LEFT && !_playerRotRight) _playerRotLeft = false;*/
}

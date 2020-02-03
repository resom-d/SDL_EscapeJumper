#include "GameEngine.h"
#include "TextScroller.h"
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

TextScroller T, T1;

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
	if ((_uiFont = TTF_OpenFont("SigmarOne-Regular.ttf", 120)) == nullptr) return false;

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
	player.Properties.AnimationRate = 40;		// t=1000/framesPerSecond e.g. 1000/25 = 40
	player.Properties.Name = "Mollmops";
	player.Properties.TextureSourcePath = "Spritesheet_Alien_01.png";
	player.Properties.HorizontalTiling = 6;
	player.Properties.VerticalTiling = 4;
	player.Properties.Position.x = Properties.WindowFrame.w >> 1;
	player.Properties.Position.y = Properties.WindowFrame.h >> 1;
	player.Properties.RotationAngle = 0.0;
	player.Properties.IsIdle = true;
	player.Properties.IsJumping = false;
	player.Properties.IsLanding = false;
	player.Properties.IsMovingDown = false;
	player.Properties.IsMovingLeft = false;
	player.Properties.IsMovingRight = false;
	player.Properties.IsMovingUp = false;
	player.Properties.IsRotatingLeft = false;
	player.Properties.IsRotatingRight = false;
	player.Properties.Speed = 2;
	player.Properties.RotationSpeed = 1.0;

	player.OnInit(_gameRenderer);
	SDL_Color c = { 252, 186, 3, 255};
	T.OnInit(_gameRenderer, "Zehnfinger's Game-Engine running. This scroller is here to inform you that the autor now finally halfways got behind what blitting is.", _uiFont, c, 30, 260);
	c = { 252, 0, 3, 255};
	T1.OnInit(_gameRenderer, "Just some stupid text to scroll around. ", _uiFont, c, 40, 440);

	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);
};

void GameEngine::OnLoop()
{
	T.OnLoop();
	T1.OnLoop();
	player.OnLoop();
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawColor(_gameRenderer, 80, 80, 80, 255);
	SDL_RenderClear(_gameRenderer);

	// Render background


	// Render UI
	T.OnRender();
	// Render player(s)
	player.OnRender();
	T1.OnRender();
	// Render Effects


	SDL_RenderPresent(_gameRenderer);
}

void GameEngine::OnCleanup()
{
	// don't change order here...
	T.OnCleanUp();
	T1.OnCleanUp();
	player.OnCleanup();

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
	if (sym == SDLK_d && !player.Properties.IsMovingLeft) player.Properties.IsMovingRight = true;
	if (sym == SDLK_a && !player.Properties.IsMovingRight)player.Properties.IsMovingLeft = true;
	if (sym == SDLK_s && !player.Properties.IsMovingUp) player.Properties.IsMovingDown = true;
	if (sym == SDLK_w && !player.Properties.IsMovingDown)player.Properties.IsMovingUp = true;
	if (sym == SDLK_RIGHT && !player.Properties.IsRotatingLeft) player.Properties.IsRotatingRight = true;
	if (sym == SDLK_LEFT && !player.Properties.IsRotatingRight) player.Properties.IsRotatingLeft = true;
}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (sym == SDLK_d) player.Properties.IsMovingRight = false;
	if (sym == SDLK_a) player.Properties.IsMovingLeft = false;
	if (sym == SDLK_s) player.Properties.IsMovingDown = false;
	if (sym == SDLK_w) player.Properties.IsMovingUp = false;
	if (sym == SDLK_RIGHT) player.Properties.IsRotatingRight = false;
	if (sym == SDLK_LEFT) player.Properties.IsRotatingLeft = false;
}

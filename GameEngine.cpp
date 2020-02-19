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
		"EscapeJumper - ein Zehnfinger Spiel",
		Properties.WindowFrame.x,
		Properties.WindowFrame.y,
		Properties.WindowFrame.w,
		Properties.WindowFrame.h,
		SDL_WINDOW_SHOWN)
		) == nullptr) return false;

	if ((_renderer = SDL_CreateRenderer(_appWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	if (SDL_NumJoysticks() > 0) _gamePad = SDL_JoystickOpen(0);

	_player.OnInit(_renderer);
	_mainUI.OnInit(_renderer);
	_scroller.OnInit(_renderer);
	_scroller.MatrixSetup.DisplayRect.y = 150;

	OnInitPlayer();

	_appIsRunning = true;
	GameStatus = Running;

	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);
	if (GameStatus == Running) _player.OnEvent(Event);
};

void GameEngine::OnLoop()
{
	if (GameStatus == Running)
	{
		_player.OnLoop();
		_scroller.OnLoop();
		OnCollisionCheck();
	}
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawColor(_renderer, 40, 40, 40, 255);
	SDL_RenderClear(_renderer);

	// Render background
	_scroller.OnRender();

	// Render player(s)
	_player.OnRender();

	// Render Effects

	// Render UI
	_mainUI.OnRender(_player.Name, _player.Score, GameStatus == GameOver);

	// do it! do it !
	SDL_RenderPresent(_renderer);
}

void GameEngine::OnCleanup()
{
	_scroller.OnCleanUp();
	_player.OnCleanup();

	SDL_DestroyWindow(_appWindow);
	SDL_free(_renderer);
	SDL_Quit();
}

void GameEngine::OnCollisionCheck()
{
	int playerScreenColumn, playerScreenRow;
	int playerEdgeT, playerEdgeL, playerEdgeR, playerEdgeB;
	int obstEdgeT, obstEdgeL, obstEdgeR, obstEdgeB;

	playerScreenColumn = (_player.DisplayRect.x - _scroller.MatrixSetup.ScreenOffsX) / (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing);
	playerScreenRow = (_player.DisplayRect.y - 150) / (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing);

	playerEdgeT = _player.DisplayRect.y;
	playerEdgeB = _player.DisplayRect.y + _scroller.MatrixSetup.BlockSize;
	playerEdgeL = _player.DisplayRect.x;
	playerEdgeR = _player.DisplayRect.x + _scroller.MatrixSetup.BlockSize;

	// Draw a grid
	// Check if row and column of player make sense - exit if not.
	if (playerScreenColumn < 0 || playerScreenRow < 0 || playerScreenColumn > _scroller.MatrixSetup.DisplayColumns || playerScreenRow > _scroller.MatrixSetup.DisplayRows) return;
	// Check 9 possible zones
	for (int x = 0; x <=1; x++)
	{
		for (int y = 0; y <= 1; y++)
		{
			if (playerScreenColumn == 0 && x == 0) continue;
			if (playerScreenColumn >= _scroller.MatrixSetup.DisplayColumns - 1 && x == 2) continue;
			if (playerScreenRow == 0 && y == 0) continue;
			if (playerScreenRow >= _scroller.MatrixSetup.DisplayRows - 1 && y == 2) continue;

			int alpha = _scroller.ColumnContainer[playerScreenColumn + _scroller.ColumnPosition + x][playerScreenRow + y].Color.a;

			obstEdgeT = (y + playerScreenRow) * (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing) + 150;
			obstEdgeB = obstEdgeT + _scroller.MatrixSetup.BlockSize;
			obstEdgeL = (_scroller.MatrixSetup.ScreenOffsX - _scroller.ScrollPosition) + ((x + playerScreenColumn) * (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing));
			obstEdgeR = obstEdgeL + _scroller.MatrixSetup.BlockSize;

			if (playerEdgeT > obstEdgeB || playerEdgeB < obstEdgeT || playerEdgeL > obstEdgeR || playerEdgeR < obstEdgeL) continue;


			if (alpha < 1) continue;
			GameStatus = GameOver;

		}
	}

}

void GameEngine::OnInitPlayer()
{
	_player.AnimationRate = 40;		// t=1000/framesPerSecond e.g. 1000/25 = 40
	_player.Name = "Mollmops";
	_player.TextureSourcePath = "Spritesheet_Alien_01.png";
	_player.HorizontalTiling = 6;
	_player.VerticalTiling = 4;
	_player.Speed = 2;
	_player.Score = 0;

	_player.MinPosition.x = _scroller.MatrixSetup.ScreenOffsX;
	_player.MaxPosition.x = _scroller.MatrixSetup.ScreenOffsX + ((_scroller.MatrixSetup.DisplayColumns - 1) * (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing));;
	_player.MinPosition.y = 150; // Bottom of Top-UI
	_player.MaxPosition.y = 150 + ((_scroller.MatrixSetup.DisplayRows - 1) * (_scroller.MatrixSetup.BlockSize + _scroller.MatrixSetup.BlockSpacing));

	_player.DisplayRect.w = _scroller.MatrixSetup.BlockSize;
	_player.DisplayRect.h = _scroller.MatrixSetup.BlockSize;
	_player.DisplayRect.x = _scroller.MatrixSetup.ScreenOffsX ;
	_player.DisplayRect.y = _player.MinPosition.y;

	_player.MotionHor = None;
	_player.MotionVer = None;
	_player.IsTop = false;
	_player.IsBottom = false;
	_player.isLeft = false;
	_player.IsRight = false;

}

void GameEngine::OnGameRestart()
{
	OnInitPlayer();
	_scroller.ColumnPosition = 0;
	_scroller.ScrollPosition = 0;

	_player.Score = 0;
	_player.Speed = 2;
	GameStatus = Running;
}


// Overrides from GameEvents

void GameEngine::OnExit()
{
	_appIsRunning = false;
}

void GameEngine::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	// global keys
	if (sym == SDLK_ESCAPE) _appIsRunning = false;
	if (sym == SDLK_SPACE)
	{
		if (GameStatus == Running) GameStatus = Paused;
		else if (GameStatus == Paused) GameStatus = Running;
	}
	if (sym == SDLK_F1) OnGameRestart();

}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (GameStatus == Running)
	{

	}
}

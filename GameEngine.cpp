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
	_isRunning = true;
	_playerSpeed = 2;
	_playerRotationSpeed = 2.0;
}

int GameEngine::OnExecute()
{

	if (OnInit() == false)
	{
		return -1;
	}

	SDL_Event AppEvent;

	while (_isRunning)
	{
		while (SDL_PollEvent(&AppEvent))
		{
			OnEvent(&AppEvent);
		}

		OnLoop();

		OnRender();
	}

	OnCleanup();

	return 0;
};

bool GameEngine::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 	return false;
	if (TTF_Init() == -1) return false;
	if ((_uiFont = TTF_OpenFont("SigmarOne-Regular.ttf", 32)) == nullptr) return false;

	_windowWidth = 1000;
	_windowHeight = 600;
	if ((_appWindow = SDL_CreateWindow("Zehnfinger Demo 2020", 50, 50, _windowWidth, _windowHeight, SDL_WINDOW_SHOWN)) == nullptr) return false;
	if ((_appRenderer = SDL_CreateRenderer(_appWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);
};

void GameEngine::OnLoop() {

	if (_playerUp) _playerPosition.y -= _playerSpeed;
	if (_playerDown) _playerPosition.y += _playerSpeed;
	if (_playerLeft) _playerPosition.x -= _playerSpeed;
	if (_playerRigth) _playerPosition.x += _playerSpeed;

	if (_playerRotRight)
	{
		_playerRotationAngle += _playerRotationSpeed;
		if (_playerRotationAngle > 360.0) _playerRotationAngle -= 360;
	}
	if (_playerRotLeft)
	{
		_playerRotationAngle -= _playerRotationSpeed;
		if (_playerRotationAngle < 0.0) _playerRotationAngle += 360.0;
	}
	
};

void GameEngine::OnRender()
{
	
	SDL_SetRenderDrawColor(_appRenderer, 80, 80, 80, 255);
	SDL_RenderClear(_appRenderer);

	SDL_Rect srcRect = { 0,0, 100,100 };
	SDL_Rect destRect = { 105,105,200,200 };

	SDL_SetRenderDrawColor(_appRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(_appRenderer, &srcRect);
	SDL_SetRenderDrawColor(_appRenderer, 100, 100, 100, 255);
	SDL_RenderFillRect(_appRenderer, &destRect);


	SDL_Color c ={ 0,0,0,255 };
	SDL_Surface* txtSurf = TTF_RenderText_Solid(_uiFont, "Jetzt kommt Text!", c);
	SDL_Texture* txtTex = SDL_CreateTextureFromSurface(_appRenderer, txtSurf);
	
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = txtSurf->w;
	srcRect.h = txtSurf->h;
		
	SDL_RenderCopy(_appRenderer, txtTex, &srcRect, &srcRect);
	SDL_free(txtSurf);

	SDL_Surface* surf = IMG_Load("ball.png");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(_appRenderer, surf);

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = surf->w;
	srcRect.h = surf->h;

	destRect.x = _playerPosition.x;
	destRect.y = _playerPosition.y;
	destRect.w = surf->w;
	destRect.h = surf->h;

	SDL_free(surf);

	SDL_Point* p = new SDL_Point();
	p->x = _playerPosition.x + 64;
	p->y = _playerPosition.y + 64;

	SDL_RenderCopyEx(_appRenderer, tex, &srcRect, &destRect, _playerRotationAngle, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(_appRenderer);
	SDL_free(txtTex);
	SDL_free(tex);
};

void GameEngine::OnCleanup()
{

	SDL_DestroyWindow(_appWindow);
	SDL_free(_appRenderer);
	SDL_free(_surf_Display);

	SDL_Quit();
};


// Overrides from ZF_DemoEvents

void GameEngine::OnExit()
{
	std::cout << "OnExit called." << std::endl;
	_isRunning = false;
}

void GameEngine::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	std::cout << "Key " << sym << " was pressed, using modifer " << mod << std::endl;
	if (sym == SDLK_ESCAPE) _isRunning = false;

	if (sym == SDLK_d && !_playerLeft) _playerRigth = true;
	if (sym == SDLK_a && !_playerRigth) _playerLeft = true;
	if (sym == SDLK_s && !_playerUp) _playerDown = true;
	if (sym == SDLK_w && !_playerDown) _playerUp = true;
	if (sym == SDLK_RIGHT && !_playerRotLeft) _playerRotRight = true;
	if (sym == SDLK_LEFT && !_playerRotRight) _playerRotLeft = true;
}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	std::cout << "Key " << sym << " was pressed, using modifer " << mod << std::endl;
	if (sym == SDLK_d) _playerRigth =false;
	if (sym == SDLK_a) _playerLeft = false;
	if (sym == SDLK_s) _playerDown = false;
	if (sym == SDLK_w) _playerUp = false;
	if (sym == SDLK_RIGHT && !_playerRotLeft) _playerRotRight = false;
	if (sym == SDLK_LEFT && !_playerRotRight) _playerRotLeft = false;
}

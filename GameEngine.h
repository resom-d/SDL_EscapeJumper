#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameEvents.h"

class GameEngine : public gen::GameEvents
{

public:
	GameEngine();

	bool OnInit();

	void OnEvent(SDL_Event* Event);

	int OnExecute();
	
	void OnLoop();

	void OnRender();

	void OnCleanup();

	// Overrides from ZF_DemoEvents

	void OnExit();
	
	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);

	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);
	

protected:
	SDL_Window* _appWindow;
	SDL_Renderer* _appRenderer;
	SDL_Surface* _surf_Display;
	TTF_Font* _uiFont;

	bool _isRunning;
	int _windowWidth;
	int _windowHeight;
	SDL_Point _playerPosition;
	int _playerSpeed;
	double _playerRotationAngle;
	double _playerRotationSpeed;

	bool _playerUp;
	bool _playerDown;
	bool _playerLeft;
	bool _playerRigth;
	bool _playerRotRight;
	bool _playerRotLeft;

private:

};
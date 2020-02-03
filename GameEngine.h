#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "GameEvents.h"
#include "GamePlayer.h"

typedef struct
{
	SDL_Rect WindowFrame;
	

} GameProperties;

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

	// Overrides from ZF_DemoEvents

	void OnExit();
	
	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);

	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);
	

protected:
	SDL_Window* _appWindow;
	SDL_Renderer* _gameRenderer;
	TTF_Font* _uiFont;
	GamePlayer player;
	bool _appIsRunning;
	int _windowWidth;
	int _windowHeight;
		
private:

};
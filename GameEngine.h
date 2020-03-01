#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <list>
#include "GameEvents.h"
#include "GameTypes.h"
#include "GameMap.h"
#include "HorizontalScoller.h"
#include "MainUI.h"
#include "GamePlayer.h"
#include"JumperPlayer.h"
#include "LevelEditor.h"
#include "TextScroller.h"
#include "SDL_Extras.h"

using namespace std;

class GameEngine : public GameEvents
{

public:
	GameEngine();

	Uint16 UI_Height;
	GameProperties Properties;
	bool _appIsRunning;
	Uint16 GlobalFrameRate;
	GameState GameStatus = GameState::Running;
	SDL_Window* AppWindow;
	SDL_Renderer* Renderer;
	MainUI MainUI;
	LevelEditor Editor;
	GameMap Map;
	JumperPlayer Player;
	HorizontalScroller Scroller;
	TextScroller MessageScroller;
	CharacterTextureMap CharMap;

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
	TTF_Font* _font;
	Mix_Music* tune;

protected:

};
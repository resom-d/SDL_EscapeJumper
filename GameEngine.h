#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <filesystem>
#include <list>
#include <boost/filesystem.hpp>
#include "GameEvents.h"
#include "GameTypes.h"
#include "GameMap.h"
#include "HorizontalScoller.h"
#include "UI_Main.h"
#include "UI_Game.h"
#include "GamePlayer.h"
#include"JumperPlayer.h"
#include "LevelEditor.h"
#include "TextScroller.h"
#include "SDL_Extras.h"

using namespace std;
using namespace boost::filesystem;

enum class GameState
{
	Idle,
	MainScreen,
	LevelEdit,
	PlayerEdit,
	Running,
	Paused,
	Highscore,
	GameOver,
	LevelComplete

};
typedef struct GameProperties
{
	SDL_Rect WindowFrame;
};

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
	UI_Game GameUI;
	LevelEditor Editor;
	GameMap Map;
	JumperPlayer Player;
	list<path> Levels;
	list<path> Avatars;
	list<path> TileTextureResources;
	CharacterTextureMap CharMap;
	unordered_map<string, SDL_Texture*> GameItems;
	SDL_Joystick* GamePad = NULL;

	bool OnInit();
	void OnEvent(SDL_Event* Event);
	int OnExecute();
	void OnLoop();
	void OnRender();
	void OnPostRender();
	void OnCleanup();
	void OnInitPlayer();
	void OnGameRestart();
	void OnExit();
	void OnKeyDown(SDL_Keycode sym, SDL_Keycode mod);
	void OnKeyUp(SDL_Keycode sym, SDL_Keycode mod);
	void GoMainscreen(void);
	void GoGame(void);
	void GoEditor(void);

	

	
private:
	TTF_Font* _font;
	Mix_Music* tune;
	Uint16 _level;
	int _timerCatch;

protected:

};
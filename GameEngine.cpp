#include "GameEngine.h"

GameEngine::GameEngine()
{
	GlobalFrameRate = 120;
	Properties.WindowFrame.x = 50;
	Properties.WindowFrame.y = 50;
	Properties.WindowFrame.w = 1600;
	Properties.WindowFrame.h = 900;

}

bool GameEngine::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 	return false;
	if (TTF_Init() == -1) return false;
	int r = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (SDL_NumJoysticks() > 0) GamePad = SDL_JoystickOpen(0);

	UI_Height = 200;
	Avatars = GetFilesInDirectory("Resources/sprites");
	Levels = GetFilesInDirectory("Resources/levels");
	TileTextureResources = GetFilesInDirectory("Resources/tilemaps");

	if ((AppWindow = SDL_CreateWindow(
		"Escape Jumper",
		20,
		50,
		0,
		0,
		SDL_WINDOW_HIDDEN)
		) == nullptr) return false;

	if ((Renderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	Map.OnInit(Renderer);
	Map.OnCleanUp();
	Map = GameMap::LoadMap(Renderer, Levels.begin()->string());
	if (Map.Setup.Cols < 1)
	{
		return false;
	}
	SDL_SetWindowSize(AppWindow, Map.Setup.DisplayRect.w, Map.Setup.DisplayRect.h + UI_Height);
	SDL_ShowWindow(AppWindow);

	Starter.DisplayRect = Map.Setup.DisplayRect;
	// CreateMap of GameItems
	list<path> items = GetFilesInDirectory("Resources/items");
	for (auto item = items.begin(); item != items.end(); item++)
	{
		SDL_Surface* sItem = IMG_Load(item->string().c_str());
		SDL_Texture* tItem = SDL_CreateTextureFromSurface(Renderer, sItem);
		GameItems.insert(pair<string, SDL_Texture*>(item->stem().string(), tItem));
	}

	// Create a texure map from a string 
	_font = TTF_OpenFont("Resources/fonts/arial.ttf", 24);
	CharMap = SDL_GetTexturesFromString(Renderer, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyzäöü,.;:*#-_|<>^°?=()!\"§$%&/()@€~", _font);

	Playfield.DisplayRect = Map.Setup.DisplayRect;
	Playfield.OnInit(Renderer, "Resources/bgnd/bgnd_001.png");

	Playfield_Too.DisplayRect = Map.Setup.DisplayRect;
	Playfield_Too.OnInit(Renderer, "Resources/bgnd/bgnd_002.png");
	Credits.DisplayRect = Map.Setup.DisplayRect;
	Credits.OnInit(Renderer);
	Credits.RepeatPause = 10000;

	Player.OnInit(Renderer, &Map);
	OnInitPlayer();

	MainUI.DisplayRect = { 0,0,Map.Setup.DisplayRect.w, UI_Height };
	MainUI.Texture = GameItems["MainUI"];
	MainUI.OnInit(Renderer);

	GameUI.DisplayRect = { 0,0,Map.Setup.DisplayRect.w, UI_Height };
	GameUI.OnInit(Renderer, &CharMap);
	GameUI.TexBackgnd = GameItems["GameUI"];

	Editor.DisplayRect = {
		0,
		0,
		Map.Setup.DisplayRect.w,
		Map.Setup.DisplayRect.w + UI_Height
	};
	Editor.OnInit(Renderer, CharMap);

	tune = Mix_LoadMUS("Resources/music/The impossible Mission.mp3");
	//Mix_PlayMusic(tune, -1);

	_appIsRunning = true;
	GameStatus = GameState::MainScreen;


	return true;
};

int GameEngine::OnExecute()
{
	if (OnInit() == false) return -1;

	SDL_Event AppEvent;
	int timeDiff;
	int timerFPS_n = SDL_GetTicks();
	int timerFPS_1n = SDL_GetTicks();

	while (_appIsRunning)
	{
		timerFPS_1n = SDL_GetTicks();

		//grab events	
		while (SDL_PollEvent(&AppEvent)) OnEvent(&AppEvent);

		// execute all logic
		OnLoop();
		// do all rendering
		OnRender();
		// cleanup after rendering. clear 
		OnPostRender();

		// make sure we are running at a constant frame rate
		timerFPS_n = SDL_GetTicks();
		timeDiff = timerFPS_n - timerFPS_1n;
		if (timeDiff < 1000 / GlobalFrameRate) SDL_Delay((1000 / GlobalFrameRate) - timeDiff);

	}

	// don't leave a messy place
	OnCleanup();

	return 0;
};

void GameEngine::OnEvent(SDL_Event* event)
{

	GameEvents::OnEvent(event);

	if (event->type == GAME_EVENT_TYPE)
	{
		Userdata ud;
		switch (event->user.code)
		{
		case (int)UI_ACTION::QUIT_GAME:
			_appIsRunning = false;
			break;

		case (int)UI_ACTION::GO_MAINSCREEN:
			GoMainscreen();
			break;

		case (int)UI_ACTION::GO_EDITOR:
			GoEditor();
			break;

		case (int)UI_ACTION::GO_GAME:
			GoGame();
			break;

		case (int)UI_ACTION::GO_EDITOR_TESTMODE:
			ud = *(Userdata*)event->user.data2;
			Map.OnCleanUp();
			Map = GameMap::LoadMap(Renderer, "Resources/levels/" + *ud.NewMapName + ".txt");
			Map.ViewMode = EngineViewMode::Game;
			Map.ScrollSpeed = 4;
			Map.ScrollXInDelay = 0;
			Map.ScrollXOutDelay = 0;
			Map.ResetScroller();
			GameStatus = GameState::LevelEditTest;
			OnInitPlayer();
			break;

		case (int)UI_ACTION::SET_PLAYER_AVATAR:
			Player.Texture = ((Userdata*)event->user.data2)->Texture;
			GoGame();
			break;
		}
	}

	if (GameStatus == GameState::MainScreen) MainUI.OnEvent(event);
	if (GameStatus == GameState::Running || GameStatus == GameState::LevelEditTest)
	{
		Player.OnEvent(event);
		GameUI.OnEvent(event);
	}
	if (GameStatus == GameState::GameOver)
	{
		GameUI.OnEvent(event);
	}
	if (GameStatus == GameState::LevelEdit)
	{
		Editor.OnEvent(event);
	}
};

void GameEngine::OnLoop()
{
	Playfield.RepeatPauseMode = false;
	if (GameStatus == GameState::MainScreen)
	{
		Map.OnLoop();
		if (_scrollCntBgnd++ % 4 == 0) Playfield.OnLoop();
		//if (_scrollCntBgnd % 2 == 0) Playfield_Too.OnLoop();
		for (auto i = 0; i < 4; i++) Credits.OnLoop();

	}

	if (GameStatus == GameState::Running || GameStatus == GameState::LevelEditTest)
	{
		GameUI.OnLoop();
		int x = 0;
		for (int l = 0; l < Map.ScrollSpeed; l++)
		{
			if (x++ % 2 == 0) Playfield.OnLoop();
			//if (x % 2 == 0) Playfield_slow.OnLoop();
			Map.OnLoop();

			for (int pl = 0; pl < Player.Speed; pl++)
			{
				Player.OnLoop();
				if (Player.GameOver) break;
			}
			if (Player.GameOver) break;
		}

		if (Player.GameOver && GameStatus != GameState::LevelEditTest) GameStatus = GameState::GameOver;
		if (Player.GameOver && GameStatus == GameState::LevelEditTest) GameStatus = GameState::LevelEdit;

		if (Map.LevelDone)
		{
			if (GameStatus != GameState::LevelEditTest)
			{
				_level++;
				if (_level > Levels.size() - 1) _level = 0;
				Map.OnCleanUp();
				Map = GameMap::LoadMap(Renderer, next(Levels.begin(), _level)->string());
				Map.ViewMode = EngineViewMode::Game;
				Map.ScrollSpeed = 4;
				Map.ScrollXInDelay = 0;
				Map.ScrollXOutDelay = 0;
				GameStatus = GameState::LevelComplete;
			}
			Map.ResetScroller();
			Map.ResetInView();
			Map.LevelDone = false;

			_timerCatch = SDL_GetTicks();
		}
	}

	if (GameStatus == GameState::LevelComplete && SDL_GetTicks() - _timerCatch > 2000) GameStatus = GameState::Running;

	if (GameStatus == GameState::GameOver)
	{
		GameUI.OnLoop();
	}

	if (GameStatus == GameState::LevelEdit)
	{
		Editor.OnLoop();
	}

	if (GameStatus == GameState::Starting && Starter.Done) GameStatus = GameState::Running;
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);
	SDL_Color c = { 0,0,0,255 };
	SDL_RenderSetDrawColor(Renderer, c);
	SDL_RenderSetClipRect(Renderer, nullptr);
	SDL_RenderClear(Renderer);
	SDL_RenderFillRect(Renderer, nullptr);

	if (GameStatus == GameState::Starting)
	{
		Starter.OnRender();
	}

	if (GameStatus == GameState::MainScreen)
	{
		MainUI.OnRender("Zehnfinger", Player.Score, GameStatus == GameState::Running);
		Playfield.OnRender();
		Map.OnRender();
		//Playfield_Too.OnRender();
		Credits.OnRender();
	}

	if (GameStatus == GameState::Running
		|| GameStatus == GameState::Paused
		|| GameStatus == GameState::LevelComplete
		|| GameStatus == GameState::GameOver
		|| GameStatus == GameState::LevelEditTest)
	{
		// Render UI
		GameUI.OnRender(GameStatus == GameState::GameOver, &Player);

		Playfield.OnRender();
		//Playfield_slow.OnRender();
		// Render Map
		Map.OnRender();
		// Render player(s)
		Player.OnRender();

		if (GameStatus == GameState::GameOver)
		{
			SDL_RenderSetClipRect(Renderer, &Map.Setup.DisplayRect);

			SDL_Rect dRect = {
					Map.Setup.DisplayRect.x + 100,
					Map.Setup.DisplayRect.y + 200,
					1200,
					400
			};

			SDL_Rect sRect = {
					0,
					0,
					1200,
					400
			};

			SDL_RenderCopy(Renderer,
				GameItems["GameOver"],
				&sRect,
				&dRect
			);
		}

		if (GameStatus == GameState::LevelComplete)
		{
			SDL_RenderSetClipRect(Renderer, &Map.Setup.DisplayRect);

			SDL_Rect dRect = {
					Map.Setup.DisplayRect.x + 100,
					Map.Setup.DisplayRect.y + 200,
					1200,
					400
			};

			SDL_Rect sRect = {
					0,
					0,
					1200,
					400
			};

			SDL_RenderCopy(Renderer,
				GameItems["LevelComplete"],
				&sRect,
				&dRect
			);
		}
	}

	if (GameStatus == GameState::LevelEdit)
	{
		Editor.OnRender();
	}

	// Hey, Renderer! Do it! Do it !
	SDL_RenderPresent(Renderer);

}

void GameEngine::OnPostRender()
{
	MainUI.OnPostRender();
}

void GameEngine::OnCleanup()
{
	Playfield.OnCleanup();
	SDL_free(_font);

	for (auto item = GameItems.begin(); item != GameItems.end(); item++)
	{
		SDL_DestroyTexture(item->second);
	}

	if (Map.Setup.Cols > 0) Map.OnCleanUp();

	MainUI.OnCleanup();
	GameUI.OnCleanup();
	Editor.OnCleanUp();
	Player.OnCleanup();
	Credits.OnCleanup();

	Mix_CloseAudio();
	Mix_FreeMusic(tune);

	SDL_DestroyWindow(AppWindow);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

}

void GameEngine::OnInitPlayer()
{
	Player.AnimationRate = 40;		// t=1000/framesPerSecond e.g. 1000/25 = 40
	Player.Name = "Mollmops";
	Player.TextureSourcePath = "Resources/sprites/Block_001.png";
	Player.HorizontalTiling = 6;
	Player.VerticalTiling = 4;

	Player.GameOver = false;
	Player.Score = 0;
	Player.Jumps = 0;

	Player.Speed = 3;
	Player.MotionHor = MotionState::None;
	Player.MotionVer = MotionState::Plus;

	Player.MinPosition.x = Map.Setup.ScreenOffsX;
	Player.MaxPosition.x = Map.Setup.ScreenOffsX + ((Map.Setup.DisplayCols - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));
	Player.MinPosition.y = Map.Setup.DisplayRect.y;
	Player.MaxPosition.y = Map.Setup.DisplayRect.y + ((Map.Setup.DisplayRows - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));

	Player.DisplayRect.w = Map.Setup.BlockSize;
	Player.DisplayRect.h = Map.Setup.BlockSize;
	Player.DisplayRect.x = Map.Setup.ScreenOffsX + (Map.Setup.DisplayRect.w / 2);
	Player.DisplayRect.y = Map.Setup.DisplayRect.y + (Map.Setup.DisplayRect.h / 2);

}

void GameEngine::OnGameRestart()
{
	Map.OnCleanUp();
	Map = GameMap::LoadMap(Renderer, next(Levels.begin(), _level)->string());
	Map.ViewMode = EngineViewMode::Game;
	Map.ScrollSpeed = 4;
	Map.ScrollXInDelay = 0;
	Map.ScrollXOutDelay = 0;
	Map.ResetScroller();
	Map.ResetInView();

	OnInitPlayer();

	Starter.OnInit(Renderer);
	Starter.Done = false;
	GameStatus = GameState::Starting;
}

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
		if (GameStatus == GameState::Running) GameStatus = GameState::Paused;
		else if (GameStatus == GameState::Paused) GameStatus = GameState::Running;
	}
	if (sym == SDLK_F1) GoMainscreen();
	if (sym == SDLK_F3)
	{
		if (GameStatus == GameState::LevelEditTest)
		{
			Map.ViewMode = EngineViewMode::Game;
			Map.ScrollSpeed = 4;
			Map.ScrollXInDelay = 0;
			Map.ScrollXOutDelay = 0;
			Map.ResetScroller();
			OnInitPlayer();
		}
		else 
		{
			GoGame();
		}
	}

	if (sym == SDLK_F5) GoEditor();
}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (GameStatus == GameState::Running)
	{

	}
}

void GameEngine::GoMainscreen(void)
{
	Levels = GetFilesInDirectory("Resources/levels");
	Map = GameMap::LoadMap(Renderer, Levels.begin()->string());
	Map.ScrollSpeed = 1;
	Map.ResetScroller();
	Map.ViewMode = EngineViewMode::Editor;
	GameStatus = GameState::MainScreen;
}

void GameEngine::GoGame(void)
{
	_level = 0;
	OnGameRestart();
}

void GameEngine::GoEditor(void)
{
	Map.ViewMode = EngineViewMode::Editor;
	GameStatus = GameState::LevelEdit;
}



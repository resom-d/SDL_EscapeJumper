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
	if (SDL_NumJoysticks() > 0) GamePad = SDL_JoystickOpen(0);

	UI_Height = 200;

	// Configure Map
	Map.Setup.Cols = 200;
	Map.Setup.Rows = 20;
	Map.Setup.DisplayCols = 40;
	Map.Setup.DisplayRows = 20;
	Map.Setup.BlockSize = 35;
	Map.Setup.BlockSpacing = 0;
	Map.Setup.ScreenOffsX = 0;
	Map.Setup.DisplayRect =
	{
		Map.Setup.ScreenOffsX,
		 UI_Height,
		(Map.Setup.BlockSize + Map.Setup.BlockSpacing) * Map.Setup.DisplayCols + 1,
		(Map.Setup.BlockSize + Map.Setup.BlockSpacing) * Map.Setup.DisplayRows + 1
	};
	Map.Setup.Background = { 0,0,0, 255 };

	if ((AppWindow = SDL_CreateWindow(
		"EscapeJumper - Ein Zehnfinger Spiel",
		Properties.WindowFrame.x,
		Properties.WindowFrame.y,
		Map.Setup.DisplayRect.w,
		Map.Setup.DisplayRect.h + UI_Height,
		SDL_WINDOW_SHOWN)
		) == nullptr) return false;

	if ((Renderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	Map.OnInit(Renderer);
	TileMapTextureResource res;
	res.Cols = 3;
	res.Rows = 3;
	res.MaxIndex = 9;
	res.Tilesize = Size2D(35, 35);
	res.Path = "Resources/tilemaps/tilemap_001.png";
	SDL_Surface* surf = IMG_Load(res.Path.c_str());
	res.Texture = SDL_CreateTextureFromSurface(Renderer, surf);
	SDL_FreeSurface(surf);

	Map.TextureResources.push_back(res);


	// Create a texure map from a string 
	_font = TTF_OpenFont("Resources/fonts/NovaMono-Regular.ttf", 36);
	CharMap = SDL_GetTexturesFromString(Renderer, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyzäöü,.;:*#-_|<>^°?=()!\"§$%&/()@€~", _font);

	Player.OnInit(Renderer, &Map);
	OnInitPlayer();

	MainUI.DisplayRect = { 0,0,Map.Setup.DisplayRect.w, UI_Height };
	MainUI.OnInit(Renderer, CharMap);

	GameUI.DisplayRect = { 0,0,Map.Setup.DisplayRect.w, UI_Height };
	GameUI.OnInit(Renderer, &CharMap);

	MessageScroller.OnInit(Renderer,
		"ESCAPE JUMPER - Ein Spiel für Kinder von 6 bis 100 Jahre. Drücke F1 um hier her zurück zu kommen, F3 um mit dem Spiel zu beginnen oder F5 um zum Level-Editor zu gelangen. Viel Spass damit.                           Let Peace Grow.       ",
		_font,
		{ 0, 61, 115, 255 },
		1
	);
	MessageScroller.DestRect.x = 320;
	MessageScroller.DestRect.y = 320;
	MessageScroller.DestRect.w = 800;

	Editor.DisplayRect = {
		0,
		0,
		Map.Setup.DisplayRect.w,
		Map.Setup.DisplayRect.w + UI_Height
	};
	Editor.OnInit(AppWindow, Renderer, &Map, CharMap);

	/*int r = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	tune = Mix_LoadMUS("Resources/music/The impossible Mission.mp3");
	Mix_PlayMusic(tune, -1);*/

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
	if (event->type == GAME_EVENT_TYPE)
	{
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
		}
	}
	GameEvents::OnEvent(event);
	if (GameStatus == GameState::MainScreen) MainUI.OnEvent(event);
	if (GameStatus != GameState::LevelEdit)	MainUI.OnEvent(event);
	if (GameStatus == GameState::Running)
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
	if (GameStatus == GameState::MainScreen)
	{
		Map.OnLoop();

		MessageScroller.OnLoop();

	}

	if (GameStatus == GameState::Running)
	{
		GameUI.OnLoop();
		for (int l = 0; l < Map.ScrollSpeed; l++)
		{
			Map.OnLoop();

			for (int pl = 0; pl < Player.Speed; pl++)
			{
				Player.OnLoop();
			}
		}
		if (Player.GameOver) GameStatus = GameState::GameOver;

		if (Map.LevelDone)
		{
			Map.LevelDone = false;
			GameStatus = GameState::GameOver;
		}
	}
	if (GameStatus == GameState::GameOver)
	{
		GameUI.OnLoop();
	}

	if (GameStatus == GameState::LevelEdit)
	{
		Editor.OnLoop();
	}
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer, 255, 208, 99, 255);
	SDL_RenderClear(Renderer);

	if (GameStatus == GameState::MainScreen)
	{
		MainUI.OnRender("Zehnfinger", Player.Score, GameStatus == GameState::Running);
		Map.OnRender();
		MessageScroller.OnRender();
	}

	if (GameStatus == GameState::Running || GameStatus == GameState::Paused || GameStatus == GameState::GameOver)
	{
		// Render UI
		GameUI.OnRender(Player.Name, to_string(Player.Score), GameStatus == GameState::GameOver);
		// Render background

		// Render Scoller
		Map.OnRender();
		// Render player(s)
		Player.OnRender();
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
	SDL_free(_font);
	Map.OnCleanUp();
	MainUI.OnCleanup();
	GameUI.OnCleanup();
	Editor.OnCleanUp();
	Player.OnCleanup();
	MessageScroller.OnCleanUp();

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
	Player.TextureSourcePath = "Resources/sprites/Spritesheet_Alien_01.png";
	Player.HorizontalTiling = 6;
	Player.VerticalTiling = 4;
	Player.Speed = 8;
	Player.Score = 0;

	Player.MinPosition.x = Map.Setup.ScreenOffsX;
	Player.MaxPosition.x = Map.Setup.ScreenOffsX + ((Map.Setup.DisplayCols - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));
	Player.MinPosition.y = Map.Setup.DisplayRect.y;
	Player.MaxPosition.y = Map.Setup.DisplayRect.y + ((Map.Setup.DisplayRows - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));

	Player.DisplayRect.w = Map.Setup.BlockSize;
	Player.DisplayRect.h = Map.Setup.BlockSize;
	Player.DisplayRect.x = Map.Setup.ScreenOffsX + (Map.Setup.DisplayRect.w / 2);
	Player.DisplayRect.y = Map.Setup.DisplayRect.y + (Map.Setup.DisplayRect.h / 2);

	Player.MotionHor = MotionState::None;
	Player.MotionVer = MotionState::None;
}

void GameEngine::OnGameRestart()
{
	OnInitPlayer();
	Player.GameOver = false;

	Map.ScrollSpeed = 4;
	Map.ScrollXInDelay = 0;
	Map.ScrollXOutDelay = 0;
	Map.Reset();

	Player.Score = 0;
	Player.Speed = 3;
	Player.MotionVer = MotionState::Plus;

	Map.ViewMode = EngineViewMode::Game;

	GameStatus = GameState::Running;
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
		if (GameStatus == GameState::Running) GameStatus = GameState::Paused;
		else if (GameStatus == GameState::Paused) GameStatus = GameState::Running;
	}
	if (sym == SDLK_F1) GoMainscreen();
	if (sym == SDLK_F3) GoGame();
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
	Map.ScrollSpeed = 1;
	Map.Reset();
	Map.ViewMode = EngineViewMode::Editor;
	GameStatus = GameState::MainScreen;
}

void GameEngine::GoGame(void)
{
	Map.ViewMode = EngineViewMode::Game;
	OnGameRestart();
}

void GameEngine::GoEditor(void)
{
	Map.ViewMode = EngineViewMode::Editor;
	GameStatus = GameState::LevelEdit;
}

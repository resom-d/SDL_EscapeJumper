#include "GameEngine.h"

int inc = 1;

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
	Map.Setup.Rows = 40;
	Map.Setup.DisplayCols = 40;
	Map.Setup.BlockSpacing = 1;
	Map.Setup.BlockSize = 35;
	Map.Setup.DisplayRows = 20;
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
	res.Tilesize = Size2D(35,35);
	res.Path = "Resources/tilemaps/tilemap_001.png";
	SDL_Surface* surf = IMG_Load(res.Path.c_str());
	res.Texture = SDL_CreateTextureFromSurface(Renderer, surf);
	SDL_FreeSurface(surf);

	Map.TextureResources.push_back(res);


	// Create a texure map from a string 
	_font = TTF_OpenFont("Resources/fonts/NovaMono-Regular.ttf", 36);
	CharMap = SDL_GetTexturesFromString(Renderer, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyzäöü,.;:*#-_|<>^°?=()!\"§$%&/()@€~", _font);

	Player.OnInit(Renderer);
	OnInitPlayer();

	MainUI.DisplayRect = { 0,0,Map.Setup.DisplayRect.w, UI_Height };
	MainUI.OnInit(Renderer, CharMap);

	Scroller.OnInit(Renderer, &Map);
	Scroller.ScrollSpeed = 1;

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
			GameStatus = GameState::MainScreen;
			break;

		case (int)UI_ACTION::GO_EDITOR:
			GameStatus = GameState::LevelEdit;
			break;

		case (int)UI_ACTION::GO_GAME:
			OnGameRestart();
			break;
		}
	}
	GameEvents::OnEvent(event);
	if (GameStatus == GameState::MainScreen) MainUI.OnEvent(event);
	if (GameStatus != GameState::LevelEdit)	MainUI.OnEvent(event);
	if (GameStatus == GameState::Running)
	{
		Player.OnEvent(event);
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
		MessageScroller.OnLoop();

		MessageScroller.DestRect.y += inc;
		if (MessageScroller.DestRect.y > 340)
		{
			MessageScroller.DestRect.y = 340;
			inc *= -1;
		}
		if (MessageScroller.DestRect.y < 300)
		{
			MessageScroller.DestRect.y = 300;
			inc *= -1;
		}
	}

	if (GameStatus == GameState::Running)
	{
		Player.OnLoop();
		Scroller.OnLoop();
		OnCollisionCheck();

		if (Scroller.LevelDone)
		{
			Scroller.LevelDone = false;
			GameStatus = GameState::GameOver;
		}
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
		Map.OnRender({ 0,0 }, { 0,0 });
		MessageScroller.OnRender();
	}

	if (GameStatus == GameState::Running || GameStatus == GameState::Paused || GameStatus == GameState::GameOver)
	{
		// Render UI
		MainUI.OnRender(Player.Name, Player.Score, GameStatus == GameState::GameOver);
		// Render background

		// Render Scoller
		Scroller.OnRender();
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
	Editor.OnCleanUp();
	Scroller.OnCleanUp();
	Player.OnCleanup();
	MessageScroller.OnCleanUp();

	Mix_CloseAudio();
	Mix_FreeMusic(tune);

	SDL_DestroyWindow(AppWindow);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

}

void GameEngine::OnCollisionCheck()
{
	TilemapTile ta[200][20];

	/*for (int column = 0; column < Map.Setup.Cols; column++)
	{
		for (int row = 0; row < Map.Setup.Rows; row++)
		{
			TilemapTile t = Map.GetTileAt(column, row);
			ta[column][row] = t;
		}
	}*/

	int playerScreenColumn, playerScreenRow;
	int playerEdgeT, playerEdgeL, playerEdgeR, playerEdgeB;
	int obstEdgeT, obstEdgeL, obstEdgeR, obstEdgeB;

	playerScreenColumn = (Player.DisplayRect.x - Map.Setup.ScreenOffsX) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	playerScreenRow = (Player.DisplayRect.y - 200) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);

	playerEdgeT = Player.DisplayRect.y;
	playerEdgeB = Player.DisplayRect.y + Map.Setup.BlockSize;
	playerEdgeL = Player.DisplayRect.x;
	playerEdgeR = Player.DisplayRect.x + Map.Setup.BlockSize;

	// Check if row and column of player make sense - exit if not.
	if (playerScreenColumn < 0 || playerScreenRow < 0 || playerScreenColumn > Map.Setup.DisplayCols || playerScreenRow > Map.Setup.DisplayRows) return;
	// Check 9 possible zones
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			int obstX = playerScreenColumn + x + Scroller.BlockPosition.x +1;
			int obstY = playerScreenRow + y + Scroller.BlockPosition.y;
			if (obstX < 0 || obstY < 0 || obstX > Map.Setup.Cols - 1 || obstY > Map.Setup.Rows - 1) continue;

			TilemapTile tile = Map.TileMap[obstX][obstY];
			if (!tile.Visible) continue;

			obstEdgeT = Map.Setup.DisplayRect.y + ((playerScreenRow + y) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));
			obstEdgeL = Map.Setup.DisplayRect.x + ((playerScreenColumn + x + 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing)) - Scroller.ScrollPosition.x;

			SDL_Rect rObst =
			{
				obstEdgeL,
				obstEdgeT,
				Map.Setup.BlockSize,
				Map.Setup.BlockSize
			};

			SDL_Rect result;

			if (SDL_IntersectRect(&Player.DisplayRect, &rObst, &result))
			{
				GameStatus = GameState::GameOver;

			}


		}
	}

}

void GameEngine::OnInitPlayer()
{
	Player.AnimationRate = 40;		// t=1000/framesPerSecond e.g. 1000/25 = 40
	Player.Name = "Mollmops";
	Player.TextureSourcePath = "Resources/sprites/Spritesheet_Alien_01.png";
	Player.HorizontalTiling = 6;
	Player.VerticalTiling = 4;
	Player.Speed = 4;
	Player.Score = 0;

	Player.MinPosition.x = Map.Setup.ScreenOffsX + 1;
	Player.MaxPosition.x = Map.Setup.ScreenOffsX + ((Map.Setup.DisplayCols - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));
	Player.MinPosition.y = Map.Setup.DisplayRect.y + 1; // Bottom of Top-UI
	Player.MaxPosition.y = Map.Setup.DisplayRect.y + ((Map.Setup.DisplayRows - 1) * (Map.Setup.BlockSize + Map.Setup.BlockSpacing));

	Player.DisplayRect.w = Map.Setup.BlockSize;
	Player.DisplayRect.h = Map.Setup.BlockSize;
	Player.DisplayRect.x = Player.MinPosition.x;
	Player.DisplayRect.y = Map.Setup.DisplayRect.y + (Map.Setup.DisplayRect.h / 2);

	Player.MotionHor = MotionState::None;
	Player.MotionVer = MotionState::None;
	Player.IsTop = false;
	Player.IsBottom = false;
	Player.isLeft = false;
	Player.IsRight = false;

}

void GameEngine::OnGameRestart()
{
	OnInitPlayer();

	Scroller.ScrollPosition.x = 0;
	Scroller.BlockPosition = { 0,0 };

	Player.Score = 0;
	Player.Speed = 2;
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
	if (sym == SDLK_F1) GameStatus = GameState::MainScreen;
	if (sym == SDLK_F3) OnGameRestart();
	if (sym == SDLK_F5) GameStatus = GameState::LevelEdit;

}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (GameStatus == GameState::Running)
	{

	}
}

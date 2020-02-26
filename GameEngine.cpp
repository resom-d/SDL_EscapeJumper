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
		if (timeDiff < 1000 / GlobalFrameRate)
		{
			cout << "Idle-Time: " << to_string((1000 / GlobalFrameRate) - timeDiff) << endl;
			SDL_Delay((1000 / GlobalFrameRate) - timeDiff);
		}
		else
		{
			cout << "Framerate not constant." << endl;
		}
	}

	// don't leave a messy place
	OnCleanup();

	return 0;
};

bool GameEngine::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 	return false;
	if (TTF_Init() == -1) return false;
	if (SDL_NumJoysticks() > 0) GamePad = SDL_JoystickOpen(0);

	_font = TTF_OpenFont("Resources/fonts/NovaMono-Regular.ttf", 36);


	UI_Height = 200;

	// Configure Map
	MapSetup.Cols = 200;
	MapSetup.Rows = 20;
	MapSetup.DisplayColumns = 40;
	MapSetup.BlockSpacing = 1;
	MapSetup.BlockSize = 35;
	MapSetup.DisplayRows = 20;
	MapSetup.ScreenOffsX = 0;
	MapSetup.DisplayRect =
	{
		MapSetup.ScreenOffsX,
		 UI_Height,
		(MapSetup.BlockSize + MapSetup.BlockSpacing) * MapSetup.DisplayColumns + 1,
		(MapSetup.BlockSize + MapSetup.BlockSpacing) * MapSetup.DisplayRows + 1
	};
	MapSetup.Background = { 0,0,0, 255 };


	if ((AppWindow = SDL_CreateWindow(
		"EscapeJumper - Ein Zehnfinger Spiel",
		Properties.WindowFrame.x,
		Properties.WindowFrame.y,
		MapSetup.DisplayRect.w,
		MapSetup.DisplayRect.h + UI_Height,
		SDL_WINDOW_SHOWN)
		) == nullptr) return false;

	if ((Renderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	// Create a texure map from a string 
	chars = SDL_AdditionalFunctions::GetTexturesFromString(Renderer, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,.;:*#-?=()!\"§$%&/()@€~", _font);
	
	// Allocate memory for a 2D matrix of tiles
	Map = (MatrixRectItem**)malloc(MapSetup.Cols * sizeof(MatrixRectItem*));
	for (int cols = 0; cols < MapSetup.Cols; cols++)
	{
		Map[cols] = (MatrixRectItem*)malloc(MapSetup.Rows * sizeof(MatrixRectItem*));
	}

	// fill the matrix with color 0 to make items invisible
	for (int x = 0; x < MapSetup.Cols; x++)
	{
		for (int y = 0; y < MapSetup.Rows; y++)
		{
			Map[x][y].FillColor = 0;
			Map[x][y].BorderColor = 0;
		}
	}

	Player.OnInit(Renderer);
	OnInitPlayer();

	MainUI.DisplayRect = { 0,0,MapSetup.DisplayRect.w, UI_Height };
	MainUI.OnInit(Renderer);

	Scroller.OnInit(Renderer, &MapSetup, Map);
	Scroller.ScrollSpeed = 4;
	Scroller.ScrollPosition = 20;


	MessageScroller.OnInit(Renderer,
		"ESCAPE JUMPER - Ein Spiel für Kinder von 6 bis 100 Jahre. Drücke F1 um hier her zurück zu kommen, F3 um mit dem Spiel zu beginnen oder F5 um zum Level-Editor zu gelangen. Viel Spass damit.                           Let Peace Grow.       ",
		_font,
		{ 0, 61, 115, 255 },
		1
	);
	MessageScroller.DestRect.x = 320;
	MessageScroller.DestRect.y =  (MessageScroller.GetSurfaceSize().y);
	MessageScroller.DestRect.w = 800;

	Editor.DisplayRect = {
		0,
		0,
		MapSetup.DisplayRect.w,
		MapSetup.DisplayRect.w + UI_Height
	};
	Editor.OnInit(AppWindow, Renderer, Map, &MapSetup);


	_appIsRunning = true;
	GameStatus = GameState_MainScreen;

	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);

	if (GameStatus == GameState_Running) Player.OnEvent(Event);
	if (GameStatus == GameState_LevelEdit)
	{
		Editor.OnEvent(Event);
	}
};

void GameEngine::OnLoop()
{
	if (GameStatus == GameState_MainScreen)
	{
		MessageScroller.OnLoop();

		MessageScroller.DestRect.y += inc;
		if (MessageScroller.DestRect.y > 40)
		{
			MessageScroller.DestRect.y = 40;
			inc *= -1;
		}
		if (MessageScroller.DestRect.y < 0)
		{
			MessageScroller.DestRect.y = 0;
			inc *= -1; 
		}
	}

	if (GameStatus == GameState_Running)
	{
		Player.OnLoop();
		Scroller.OnLoop();
		OnCollisionCheck();

		if (Scroller.LevelDone)
		{
			Scroller.LevelDone = false;
			GameStatus = GameState_GameOver;
		}
	}

	if (GameStatus == GameState_LevelEdit)
	{
		Editor.OnLoop();
	}
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Renderer, 255, 208, 99, 255);
	SDL_RenderClear(Renderer);

	if (GameStatus == GameState_MainScreen)
	{
		MessageScroller.OnRender();
	}

	if (GameStatus == GameState_Running || GameStatus == GameState_Paused || GameStatus == GameState_GameOver)
	{
		// Render UI
		MainUI.OnRender(Player.Name, Player.Score, GameStatus == GameState_GameOver);
		// Render background

		// Render Scoller
		Scroller.OnRender();
		// Render player(s)
		Player.OnRender();
	}

	if (GameStatus == GameState_LevelEdit)
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
	Editor.OnCleanUp();
	Scroller.OnCleanUp();
	Player.OnCleanup();
	MainUI.OnCleanup();
	MessageScroller.OnCleanUp();
	SDL_DestroyWindow(AppWindow);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();

}

void GameEngine::OnCollisionCheck()
{
	int playerScreenColumn, playerScreenRow;
	int playerEdgeT, playerEdgeL, playerEdgeR, playerEdgeB;
	int obstEdgeT, obstEdgeL, obstEdgeR, obstEdgeB;

	playerScreenColumn = (Player.DisplayRect.x - MapSetup.ScreenOffsX) / (MapSetup.BlockSize + MapSetup.BlockSpacing);
	playerScreenRow = (Player.DisplayRect.y - 200) / (MapSetup.BlockSize + MapSetup.BlockSpacing);

	playerEdgeT = Player.DisplayRect.y;
	playerEdgeB = Player.DisplayRect.y + MapSetup.BlockSize;
	playerEdgeL = Player.DisplayRect.x;
	playerEdgeR = Player.DisplayRect.x + MapSetup.BlockSize;

	// Check if row and column of player make sense - exit if not.
	if (playerScreenColumn < 0 || playerScreenRow < 0 || playerScreenColumn > MapSetup.DisplayColumns || playerScreenRow > MapSetup.DisplayRows) return;
	// Check 9 possible zones
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			int obstX = playerScreenColumn + x + Scroller.ColumnPosition + 1;
			int obstY = playerScreenRow + y;

			if (obstX < 0 || obstY < 0 || obstX > MapSetup.Cols || obstY > MapSetup.Rows) continue;

			if (Map[obstX][obstY].FillColor == 0) continue;

			obstEdgeT = MapSetup.DisplayRect.y + ((playerScreenRow + y) * (MapSetup.BlockSize + MapSetup.BlockSpacing));
			obstEdgeL = MapSetup.DisplayRect.x + ((playerScreenColumn + x + 1) * (MapSetup.BlockSize + MapSetup.BlockSpacing)) - Scroller.ScrollPosition;

			SDL_Rect rObst =
			{
				obstEdgeL,
				obstEdgeT,
				MapSetup.BlockSize,
				MapSetup.BlockSize
			};

			SDL_Rect result;

			if (SDL_IntersectRect(&Player.DisplayRect, &rObst, &result))
			{
				GameStatus = GameState_GameOver;

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

	Player.MinPosition.x = MapSetup.ScreenOffsX + 1;
	Player.MaxPosition.x = MapSetup.ScreenOffsX + ((MapSetup.DisplayColumns - 1) * (MapSetup.BlockSize + MapSetup.BlockSpacing));
	Player.MinPosition.y = MapSetup.DisplayRect.y + 1; // Bottom of Top-UI
	Player.MaxPosition.y = MapSetup.DisplayRect.y + ((MapSetup.DisplayRows - 1) * (MapSetup.BlockSize + MapSetup.BlockSpacing));

	Player.DisplayRect.w = MapSetup.BlockSize;
	Player.DisplayRect.h = MapSetup.BlockSize;
	Player.DisplayRect.x = Player.MinPosition.x;
	Player.DisplayRect.y = MapSetup.DisplayRect.y + (MapSetup.DisplayRect.h / 2);

	Player.MotionHor = None;
	Player.MotionVer = None;
	Player.IsTop = false;
	Player.IsBottom = false;
	Player.isLeft = false;
	Player.IsRight = false;

}

void GameEngine::OnGameRestart()
{
	OnInitPlayer();

	Scroller.ScrollPosition = 0;
	Scroller.ColumnPosition = 0;

	Player.Score = 0;
	Player.Speed = 2;
	GameStatus = GameState_Running;
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
		if (GameStatus == GameState_Running) GameStatus = GameState_Paused;
		else if (GameStatus == GameState_Paused) GameStatus = GameState_Running;
	}
	if (sym == SDLK_F1) GameStatus = GameState_MainScreen;
	if (sym == SDLK_F3) OnGameRestart();
	if (sym == SDLK_F5) GameStatus = GameState_LevelEdit;

}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (GameStatus == GameState_Running)
	{

	}
}

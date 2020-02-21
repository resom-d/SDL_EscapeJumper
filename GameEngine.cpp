#include "GameEngine.h"


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
		// cleanup after rendering. clear 
		OnPostRender();

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

	if ((AppWindow = SDL_CreateWindow(
		"EscapeJumper - ein Zehnfinger Spiel",
		Properties.WindowFrame.x,
		Properties.WindowFrame.y,
		Properties.WindowFrame.w,
		Properties.WindowFrame.h,
		SDL_WINDOW_SHOWN)
		) == nullptr) return false;

	if ((Renderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr) return false;

	if (SDL_NumJoysticks() > 0) GamePad = SDL_JoystickOpen(0);


	// Configure Map
	MapSetup.Cols = 100;
	MapSetup.Rows = 20;
	MapSetup.BlockSize = 35;
	MapSetup.BlockSpacing = 1;
	MapSetup.DisplayColumns = 40;
	MapSetup.DisplayRows = 20;
	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(Renderer, &screenWidth, &screenHeight);
	MapSetup.ScreenOffsX = (screenWidth - ((MapSetup.BlockSize + MapSetup.BlockSpacing) * MapSetup.DisplayColumns)) / 2;
	MapSetup.DisplayRect.y = 160;
	MapSetup.DisplayRect = { MapSetup.ScreenOffsX, 160, (MapSetup.BlockSize + MapSetup.BlockSpacing) * MapSetup.DisplayColumns + 1, (MapSetup.BlockSize + MapSetup.BlockSpacing) * MapSetup.DisplayRows + 1};

	// Allocate memory for a 2D matrix of tiles
	Map = (MatrixRectItem**)malloc(MapSetup.Cols * sizeof(MatrixRectItem*));
	for (int cols = 0; cols < MapSetup.Cols; cols++)
	{
		Map[cols] = (MatrixRectItem*)malloc(MapSetup.Rows * sizeof(MatrixRectItem*));
	}
	// fill the matrix with alpha 0 to make items invisible
	for (int y = 0; y < MapSetup.Rows; y++)
	{
		for (int x = 0; x < MapSetup.Cols; x++)
		{
			Map[x][y].BorderColor.a = 0;
			Map[x][y].FillColor.a = 0;
		}
	}

	Player.OnInit(Renderer);
	MainUI.OnInit(Renderer);
	Scroller.OnInit(Renderer, &MapSetup, Map);
	Editor.OnInit(AppWindow, Renderer, Map, &MapSetup);

	OnInitPlayer();

	_appIsRunning = true;
	GameStatus = GameState_LevelEdit;

	return true;
};

void GameEngine::OnEvent(SDL_Event* Event)
{
	GameEvents::OnEvent(Event);

	if (GameStatus == GameState_Running) Player.OnEvent(Event);
	if (GameStatus == GameState_LevelEdit) Editor.OnEvent(Event);
};

void GameEngine::OnLoop()
{
	if (GameStatus == GameState_Running)
	{
		Player.OnLoop();
		Scroller.OnLoop();
		//OnCollisionCheck();
	}

	if (GameStatus == GameState_LevelEdit)
	{
		Editor.OnLoop();
	}
};

void GameEngine::OnRender()
{
	SDL_SetRenderDrawColor(Renderer, 40, 40, 40, 255);
	SDL_RenderClear(Renderer);

	
	if (GameStatus == GameState_Running || GameStatus == GameState_Paused || GameStatus == GameState_GameOver)
	{
		// Render background
		Scroller.OnRender();
		// Render player(s)
		Player.OnRender();		
	}

	if (GameStatus == GameState_LevelEdit)
	{
		Editor.OnRender();
	}

	// Render UI
	MainUI.OnRender(Player.Name, Player.Score, GameStatus == GameState_GameOver);

	// do it! do it !
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

	SDL_DestroyWindow(AppWindow);
	SDL_free(Renderer);
	SDL_Quit();

}

void GameEngine::OnCollisionCheck()
{
	int playerScreenColumn, playerScreenRow;
	int playerEdgeT, playerEdgeL, playerEdgeR, playerEdgeB;
	int obstEdgeT, obstEdgeL, obstEdgeR, obstEdgeB;

	playerScreenColumn = (Player.DisplayRect.x - MapSetup.ScreenOffsX) / (MapSetup.BlockSize + MapSetup.BlockSpacing);
	playerScreenRow = (Player.DisplayRect.y - 150) / (MapSetup.BlockSize + MapSetup.BlockSpacing);

	playerEdgeT = Player.DisplayRect.y;
	playerEdgeB = Player.DisplayRect.y + MapSetup.BlockSize;
	playerEdgeL = Player.DisplayRect.x;
	playerEdgeR = Player.DisplayRect.x + MapSetup.BlockSize;

	// Draw a grid
	// Check if row and column of player make sense - exit if not.
	if (playerScreenColumn < 0 || playerScreenRow < 0 || playerScreenColumn > MapSetup.DisplayColumns || playerScreenRow > MapSetup.DisplayRows) return;
	// Check 9 possible zones
	for (int x = 0; x <= 1; x++)
	{
		for (int y = 0; y <= 1; y++)
		{
			if (playerScreenColumn == 0 && x == 0) continue;
			if (playerScreenColumn >= MapSetup.DisplayColumns - 1 && x == 2) continue;
			if (playerScreenRow == 0 && y == 0) continue;
			if (playerScreenRow >= MapSetup.DisplayRows - 1 && y == 2) continue;

			int alpha = Scroller.MapMatrix[playerScreenColumn + Scroller.ColumnPosition + x][playerScreenRow + y].FillColor.a;

			obstEdgeT = (y + playerScreenRow) * (MapSetup.BlockSize + MapSetup.BlockSpacing) + 150;
			obstEdgeB = obstEdgeT + MapSetup.BlockSize;
			obstEdgeL = (MapSetup.ScreenOffsX - Scroller.ScrollPosition) + ((x + playerScreenColumn) * (MapSetup.BlockSize + MapSetup.BlockSpacing));
			obstEdgeR = obstEdgeL + MapSetup.BlockSize;

			if (playerEdgeT > obstEdgeB || playerEdgeB < obstEdgeT || playerEdgeL > obstEdgeR || playerEdgeR < obstEdgeL) continue;


			if (alpha < 1) continue;
			GameStatus = GameState_GameOver;

		}
	}

}

void GameEngine::OnInitPlayer()
{
	Player.AnimationRate = 40;		// t=1000/framesPerSecond e.g. 1000/25 = 40
	Player.Name = "Mollmops";
	Player.TextureSourcePath = "Spritesheet_Alien_01.png";
	Player.HorizontalTiling = 6;
	Player.VerticalTiling = 4;
	Player.Speed = 2;
	Player.Score = 0;

	Player.MinPosition.x = MapSetup.ScreenOffsX;
	Player.MaxPosition.x = MapSetup.ScreenOffsX + ((MapSetup.DisplayColumns - 1) * (MapSetup.BlockSize + MapSetup.BlockSpacing));;
	Player.MinPosition.y = MapSetup.DisplayRect.y; // Bottom of Top-UI
	Player.MaxPosition.y = MapSetup.DisplayRect.y + ((MapSetup.DisplayRows - 1) * (MapSetup.BlockSize + MapSetup.BlockSpacing));

	Player.DisplayRect.w = MapSetup.BlockSize;
	Player.DisplayRect.h = MapSetup.BlockSize;
	Player.DisplayRect.x = MapSetup.ScreenOffsX;
	Player.DisplayRect.y = Player.MinPosition.y;

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
	if (sym == SDLK_F4)
	{
		if (GameStatus == GameState_MainScreen) GameStatus = GameState_LevelEdit;
	}
}

void GameEngine::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (GameStatus == GameState_Running)
	{

	}
}

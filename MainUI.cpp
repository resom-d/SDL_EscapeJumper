#include "MainUI.h"


MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_renderer, &screenWidth, &screenHeight);
	DisplayRect = { 0,0, screenWidth, 150 };
	_fontBig = TTF_OpenFont("SigmarOne-Regular.ttf", 48);
	_fontMedium = TTF_OpenFont("SigmarOne-Regular.ttf", 36);
	_fontSmall = TTF_OpenFont("SigmarOne-Regular.ttf", 24);
	_fontGameOver = TTF_OpenFont("SigmarOne-Regular.ttf", 120);
}

void MainUI::OnRender(std::string playerName, int playerScore, bool gameOver)
{
	int h, w;
	SDL_GetRendererOutputSize(_renderer, &w, &h);

	if (_renderer == nullptr) return;
	SDL_RenderSetClipRect(_renderer, &DisplayRect);
	// Give us Background and a Boarder
	SDL_SetRenderDrawColor(_renderer, 168, 127, 50, 255);
	SDL_RenderFillRect(_renderer, &DisplayRect);
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(_renderer, &DisplayRect);

	// Game-Name
	surfGameName = TTF_RenderText_Solid(_fontBig, "Escape Jumper - Make your way out ", SDL_Color{ 0, 0, 0, 255 });
	texGameName = SDL_CreateTextureFromSurface(_renderer, surfGameName);

	SDL_Rect srcRect =
	{
		0,
		0,
		surfGameName->w,
		surfGameName->h
	};

	SDL_Rect destRect =
	{
		10, 10,
		surfGameName->w,
		surfGameName->h
	};

	SDL_RenderCopy(_renderer, texGameName, &srcRect, &destRect);
	SDL_FreeSurface(surfGameName);

	// Player-Name
	surfPlayerName = TTF_RenderText_Solid(_fontMedium, playerName.c_str(), SDL_Color{ 0, 0, 0, 255 });
	texPlayerName = SDL_CreateTextureFromSurface(_renderer, surfPlayerName);

	srcRect =
	{
		0,
		0,
		surfPlayerName->w,
		surfPlayerName->h
	};

	destRect =
	{
		1200, 10,
		surfPlayerName->w,
		surfPlayerName->h
	};

	SDL_RenderCopy(_renderer, texPlayerName, &srcRect, &destRect);
	SDL_FreeSurface(surfPlayerName);

	// Player-Score
	surfPlayerScore = TTF_RenderText_Solid(_fontMedium, std::to_string(playerScore).c_str(), SDL_Color{ 0, 0, 0, 255 });
	texPlayerScore = SDL_CreateTextureFromSurface(_renderer, surfPlayerScore);

	srcRect =
	{
		0,
		0,
		surfPlayerScore->w,
		surfPlayerScore->h
	};

	destRect =
	{
		1200, 60,
		surfPlayerScore->w,
		surfPlayerScore->h
	};

	SDL_RenderCopy(_renderer, texPlayerScore, &srcRect, &destRect);
	SDL_FreeSurface(surfPlayerScore);

	// GameOver
	if (gameOver)
	{
		surfGameOver = TTF_RenderText_Solid(_fontGameOver, " Game Over", SDL_Color{ 255, 0, 0, 255 });
		texGameOver = SDL_CreateTextureFromSurface(_renderer, surfGameOver);

		srcRect =
		{
			0,
			0,
			surfGameOver->w,
			surfGameOver->h
		};

		destRect =
		{
			(w/2 - surfGameOver->w/2),
			150 + ((h-150)/2 - (surfGameOver->h/2)), 
			surfGameOver->w,
			surfGameOver->h
		};

		SDL_RenderSetClipRect(_renderer, NULL);
		SDL_RenderCopy(_renderer, texGameOver, &srcRect, &destRect);
		SDL_FreeSurface(surfGameOver);
		
		SDL_RenderSetClipRect(_renderer, &DisplayRect);
	}

	
}

void MainUI::OnPostRender()
{
	SDL_DestroyTexture(texGameName);
	SDL_DestroyTexture(texGameOver);
	SDL_DestroyTexture(texPlayerName);
	SDL_DestroyTexture(texPlayerScore);
}

void MainUI::OnCleanup()
{
}
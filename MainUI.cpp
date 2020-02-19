#include "MainUI.h"
#include <string>

MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_renderer, &screenWidth, &screenHeight);
	DisplayRect = { 0,0, screenWidth, 120 };
	_fontBig = TTF_OpenFont("SigmarOne-Regular.ttf", 48);
	_fontMedium = TTF_OpenFont("SigmarOne-Regular.ttf", 36);
	_fontSmall = TTF_OpenFont("SigmarOne-Regular.ttf", 24);
	_fontGameOver = TTF_OpenFont("SigmarOne-Regular.ttf", 120);
}

void MainUI::OnRender(std::string playerName, int playerScore, bool gameOver)
{
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

	// GameOver
	if (gameOver)
	{
		surfGameOver = TTF_RenderText_Solid(_fontGameOver," Game Over", SDL_Color{ 0, 0, 0, 255 });
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
			300, 450,
			surfGameOver->w,
			surfGameOver->h
		};

		SDL_RenderSetClipRect(_renderer, NULL);
		SDL_RenderCopy(_renderer, texGameOver, &srcRect, &destRect);
		SDL_RenderSetClipRect(_renderer, &DisplayRect);
	}
	
	SDL_RenderPresent(_renderer);

}

void MainUI::OnCleanup()
{
	SDL_free(surfGameName);
	SDL_free(surfGameOver);
	SDL_free(surfPlayerName);
	SDL_free(surfPlayerScore);

	SDL_free(texGameName);
	SDL_free(texGameOver);
	SDL_free(texPlayerName);
	SDL_free(texPlayerScore);
}
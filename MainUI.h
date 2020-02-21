#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "UI_Abstract.h"

class MainUI
{
public:
	MainUI();

	void OnInit(SDL_Renderer* renderer);
	void OnLoop();
	void OnEvent();
	void OnRender(std::string  playerName, int playerScore, bool gameOver);
	void OnPostRender();
	void OnCleanup();

	SDL_Renderer* _renderer;
	SDL_Surface* surfGameOver;
	SDL_Surface* surfGameName;
	SDL_Surface* surfPlayerName;
	SDL_Surface* surfPlayerScore;
	SDL_Texture* texGameOver;
	SDL_Texture* texGameName;
	SDL_Texture* texPlayerName;
	SDL_Texture* texPlayerScore;

	SDL_Rect DisplayRect;
	std::vector<UI_Widget> Widgets;
	TTF_Font* _fontGameOver;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
};


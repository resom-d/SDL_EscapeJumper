#include "GameStart.h"

GameStart::GameStart()
{
}

GameStart::~GameStart()
{
}

void GameStart::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	_timeCatch = SDL_GetTicks() + 3000;
	_font = TTF_OpenFont("Resources/fonts/LuckiestGuy-Regular.ttf", 72);
}

void GameStart::OnLoop(void)
{
}

void GameStart::OnRender(void)
{
	double dt = ((double)_timeCatch - (double)SDL_GetTicks()) / 1000.0;
	string st = "Spiel startet in : " + to_string((int)dt+1);
	SDL_Surface* surf = TTF_RenderText_Solid(_font, st.c_str() , { 200,60,30, 255 });
	SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
	int w = surf->w, h = surf->h;
	SDL_FreeSurface(surf);

	SDL_Rect sRect = { 0,0, w, h };
	SDL_Rect dRect = { DisplayRect.w / 2 - w / 2, DisplayRect.y + (DisplayRect.h / 2 - h / 2), w, h };
	SDL_RenderCopy(_rend, tex, &sRect, &dRect);
	SDL_DestroyTexture(tex);
	
	Done = SDL_GetTicks() > _timeCatch;
}

void GameStart::OnCleanup(void)
{
}

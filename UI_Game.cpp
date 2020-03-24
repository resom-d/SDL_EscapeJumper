#include "UI_Game.h"

UI_Game::UI_Game()
{}

UI_Game::~UI_Game()
{}

void UI_Game::OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;
	_font = TTF_OpenFont("Resources/fonts/LuckiestGuy-Regular.ttf", 40);	
}

void UI_Game::OnLoop(void)
{
	
}

void UI_Game::OnEvent(SDL_Event* event)
{
	
}

void UI_Game::OnCleanup(void)
{
	SDL_DestroyTexture(TexBackgnd);
}

void UI_Game::OnRender(bool gameOver, JumperPlayer* player)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);

	int h, w;
	SDL_QueryTexture(TexBackgnd, nullptr, nullptr, &w, &h);
	SDL_Rect srect = {0,0, w, h};
	SDL_Rect drect = { 0,0, w, h };
	SDL_RenderCopyEx(_rend, TexBackgnd, &srect, &drect, 0, nullptr, SDL_FLIP_NONE);	

	SDL_Color textCol = { 255,255,255,255 };

	SDL_Surface* surf = TTF_RenderText_Solid(_font, to_string(player->Score).c_str(), textCol);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
	srect = { 0,0, surf->w, surf->h };
	drect = { 190, 100, surf->w, surf->h };
	SDL_RenderCopy(_rend, tex, &srect, &drect);

	surf = TTF_RenderText_Solid(_font, to_string(player->Jumps).c_str(), textCol);
	tex = SDL_CreateTextureFromSurface(_rend, surf);
	srect = { 0,0, surf->w, surf->h };
	drect = { 190, 140, surf->w, surf->h };
	SDL_RenderCopy(_rend, tex, &srect, &drect);

	surf = TTF_RenderText_Solid(_font, to_string(player->Energy).c_str(), textCol);
	tex = SDL_CreateTextureFromSurface(_rend, surf);
	srect = { 0,0, surf->w, surf->h };
	drect = { 740, 100, surf->w, surf->h };
	SDL_RenderCopy(_rend, tex, &srect, &drect);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
}

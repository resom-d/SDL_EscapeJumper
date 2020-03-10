#include "UI_Game.h"

UI_Game::UI_Game()
{}

UI_Game::~UI_Game()
{}

void UI_Game::OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;

	SDL_Surface* surf = IMG_Load("Resources/avatars/Happy.png");
	_texHappy = SDL_CreateTextureFromSurface(_rend, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("Resources/avatars/Dead.png");
	_texSad = SDL_CreateTextureFromSurface(_rend, surf);
	SDL_FreeSurface(surf);

	return;

	SDL_Texture* tex;
	Userdata ud;

	int w = 50;
	int pad = 2;
	int bordW = 2;
	int gap = 5;
	SDL_Rect dRect = { 0,0, 76, 76 };

	// Load

	_bbox.OnInit(_rend);
	_bbox.Padding = 0;
	_bbox.DisplayRect = { 20, 110, 0, 80 };
	
	UI_Control btn;
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Home.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex); btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_MAINSCREEN;
	_bbox.AddChild(btn);

	btn = UI_Control();
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Joypad.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);
	btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_GAME;
	_bbox.AddChild(btn);

	btn = UI_Control();
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Editor.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);
	btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_EDITOR;
	_bbox.AddChild(btn);
}

void UI_Game::OnLoop(void)
{
	//_bbox.OnLoop();
}

void UI_Game::OnEvent(SDL_Event* event)
{
	//_bbox.OnEvent(event);
}

void UI_Game::OnCleanup(void)
{
	//_bbox.OnCleanup();
	SDL_DestroyTexture(_texHappy);
	SDL_DestroyTexture(_texSad);
}

void UI_Game::OnRender(bool gameOver, JumperPlayer* player)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	// Give us Background and a Border
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_Color col = UI_GameFill;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderFillRect(_rend, &DisplayRect);
	col = UI_GameBorder;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderDrawRect(_rend, &DisplayRect);

	//_bbox.OnRender();

	SDL_RenderStringAt(_rend, "Spieler: " + player->Name, { DisplayRect.x + 10, DisplayRect.y + 10 }, *_charMap, 36, nullptr);
	SDL_RenderStringAt(_rend, "Punkte : " + to_string(player->Score), { DisplayRect.x + 10, DisplayRect.y + 62 }, *_charMap, 36, nullptr);
	SDL_RenderStringAt(_rend, "Sprünge: " + to_string(player->Jumps), { DisplayRect.x + 10, DisplayRect.y + 114 }, *_charMap, 36, nullptr);
	
	int h, w;
	SDL_QueryTexture(_texHappy, nullptr, nullptr, &w, &h);
	SDL_Rect srect = {0,0, w, h};
	SDL_Rect drect = { 1000,10, 180, 180 };

	if (!gameOver)SDL_RenderCopyEx(_rend, player->Texture, &srect, &drect, 5, nullptr, SDL_FLIP_NONE);
	else SDL_RenderCopyEx(_rend, _texSad, &srect, &drect, 5, nullptr, SDL_FLIP_NONE);

}

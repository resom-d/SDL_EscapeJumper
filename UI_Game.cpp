#include "UI_Game.h"

UI_Game::UI_Game()
{}

UI_Game::~UI_Game()
{}

void UI_Game::OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;

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
	CreateWidgetTexture(_rend, "Resources/icons/Run.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);
	btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_GAME;
	_bbox.AddChild(btn);

	btn = UI_Control();
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Home.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex); btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_MAINSCREEN;
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
	_bbox.OnLoop();
}

void UI_Game::OnEvent(SDL_Event* event)
{
	_bbox.OnEvent(event);
}

void UI_Game::OnCleanup(void)
{
	_bbox.OnCleanup();
}

void UI_Game::OnRender(string playerName, string playerScore, bool playerStatus)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	_bbox.OnRender();

	SDL_RenderStringAt(_rend, "Spieler: " + playerName, { DisplayRect.x + 10, DisplayRect.y + 10 }, *_charMap, 42, nullptr);
	SDL_RenderStringAt(_rend, "Punkte: " + playerScore, { DisplayRect.x + 10, DisplayRect.y + 62 }, *_charMap, 42, nullptr);
	string stat = playerStatus ? "Dead" : "Alive";
	SDL_RenderStringAt(_rend, "Status: " + stat, { DisplayRect.x + 1000, DisplayRect.y + 62 }, *_charMap, 22, nullptr);
}

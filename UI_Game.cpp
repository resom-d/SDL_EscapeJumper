#include "UI_Game.h"

UI_Game::UI_Game()
{}

UI_Game::~UI_Game()
{}

void UI_Game::OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;

	_bbox.OnInit(_rend);
	_bbox.Padding = 0;
	_bbox.DisplayRect = { 20, 110, 0, 80 };
	
	UI_Control btn;
	btn.OnInit(_rend);
	btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_GAME;
	_bbox.AddChild(btn);

	btn = UI_Control();
	btn.OnInit(_rend);
	btn.Margin = 5;
	btn.DisplayRect = { 0,0, 70,70 };
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_MAINSCREEN;
	_bbox.AddChild(btn);

	btn = UI_Control();
	btn.OnInit(_rend);
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

void UI_Game::OnRender(string playerName, string playerScore)
{
	_bbox.OnRender();

	SDL_RenderStringAt(_rend, "Spieler: " + playerName, { DisplayRect.x + 10, DisplayRect.y + 10 }, *_charMap, 42, nullptr);
	SDL_RenderStringAt(_rend, "Punkte: " + playerScore, { DisplayRect.x + 10, DisplayRect.y + 62 }, *_charMap, 42, nullptr);
}

#include "UI_Main.h"


MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer, CharacterTextureMap charMap)
{
	_rend = renderer;
	_charMap = charMap;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_rend, &screenWidth, &screenHeight);
	
	UI_Control btn;
	SDL_Texture* tex;
	SDL_Rect srcRect = { 0,0,76,76 };
	SDL_Rect destRect = { 0,0, 100, 100 };
	
	int w = 100;
	int pad = 2;
	int gap = 80;
	int bordW = 2;
	SDL_Rect dRect = {  350, DisplayRect.h - 110 , 100 , 100 };

	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Joypad.png", tex, srcRect, destRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);;
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_GAME;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Editor.png", tex, srcRect, destRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);;
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_EDITOR;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Close.png", tex, srcRect, destRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);;
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::QUIT_GAME;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;	
		
}

void MainUI::OnEvent(SDL_Event* event)
{
	for (auto iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnEvent(event);
	}

}

void MainUI::OnRender(std::string playerName, int playerScore, bool gameOver)
{
	int h, w;
	SDL_GetRendererOutputSize(_rend, &w, &h);

	if (_rend == nullptr) return;
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	// Give us Background and a Border
	SDL_Color col = UI_EditorFill;
	SDL_SetRenderDrawColor(_rend, col.r, col.b, col.g, col.a );
	SDL_RenderFillRect(_rend, &DisplayRect);
	col = UI_EditorBorder;
	SDL_SetRenderDrawColor(_rend, col.r, col.b, col.g, col.a);
	SDL_RenderDrawRect(_rend, &DisplayRect);

	SDL_RenderStringAt(_rend, "ESCAPE JUMPER", { 150, 10 }, _charMap, 60, &DisplayRect);

	for (auto iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnRender();
	}

	
}

void MainUI::OnPostRender()
{
	
}

void MainUI::OnCleanup()
{
	for (auto iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnCleanup();
	}


}
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
	int gap = 20;
	int bordW = 2;
	SDL_Rect dRect = {  50, DisplayRect.h - 110 , 100 , 100 };

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
	dRect.x += w + gap + 50;	
	
	Avatars = GetFilesInDirectory("Resources/sprites");

	for (auto avatar = Avatars.begin(); avatar != Avatars.end(); avatar++)
	{
		SDL_Surface* surf = IMG_Load(avatar->string().c_str());
		SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
		SDL_FreeSurface(surf);
		Userdata ud;
		ud.Texture = tex;

		UI_Control ct;
		ct.OnInit(_rend, tex);
		ct.UserData = ud;
		ct.EventType = GAME_EVENT_TYPE;
		ct.ActionCode = UI_ACTION::SET_PLAYER_AVATAR;
		ct.DisplayRect = { dRect.x, dRect.y, 70, 70 };
		dRect.x += 75;
		Buttons.push_back(ct);
	}

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
	if (_rend == nullptr) return;
	
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	// Give us Background and a Border
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_Color col = UI_EditorFill;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderFillRect(_rend, &DisplayRect);
	col = UI_EditorBorder;
	SDL_RenderSetDrawColor(_rend, col);
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
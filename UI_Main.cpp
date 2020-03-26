#include "UI_Main.h"


MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer, JumperPlayer* player)
{
	_rend = renderer;
	_player = player;
	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_rend, &screenWidth, &screenHeight);
	
	_font = TTF_OpenFont("Resources/fonts/LuckiestGuy-Regular.ttf", 48);
	_charMap = SDL_GetTexturesFromString(_rend, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyzäöü,.;:*#-_|<>^°?=()!\"§$%&/()@€~", _font, { 255,255,0, 255 });

	txtPlayerName.DisplayRect = { 630, 30, 550, 50 };
	txtPlayerName.Padding = 8;
	txtPlayerName.TextBuffer = &_player->Name;
	txtPlayerName.Vocabular = " ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyz.-_@#ÄäöÖüÜß";
	txtPlayerName.MaxSize = 20;
	txtPlayerName.OnInit(_rend, _charMap);	

	UI_Control btn;
	SDL_Texture* tex;
	SDL_Rect srcRect = { 0,0,76,76 };
	SDL_Rect destRect = { 0,0, 96, 96 };
	
	int w = 76;
	int pad = 2;
	int gap = 10;
	int bordW = 2;
	SDL_Rect dRect = {  10, DisplayRect.h - 90 , w , w };
	
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
	
	dRect = { DisplayRect.w - 180, 10 , w, w };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Editor.png", tex, srcRect, destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_EDITOR;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + 10;

	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Close.png", tex, srcRect, destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::QUIT_GAME;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap + 50;		

}

void MainUI::OnLoop()
{
	txtPlayerName.OnLoop();
}

void MainUI::OnEvent(SDL_Event* event)
{
	for (auto iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnEvent(event);
	}
	txtPlayerName.OnEvent(event);
}

void MainUI::OnRender(bool gameOver)
{
	if (_rend == nullptr) return;
	
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	
	SDL_Rect rect = { 0,0, DisplayRect.w, DisplayRect.h };
	SDL_RenderCopy(_rend, Texture, &rect, &rect);

	for (auto iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnRender();
	}

	txtPlayerName.OnRender();
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
	txtPlayerName.OnCleanup();

}
#include "UI_NewMapDialog.h"

UI_NewMapDialog::UI_NewMapDialog()
{
}

UI_NewMapDialog::~UI_NewMapDialog()
{
}

void UI_NewMapDialog::OnInit(SDL_Renderer* rend)
{
	_rend = rend;

	TTF_Font* font = TTF_OpenFont("Resources/fonts/arial.ttf", 24);
	_charMap = SDL_GetTexturesFromString(_rend, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜßabcdefghijklmnopqrstuvwxyzäöü,.;:*#-_|<>^°?=()!\"§$%&/()@€~", font, { 0,0,0, 255 });

	UI_Control btn;
	SDL_Texture* tex;
	
	int w = 50;
	int pad = 2;
	int bordW = 2;
	int gap = 5;
	SDL_Rect sRect = { 5,5, 76,76 };
	SDL_Rect dRect = { 5,5, w, w };

	_txtFilename.DisplayRect = { DisplayRect.x + 5, DisplayRect.y + 5, 400, 50 };
	_txtCols.DisplayRect = { DisplayRect.x + 5, DisplayRect.y + 55, 400, 50 };
	_bbox.DisplayRect = { DisplayRect.x + 5, DisplayRect.y + 105, 110, 60 };
	_txtCols.TextBuffer = &_mapCols;
	_txtCols.Vocabular = "0123456789";
	_txtCols.MaxSize = 4;
	_txtFilename.TextBuffer = &_filename;
	_txtFilename.Vocabular = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
	_txtFilename.MaxSize = 30;
	_txtCols.OnInit(_rend, _charMap);
	_txtFilename.OnInit(_rend, _charMap);

	// New map Ok
	Userdata ud;
	if (_mapCols.length() < 1) _mapCols = "500";
	ud.NewMapCols = stoi(_mapCols);
	ud.NewMapName = &_filename;
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Ok.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);
	btn.UserData = ud;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::MAP_NEW;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_bbox.AddChild(btn);	

	// New map cancel
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Close.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::CLOSE_EDITOR_NEW_MAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_bbox.AddChild(btn);
	
}

void UI_NewMapDialog::OnLoop(void)
{
	_bbox.OnLoop();
	_txtFilename.OnLoop();
	_txtCols.OnLoop();
}

void UI_NewMapDialog::OnRender(void)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_RenderSetDrawColor(_rend, { 50,50,50, 255 });
	SDL_RenderFillRect(_rend, &DisplayRect);

	_bbox.OnRender();
	_txtFilename.OnRender();
	_txtCols.OnRender();
}

void UI_NewMapDialog::OnEvent(SDL_Event* event)
{
	_bbox.OnEvent(event);
	_txtFilename.OnEvent(event);
	_txtCols.OnEvent(event);

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		int x = event->button.x;
		int y = event->button.y;

		if (!(x >= DisplayRect.x && x <= DisplayRect.x + DisplayRect.w &&
			y >= DisplayRect.y && y <= DisplayRect.y + DisplayRect.h))
		{
			SDL_Event Event;
			SDL_zero(Event);
			Event.type = EDITOR_EVENT_TYPE;
			Event.user.code = (Sint32)UI_ACTION::CLOSE_EDITOR_NEW_MAP;;
			Event.user.data1 = this;
			SDL_PushEvent(&Event);
		}
	}
}

void UI_NewMapDialog::OnCleanup(void)
{
	_bbox.OnCleanup();
	_txtCols.OnCleanup();
	_txtFilename.OnCleanup();
}

#include "UI_MapConfig.h"

void UI_MapConfig::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	TTF_Font* font = TTF_OpenFont("Resources/fonts/ShareTechMono-Regular.ttf", 40);
	_charMap = SDL_GetTexturesFromString(_rend, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ????abcdefghijklmnopqrstuvwxyz???,.;:*#-_|<>^??=()!\"?$%&/()@?~", font, { 0,0,0, 255 });
	_gridUpperLeft = { DisplayRect.x + 10, DisplayRect.y + 120 };

	_bbox.DisplayRect = { 5, 5, 0, 80 };
	_bbox.OnInit(_rend);
	CreateControls();
}

void UI_MapConfig::OnLoop(void)
{
	for (auto txt = _txtInputs.begin(); txt != _txtInputs.end(); txt++)
	{
		txt->OnLoop();
	}
	_bbox.OnLoop();
}

void UI_MapConfig::OnRender(void)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_Color col = EDITOR_Fill;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderFillRect(_rend, &DisplayRect);
	
	col = UI_EditorFill;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_Rect r = { DisplayRect.x, DisplayRect.y, DisplayRect.w, 100 };
	SDL_RenderFillRect(_rend, &r);
	SDL_SetRenderDrawColor(_rend, 0, 0, 0, 255);
	SDL_RenderDrawRect(_rend, &r);

	SDL_Point ip = { _gridUpperLeft.x + 10,  _gridUpperLeft.y};
	SDL_RenderStringAt(_rend, "Dateiname", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Spalten", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Reihen", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Spalten Anzeige", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Reihen Anzeige", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Block-Gr??e", ip, _charMap, 24, nullptr);
	ip.y += 45;
	SDL_RenderStringAt(_rend, "Block-Abstand", ip, _charMap, 24, nullptr);
	ip.y += 45;
	for (auto txt = _txtInputs.begin(); txt != _txtInputs.end(); txt++)
	{
		txt->OnRender();

	}

	_bbox.OnRender();
}

void UI_MapConfig::OnEvent(SDL_Event* event)
{
	for (auto txt = _txtInputs.begin(); txt != _txtInputs.end(); txt++)
	{
		txt->OnEvent(event);
	}
	_bbox.OnEvent(event);
}

void UI_MapConfig::OnCleanup(void)
{
	for (auto txt = _txtInputs.begin(); txt != _txtInputs.end(); txt++)
	{
		txt->OnCleanup();
	}
	_bbox.OnCleanup();
}

void UI_MapConfig::CreateControls()
{	
	SDL_Texture* tex;
	Userdata ud;

	int w = 50;
	int bordW = 2;
	int gap = 5;
	int pad = 5;
	SDL_Rect dRect = { 5,5, 76, 76 };

	// Load
	UI_Control co;
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Editor.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	co.OnInit(_rend, tex);
	co.EventType = EDITOR_EVENT_TYPE;
	co.ActionCode = UI_ACTION::GO_EDITOR;
	co.DisplayRect = { 0,0, 70, 70 };
	co.Margin = gap;
	_bbox.AddChild(co);

	co = UI_Control();
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/FileOpen.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	co.OnInit(_rend, tex);
	co.EventType = EDITOR_EVENT_TYPE;
	co.ActionCode = UI_ACTION::LOADMAP;
	co.DisplayRect = { 0,0, 70, 70 };
	co.Margin = gap;
	_bbox.AddChild(co);

	co = UI_Control();
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/FileSave.png", tex, dRect, dRect, 0, SDL_FLIP_NONE);
	co.OnInit(_rend, tex);
	co.EventType = EDITOR_EVENT_TYPE;
	co.ActionCode = UI_ACTION::SAVEMAP;
	co.DisplayRect = { 0,0, 70, 70 };
	co.Margin = gap;
	_bbox.AddChild(co);

	// Properties-Grid
	SDL_Point iP = { _gridUpperLeft.x + 400, _gridUpperLeft.y};
	UI_TextInput ti;
	ti.OnInit(_rend, _charMap);
	ti.MaxSize = 30;
	ti.Vocabular = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sFilename;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.Vocabular = "0123456789";
	ti.MaxSize = 4;
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sCols;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.Vocabular = "0123456789";
	ti.MaxSize = 4;
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sRows;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.MaxSize = 2;
	ti.Vocabular = "0123456789";
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sDCols;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.Vocabular = "0123456789";
	ti.MaxSize = 2;
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sDRows;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.Vocabular = "0123456789";
	ti.MaxSize = 3;
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sBSize;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;

	ti = UI_TextInput();
	ti.OnInit(_rend, _charMap);
	ti.Vocabular = "0123456789";
	ti.MaxSize = 2;
	ti.DisplayRect = { iP.x, iP.y, 600, 40 };
	ti.TextBuffer = &sBSpace;
	_txtInputs.push_back(ti);
	iP.y += ti.DisplayRect.h + pad;


}


#include "UI_Editor.h"

UI_Editor::UI_Editor()
{
	FillColor = UI_EditorFill;
	BorderColor = UI_EditorBorder;

}

void UI_Editor::OnInit(SDL_Renderer* renderer, GameMap* map, CharacterTextureMap charMap, ColorPalette colors)
{
	_rend = renderer;
	_map = map;
	_charmap = charMap;
	_colorPalette = colors;
	_activeTool = UI_ACTION::DRAWMODE;
	_tileResourceDPoint = { 750,5 };

	_confScreen.DisplayRect = { 800, 20, 375, 185 };

	SDL_Texture* orgTex = SDL_GetRenderTarget(_rend);

	SDL_Rect srcRect = { 0,0,76,76 };
	SDL_Rect destRect = { 0,0, 76,76 };
	ConfigureWidgets(&srcRect, &destRect);

	// Fill-ColorPallet
	FillColorWidgets.Orientation = WIDGET_ORIENTATION::HORIZONTAL;
	FillColorWidgets.BorderWidth = 2;
	FillColorWidgets.DisplayRect =
	{
		DisplayRect.x + 5,
		DisplayRect.y + 60,
		0,
		44
	};
	int x = 0;
	for (auto fillIter = _colorPalette.begin(); fillIter != _colorPalette.end(); fillIter++)
	{
		Userdata data;
		UI_Control btn;
		btn.ActionCode = UI_ACTION::SET_FILL_COLOR;
		btn.EventType = UI_EDITOR_EVENT_TYPE;
		data.ColorIndex = x;
		btn.UserData = data;
		btn.FillColor = *fillIter;
		btn.Margin = 5;
		btn.BorderWidth = 1;
		btn.DisplayRect.w = 30;
		btn.DisplayRect.h = 30;
		FillColorWidgets.AddChild(btn);
		x++;
	}
	FillColorWidgets.OnInit(_rend, UI_ACTION::DRAWMODE);

	// Border-ColorPallet
	BorderColorWidgets.Orientation = WIDGET_ORIENTATION::HORIZONTAL;
	BorderColorWidgets.BorderWidth = 2;
	BorderColorWidgets.DisplayRect =
	{
		DisplayRect.x + 5,
		DisplayRect.y + 110,
		0,
		44
	};
	x = 0;
	for (auto borderIter = _colorPalette.begin(); borderIter != _colorPalette.end(); borderIter++)
	{
		UI_Control btn;
		btn.ActionCode = UI_ACTION::SET_BORDER_COLOR;
		btn.EventType = UI_EDITOR_EVENT_TYPE;
		Userdata data;
		data.ColorIndex = x;
		btn.UserData = data;
		btn.FillColor = *borderIter;
		btn.Margin = 5;
		btn.BorderWidth = 1;
		btn.DisplayRect.w = 30;
		btn.DisplayRect.h = 30;
		BorderColorWidgets.AddChild(btn);
		x++;
	}
	BorderColorWidgets.OnInit(_rend, UI_ACTION::DRAWMODE);

	txtFilename.MaxSize = 30;
	txtFilename.Padding = 5;
	txtFilename.Vocabular = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
	txtFilename.OnInit(_rend, _charmap);
	txtFilename.TextBuffer = &FilenameSave;
	txtFilename.DisplayRect = { 220, 160, 600, 35 };

	// Cleanup
	SDL_RenderSetClipRect(_rend, nullptr);
}

void UI_Editor::ConfigureWidgets(SDL_Rect* srcRect, SDL_Rect* destRect)
{
	UI_Control btn;
	SDL_Texture* tex;
	Userdata ud;

	int w = 50;
	int pad = 2;
	int bordW = 2;
	int gap = 5;
	SDL_Rect dRect = { 5,5, 50,50 };

	// home
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Home.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn.OnInit(_rend, tex);
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_MAINSCREEN;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Game
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Joypad.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);
	btn.EventType = GAME_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_GAME;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Load Prev
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Forward.png", tex, *srcRect, *destRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::EDIT_LOAD_PREV;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Load next
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Forward.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::EDIT_LOAD_NEXT;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Save 
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/FileSave.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SAVEMAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Drawmode
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Draw.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::DRAWMODE;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Blockdraw
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Border.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::BORDERDRAWMODE;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// +/- 3 means scroll start,+/ -2 means scroll page, +/-1 mean scroll increment
	// Scroll X Start
	ud = Userdata();
	ud.Scrollposition = { -3, 0 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollEnd.png", tex, *srcRect, *destRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll X left
	ud = Userdata();
	ud.Scrollposition = { -1, 0 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Scroll.png", tex, *srcRect, *destRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll X Right
	ud = Userdata();
	ud.Scrollposition = { 1, 0 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Scroll.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll X End
	ud = Userdata();
	ud.Scrollposition = { 3, 0 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollEnd.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll Y Start
	ud = Userdata();
	ud.Scrollposition = { 0, -3 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollEnd.png", tex, *srcRect, *destRect, -90, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll Y increment Up
	ud = Userdata();
	ud.Scrollposition = { 0, -1 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Scroll.png", tex, *srcRect, *destRect, -90, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll Y increment down
	ud = Userdata();
	ud.Scrollposition = { 0, 1 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Scroll.png", tex, *srcRect, *destRect, 90, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Scroll Y End
	ud = Userdata();
	ud.Scrollposition = { 0, 3 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollEnd.png", tex, *srcRect, *destRect, 90, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SCROLL_TO;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Prev Tilemap
	ud = Userdata();
	ud.Scrollposition = { 0, 3 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Prev.png", tex, *srcRect, *destRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::PREV_RESOURCEINDEX;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Next Tilemap
	ud = Userdata();
	ud.Scrollposition = { 0, 3 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Prev.png", tex, *srcRect, *destRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::NEXT_RESOURCEINDEX;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	// Tilemap Config
	ud = Userdata();
	ud.Scrollposition = { 0, 3 };
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Cog.png", tex, *srcRect, *destRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::GO_EDITOR_CONFIG;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	Buttons.push_back(btn);
	dRect.x += w + gap;

	_tileResourceDPoint.x = dRect.x;
}

void UI_Editor::OnLoop()
{
	if (ConfigShown) _confScreen.OnLoop();
	else
	{
		list<UI_Control>::iterator iter = Buttons.begin();
		advance(iter, 5);
		iter->IsActive = _activeTool == UI_ACTION::DRAWMODE;
		advance(iter, 1);
		iter->IsActive = _activeTool == UI_ACTION::BORDERDRAWMODE;
		txtFilename.OnLoop();
	}

}

void UI_Editor::OnEvent(SDL_Event* event)
{
	if (event->type == EDITOR_EVENT_TYPE)
	{
		switch (event->user.code)
		{
		case (Sint32)UI_ACTION::DRAWMODE:
			_activeTool = (UI_ACTION)event->user.code;
			break;

			case (Sint32)UI_ACTION::BORDERDRAWMODE:
			_activeTool = (UI_ACTION)event->user.code;
			break;

			case (Sint32)UI_ACTION::GO_EDITOR_CONFIG:
				_confScreen.OnInit(_rend, &_charmap);
				ConfigShown = true;
				break;

			case (Sint32)UI_ACTION::CLOSE_EDITOR_CONFIG:
				_confScreen.OnCleanup();
				ConfigShown = false;
				break;
		}

	}
	if (ConfigShown) _confScreen.OnEvent(event);
	else
	{
		txtFilename.OnEvent(event);
		for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
		{
			_widgetsIter->OnEvent(event);
		}
		FillColorWidgets.OnEvent(event);
		BorderColorWidgets.OnEvent(event);
		if (event->type == UI_EDITOR_EVENT_TYPE)
		{
			SDL_Event newEvent;
			SDL_zero(newEvent);

			newEvent.type = EDITOR_EVENT_TYPE;
			newEvent.user.data1 = this;
			newEvent.user.data2 = event->user.data2;
			newEvent.user.code = event->user.code;

			SDL_PushEvent(&newEvent);
		}
		if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (_map->TextureResources.size() < 1) return;
		Size2D texSize;
		auto iter = _map->TextureResources.begin();
		int mx = event->button.x, my = event->button.y;
		SDL_QueryTexture(iter->Texture, nullptr, nullptr, &texSize.w, &texSize.h);
		if (mx >= _tileResourceDPoint.x && mx <= _tileResourceDPoint.x + texSize.w && my >= _tileResourceDPoint.y && my <= _tileResourceDPoint.y + texSize.h)
		{
			int col = (mx - _tileResourceDPoint.x) / iter->Tilesize.w;
			int row = (my - _tileResourceDPoint.y) / iter->Tilesize.h;
			_setTileData.ResourceIndex = ResourceIndex;
			_setTileData.TileIndex = (row * iter->Cols) + col + 1;

			SDL_zero(_setTileIdEvent);
			_setTileIdEvent.type = UI_EDITOR_EVENT_TYPE;
			_setTileIdEvent.user.code = (int)UI_ACTION::SET_TILEINDEX;
			_setTileIdEvent.user.data1 = this;
			_setTileIdEvent.user.data2 = &_setTileData;;

			SDL_PushEvent(&_setTileIdEvent);
		}
	}
	}
}

void UI_Editor::OnRender(Uint16 colPos, Uint16 rowPos)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	// Give us Background and a Border
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_Color col = UI_EditorFill;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderFillRect(_rend, &DisplayRect);
	col = UI_EditorBorder;
	SDL_RenderSetDrawColor(_rend, col);
	SDL_RenderDrawRect(_rend, &DisplayRect);

	txtFilename.OnRender();

	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnRender();
	}

	FillColorWidgets.OnRender();
	BorderColorWidgets.OnRender();


	SDL_RenderStringAt(_rend, "X " + to_string(colPos + 1) + "-" + to_string(colPos + 1 + _map->Setup.DisplayCols), { 10, 160 }, _charmap, 22, nullptr);
	SDL_RenderStringAt(_rend, "Y " + to_string(rowPos + 1) + "-" + to_string(rowPos + 1 + _map->Setup.DisplayRows), { 10, 175 }, _charmap, 22, nullptr);

	RenderTileResource(ResourceIndex, _tileResourceDPoint);

	if (ConfigShown) _confScreen.OnRender();
}

void UI_Editor::OnPostRender()
{
	SDL_DestroyTexture(tex);
}

void UI_Editor::OnCleanup()
{
	txtFilename.OnCleanup();

	for (list<UI_Control>::iterator iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnCleanup();
	}

	FillColorWidgets.OnCleanup();
	BorderColorWidgets.OnCleanup();

	Buttons.clear();

	_confScreen.OnCleanup();
}

void UI_Editor::RenderTileResource(Uint16 index, SDL_Point dispPoint)
{
	if (_map->TextureResources.size() < 1 || index > _map->TextureResources.size() || index < 1) return;

	auto resource = _map->TextureResources.begin();
	advance(resource, index - 1);

	int w, h;
	SDL_QueryTexture(resource->Texture, nullptr, nullptr, &w, &h);

	if (resource->Tilesize.w * resource->Rows != w) return;
	if (resource->Tilesize.h * resource->Cols != h) return;

	SDL_Rect sRect = { 0,0, resource->Tilesize.w, resource->Tilesize.h };
	SDL_Rect dRect = { dispPoint.x, dispPoint.y, resource->Tilesize.w, resource->Tilesize.h };
	for (auto x = 0; x < resource->Rows; x++)
	{
		sRect.x = 0;
		dRect.x = dispPoint.x;
		for (auto y = 0; y < resource->Cols; y++)
		{
			SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
			SDL_RenderSetClipRect(_rend, &dRect);
			SDL_RenderCopy(_rend, resource->Texture, &sRect, &dRect);

			sRect.x += resource->Tilesize.w;
			dRect.x += resource->Tilesize.w;
		}

		sRect.y += resource->Tilesize.h;
		dRect.y += resource->Tilesize.h;
	}
}

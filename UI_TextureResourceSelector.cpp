#include "UI_TextureResourceSelector.h"

void UI_TextureResourceSelector::OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;
	_tilemapIndex = 0;
	_tilemaps = GetFilesInDirectory("Resources/tilemaps");
	_resource.Cols = 3;
	_resource.Rows = 3;
	_resource.MaxIndex = 9;
	_resource.Tilesize = { 35, 35 };
	_resource.Type = TileType::Background;

	if (_tilemaps.size() > 0)
	{
		path p = *next(_tilemaps.begin(), _tilemapIndex);
		_resource.Path = p.string();
		SDL_Surface* surf = IMG_Load(p.string().c_str());
		_texture = SDL_CreateTextureFromSurface(_rend, surf);
		SDL_FreeSurface(surf);
	}

	_toolbar.OnInit(_rend);
	_toolbar.DisplayRect = { DisplayRect.x + 5, DisplayRect.y + 5, 0, 60 };
	_toolbar.Padding = 5;

	UI_Control btn;
	SDL_Texture* tex;
	Userdata ud;

	int w = 50;
	int pad = 2;
	int bordW = 2;
	int gap = 5;
	SDL_Rect sRect = { 0, 0, 76, 76 };
	SDL_Rect dRect = { 0, 0, 50, 50 };

	// Back
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Forward.png", tex, sRect, dRect, 0, SDL_FLIP_HORIZONTAL);
	btn.OnInit(_rend, tex);
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::CLOSE_EDITOR_CONFIG;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// Prev. tilemap
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Prev.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);
	btn.EventType = UI_EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::PREV_TILEMAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// next tilemap
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Prev.png", tex, sRect, dRect, 0, SDL_FLIP_HORIZONTAL);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = UI_EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::NEXT_TILEMAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// add tilemap
	ud = Userdata();
	ud.TilemapResource = &_resource;
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Add.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::ADD_TILEMAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// replace tilemap
	ud = Userdata();
	ud.TilemapResource = &_resource;
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Replace.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.UserData = ud;
	btn.OnInit(_rend, tex);;
	btn.EventType = EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::REPLACE_TILEMAP;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// background collision
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Block.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = UI_EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SET_TILETYPE_BGND;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);

	// background collision
	tex = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dRect.w, dRect.h);
	CreateWidgetTexture(_rend, "Resources/icons/Crown.png", tex, sRect, dRect, 0, SDL_FLIP_NONE);
	btn = UI_Control();
	btn.OnInit(_rend, tex);;
	btn.EventType = UI_EDITOR_EVENT_TYPE;
	btn.ActionCode = UI_ACTION::SET_TILETYPE_COIN;
	btn.DisplayRect = dRect;
	btn.BorderWidth = bordW;
	btn.Padding = pad;
	_toolbar.AddChild(btn);
}

void UI_TextureResourceSelector::OnLoop(void)
{
	_toolbar.OnLoop();
}

void UI_TextureResourceSelector::OnEvent(SDL_Event* event)
{
	_toolbar.OnEvent(event);
	
	next(_toolbar.Children.begin(), 5)->IsActive = _resource.Type == TileType::Background;
	next(_toolbar.Children.begin(), 6)->IsActive = _resource.Type == TileType::Coin;

	if (event->type == UI_EDITOR_EVENT_TYPE)
	{
		path p;
		SDL_Surface* surf = nullptr;
		switch (event->user.code)
		{
		case (int)UI_ACTION::PREV_TILEMAP:
			_tilemapIndex--;
			if (_tilemapIndex < 0) _tilemapIndex = _tilemaps.size() - 1;
			p = *next(_tilemaps.begin(), _tilemapIndex);
			_resource.Path = p.string();
			surf = IMG_Load(p.string().c_str());
			SDL_DestroyTexture(_texture);
			_texture = SDL_CreateTextureFromSurface(_rend, surf);
			SDL_FreeSurface(surf);
			break;

		case (int)UI_ACTION::NEXT_TILEMAP:
			_tilemapIndex++;
			if (_tilemapIndex > _tilemaps.size() - 1) _tilemapIndex = 0;
			p = *next(_tilemaps.begin(), _tilemapIndex);
			_resource.Path = p.string();
			surf = IMG_Load(p.string().c_str());
			SDL_DestroyTexture(_texture);
			_texture = SDL_CreateTextureFromSurface(_rend, surf);
			SDL_FreeSurface(surf);
			break;

		case (int)UI_ACTION::SET_TILETYPE_BGND:
			_resource.Type = TileType::Background;
			break;

		case (int)UI_ACTION::SET_TILETYPE_COIN:
			_resource.Type = TileType::Coin;
			break;

		}
	}

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
			Event.user.code = (Sint32)UI_ACTION::CLOSE_EDITOR_CONFIG;;
			Event.user.data1 = this;
			SDL_PushEvent(&Event);
		}
	}
}

void UI_TextureResourceSelector::OnRender(void)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_RenderSetDrawColor(_rend, { 50,50,50, 255 });
	SDL_RenderFillRect(_rend, &DisplayRect);

	_toolbar.OnRender();
	
	path p = *next(_tilemaps.begin(), _tilemapIndex);

	int w, h;
	SDL_QueryTexture(_texture, nullptr, nullptr, &w, &h);

	SDL_Rect dRect = { DisplayRect.x + 4, DisplayRect.y + 70, w, h };
	SDL_RenderCopy(_rend, _texture, nullptr, &dRect);

}

void UI_TextureResourceSelector::OnCleanup(void)
{
	_tilemaps.clear();
	_toolbar.OnCleanup();
	SDL_DestroyTexture(_texture);
}

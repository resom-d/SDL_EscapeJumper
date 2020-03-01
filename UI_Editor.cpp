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

	_tileResourceDPoint = { 650,5 };

	_activeTool = UI_ACTION::DRAWMODE;

	btnSetDrawMode.EventType = EDITOR_EVENT_TYPE;
	btnSetBlockdrawMode.EventType = EDITOR_EVENT_TYPE;
	btnScrollLeft.EventType = EDITOR_EVENT_TYPE;
	btnScrollRight.EventType = EDITOR_EVENT_TYPE;
	btnScrollBlockLeft.EventType = EDITOR_EVENT_TYPE;
	btnScrollBlockRight.EventType = EDITOR_EVENT_TYPE;

	btnSetDrawMode.ActionCode = UI_ACTION::DRAWMODE;
	btnSetBlockdrawMode.ActionCode = UI_ACTION::BORDERDRAWMODE;
	btnScrollLeft.ActionCode = UI_ACTION::SCROLL_BLOCK_LEFT;
	btnScrollRight.ActionCode = UI_ACTION::SCROLL_BLOCK_RIGHT;
	btnScrollBlockLeft.ActionCode = UI_ACTION::SCROLL_BLOCK_START;
	btnScrollBlockRight.ActionCode = UI_ACTION::SCROLL_BLOCK_END;


	SDL_Texture* orgTex = SDL_GetRenderTarget(_rend);

	SDL_Rect srcRect = { 0,0,48,48 };
	SDL_Rect destRect = { 0,0, 48,48 };
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
	for (_colorPaletteIter = _colorPalette.begin(); _colorPaletteIter != _colorPalette.end(); _colorPaletteIter++)
	{
		UI_Control btn;
		btn.ActionCode = UI_ACTION::SET_FILL_COLOR;
		btn.EventType = UI_EDITOR_EVENT_TYPE;
		Userdata data;
		data.ColorIndex = x;
		data.Color = *_colorPaletteIter;
		btn.UserData = data;
		btn.FillColor = *_colorPaletteIter;
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
		DisplayRect.y + 115,
		0,
		44
	};
	x = 0;
	for (_colorPaletteIter = _colorPalette.begin(); _colorPaletteIter != _colorPalette.end(); _colorPaletteIter++)
	{
		UI_Control btn;
		btn.ActionCode = UI_ACTION::SET_BORDER_COLOR;
		btn.EventType = UI_EDITOR_EVENT_TYPE;
		Userdata data;
		data.ColorIndex = x;
		data.Color = *_colorPaletteIter;
		btn.UserData = data;
		btn.FillColor = *_colorPaletteIter;
		btn.Margin = 5;
		btn.BorderWidth = 1;
		btn.DisplayRect.w = 30;
		btn.DisplayRect.h = 30;
		BorderColorWidgets.AddChild(btn);
		x++;
	}
	BorderColorWidgets.OnInit(_rend, UI_ACTION::DRAWMODE);

	_txtSaveFilename.OnInit(_rend, _charmap, &FilenameSave);
	_txtSaveFilename.DisplayRect = { 800,5, 600, 50 };

	// Cleanup
	SDL_RenderSetScale(_rend, 1.0, 1.0);
	SDL_SetRenderTarget(_rend, orgTex);
	SDL_RenderSetClipRect(_rend, nullptr);

}

void UI_Editor::CreateWidgetTexture(SDL_Renderer* rend, string filePath, SDL_Texture* destTex, SDL_Rect srcRect, SDL_Rect destRect)
{
	SDL_Surface* s = IMG_Load(filePath.c_str());
	SDL_Texture* t = SDL_CreateTextureFromSurface(rend, s);
	SDL_SetTextureBlendMode(destTex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(rend, destTex);
	SDL_RenderSetClipRect(rend, &destRect);
	SDL_RenderCopy(rend, t, &srcRect, &destRect);
	SDL_RenderSetClipRect(rend, nullptr);
	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
}

void UI_Editor::ConfigureWidgets(SDL_Rect* srcRect, SDL_Rect* destRect)
{
	LoadTextures(destRect, srcRect);

	btnSetDrawMode.OnInit(_rend, texSetDrawMode, "DRAW");
	btnSetDrawMode.DisplayRect = { 5,5, 50, 50 };
	btnSetDrawMode.BorderWidth = 2;
	btnSetDrawMode.Padding = 5;

	btnSetBlockdrawMode.OnInit(_rend, texSetBlockdrawMode, "ERASE");
	btnSetBlockdrawMode.DisplayRect = { 65, 5, 50, 50 };
	btnSetBlockdrawMode.BorderWidth = 2;
	btnSetBlockdrawMode.Padding = 5;

	btnScrollBlockLeft.OnInit(_rend, texSetBlockscrollStart, "|<");
	btnScrollBlockLeft.DisplayRect = { 125, 5, 50, 50 };
	btnScrollBlockLeft.BorderWidth = 2;
	btnScrollBlockLeft.ActionCode = UI_ACTION::SCROLL_BLOCK_START;
	btnScrollBlockLeft.Padding = 5;

	btnScrollBlockRight.OnInit(_rend, texScrollBlockLeft, "<");
	btnScrollBlockRight.DisplayRect = { 185, 5, 50, 50 };
	btnScrollBlockRight.BorderWidth = 2;
	btnScrollBlockRight.ActionCode = UI_ACTION::SCROLL_BLOCK_LEFT;
	btnScrollBlockRight.Padding = 5;

	btnScrollLeft.OnInit(_rend, texScrollBlockRight, ">");
	btnScrollLeft.DisplayRect = { 245, 5, 50, 50 };
	btnScrollLeft.BorderWidth = 2;
	btnScrollLeft.ActionCode = UI_ACTION::SCROLL_BLOCK_RIGHT;
	btnScrollLeft.Padding = 5;

	btnScrollRight.OnInit(_rend, texSetBlockscrollEnd, ">|");
	btnScrollRight.DisplayRect = { 305, 5, 50, 50 };
	btnScrollRight.BorderWidth = 2;
	btnScrollRight.ActionCode = UI_ACTION::SCROLL_BLOCK_END;
	btnScrollRight.Padding = 5;

	Buttons.push_back(btnSetDrawMode);
	Buttons.push_back(btnSetBlockdrawMode);
	Buttons.push_back(btnSetBorderDrawmode);
	Buttons.push_back(btnScrollLeft);
	Buttons.push_back(btnScrollRight);
	Buttons.push_back(btnScrollBlockLeft);
	Buttons.push_back(btnScrollBlockRight);
}

void UI_Editor::LoadTextures(const SDL_Rect* destRect, const SDL_Rect* srcRect)
{
	texSetDrawMode = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Draw.png", texSetDrawMode, *srcRect, *destRect);

	texSetBlockdrawMode = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/Blockdraw.png", texSetBlockdrawMode, *srcRect, *destRect);

	texSetBlockscrollStart = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/BlockscrollLeft.png", texSetBlockscrollStart, *srcRect, *destRect);

	texScrollBlockLeft = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollLeft.png", texScrollBlockLeft, *srcRect, *destRect);

	texScrollBlockRight = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/ScrollRight.png", texScrollBlockRight, *srcRect, *destRect);

	texSetBlockscrollEnd = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_rend, "Resources/icons/BlockscrollRight.png", texSetBlockscrollEnd, *srcRect, *destRect);
}

void UI_Editor::OnLoop()
{
	list<UI_Control>::iterator iter = Buttons.begin();
	iter->IsActive = _activeTool == UI_ACTION::DRAWMODE;
	advance(iter, 1);
	iter->IsActive = _activeTool == UI_ACTION::BORDERDRAWMODE;
}

void UI_Editor::OnEvent(SDL_Event* event)
{
	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnEvent(event);
	}

	FillColorWidgets.OnEvent(event);
	BorderColorWidgets.OnEvent(event);
	_txtSaveFilename.OnEvent(event);

	if (event->type == EDITOR_EVENT_TYPE)
	{
		if (event->user.code == (Sint32)UI_ACTION::DRAWMODE || event->user.code == (Sint32)UI_ACTION::BORDERDRAWMODE)
		{
			_activeTool = (UI_ACTION)event->user.code;
		}
	}

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
		Size2D texSize;
		auto iter = _map->TextureResources.begin();
		int mx = event->button.x, my = event->button.y;
		SDL_QueryTexture(iter->Texture, nullptr, nullptr, &texSize.w, &texSize.h);
		if (mx >= _tileResourceDPoint.x && mx <= _tileResourceDPoint.x + texSize.w && my >= _tileResourceDPoint.y && my <= _tileResourceDPoint.y + texSize.h)
		{
			int col = (mx - _tileResourceDPoint.x) / iter->Tilesize.w;
			int row = (my - _tileResourceDPoint.y) / iter->Tilesize.h;
			_setTileData.ResourceIndex = 1;
			_setTileData.TileIndex = (row * iter->Cols) + col + 1;

			SDL_zero(_setTileIdEvent);
			_setTileIdEvent.type = UI_EDITOR_EVENT_TYPE;
			_setTileIdEvent.user.code = (int)UI_ACTION::SET_TILEINDEX;
			_setTileIdEvent.user.data1= this;
			_setTileIdEvent.user.data2 = &_setTileData;;			
									
			SDL_PushEvent(&_setTileIdEvent);
		}
	}

}

void UI_Editor::OnRender(Uint16 colPos, Uint16 rowPos)
{
	// Background and a border please...
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_Extras::SDL_RenderSetDrawColor(_rend, FillColor);
	SDL_RenderFillRect(_rend, &DisplayRect);
	SDL_Extras::SDL_RenderDrawBorder(_rend, DisplayRect, 2, BorderColor);

	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnRender();
	}

	FillColorWidgets.OnRender();
	BorderColorWidgets.OnRender();
	_txtSaveFilename.OnRender();

	SDL_Extras::SDL_RenderStringAt(_rend, "Column " + to_string(colPos + 1) + "-" + to_string(colPos + 1 + _map->Setup.DisplayCols), { 10, 170 }, _charmap, 22, nullptr);

	RenderTileResource(1, _tileResourceDPoint);
}

void UI_Editor::OnPostRender()
{
	SDL_DestroyTexture(tex);
}

void UI_Editor::OnCleanup()
{

	for (list<UI_Control>::iterator iter = Buttons.begin(); iter != Buttons.end(); iter++)
	{
		iter->OnCleanup();
	}

	FillColorWidgets.OnCleanUp();
	BorderColorWidgets.OnCleanUp();

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(texSetDrawMode);
	SDL_DestroyTexture(texSetBlockdrawMode);
	SDL_DestroyTexture(texScrollLeft);
	SDL_DestroyTexture(texScrollRight);
	SDL_DestroyTexture(texScrollBlockLeft);
	SDL_DestroyTexture(texScrollBlockRight);
	SDL_DestroyTexture(texSetBlockscrollStart);
	SDL_DestroyTexture(texSetBlockscrollEnd);
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

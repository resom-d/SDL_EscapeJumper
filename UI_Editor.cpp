#include "UI_Editor.h"

UI_Editor::UI_Editor()
{}

void UI_Editor::OnInit(SDL_Renderer* renderer, list<SDL_Color> colors)
{
	_renderer = renderer;
	_colorPalette = colors;

	_activeTool = EDITOR_DRAWMODE;	

	btnSetDrawMode.EventType = EDITOR_EVENT_TYPE;
	btnSetBlockdrawMode.EventType = EDITOR_EVENT_TYPE;
	btnScrollLeft.EventType = EDITOR_EVENT_TYPE;
	btnScrollRight.EventType = EDITOR_EVENT_TYPE;
	btnScrollBlockLeft.EventType = EDITOR_EVENT_TYPE;
	btnScrollBlockRight.EventType = EDITOR_EVENT_TYPE;

	btnSetDrawMode.ActionCode = EDITOR_DRAWMODE;
	btnSetBlockdrawMode.ActionCode = EDITOR_BORDERDRAWMODE;
	btnScrollLeft.ActionCode = EDITOR_SCROLL_BLOCK_LEFT;
	btnScrollRight.ActionCode = EDITOR_SCROLL_BLOCK_RIGHT;
	btnScrollBlockLeft.ActionCode = EDITOR_SCROLL_BLOCK_START;
	btnScrollBlockRight.ActionCode = EDITOR_SCROLL_BLOCK_END;

	FillColor = { 235, 210, 52, 255 };
	BorderColor = { 255, 255, 255, 255 };

	SDL_Texture* orgTex = SDL_GetRenderTarget(_renderer);

	SDL_Rect srcRect = { 0,0,48,48 };
	SDL_Rect destRect = { 0,0, 48,48 };
	ConfigureWidgets(&srcRect, &destRect);

	// Fill-ColorPallet
	FillColorWidgets.Orientation = HORIZONTAL;
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
		UI_Button btn;
		btn.ActionCode = EDITOR_SET_FILL_COLOR;
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
	FillColorWidgets.OnInit(_renderer, 24);

	// Border-ColorPallet
	BorderColorWidgets.Orientation = HORIZONTAL;
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
		UI_Button btn;
		btn.ActionCode = EDITOR_SET_BORDER_COLOR;
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
	BorderColorWidgets.OnInit(_renderer, 24);

	// Cleanup
	SDL_RenderSetScale(_renderer, 1.0, 1.0);
	SDL_SetRenderTarget(_renderer, orgTex);
	SDL_RenderSetClipRect(_renderer, nullptr);

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

	btnSetDrawMode.OnInit(_renderer, texSetDrawMode, "DRAW");
	btnSetDrawMode.DisplayRect = { 5,5, 50, 50 };
	btnSetDrawMode.BorderWidth = 2;

	btnSetBlockdrawMode.OnInit(_renderer, texSetBlockdrawMode, "ERASE");
	btnSetBlockdrawMode.DisplayRect = { 65, 5, 50, 50 };
	btnSetBlockdrawMode.BorderWidth = 2;
	
	
	btnScrollBlockLeft.OnInit(_renderer, texSetBlockscrollStart, "|<");
	btnScrollBlockLeft.DisplayRect = { 125, 5, 50, 50 };
	btnScrollBlockLeft.BorderWidth = 2;
	btnScrollBlockLeft.ActionCode = EDITOR_SCROLL_BLOCK_START;

	btnScrollBlockRight.OnInit(_renderer, texScrollBlockLeft, "<");
	btnScrollBlockRight.DisplayRect = { 185, 5, 50, 50 };
	btnScrollBlockRight.BorderWidth = 2;
	btnScrollBlockRight.ActionCode = EDITOR_SCROLL_BLOCK_LEFT;

	btnScrollLeft.OnInit(_renderer, texScrollBlockRight, ">");
	btnScrollLeft.DisplayRect = { 245, 5, 50, 50 };
	btnScrollLeft.BorderWidth = 2;
	btnScrollLeft.ActionCode = EDITOR_SCROLL_BLOCK_RIGHT;

	btnScrollRight.OnInit(_renderer, texSetBlockscrollEnd, ">|");
	btnScrollRight.DisplayRect = { 305, 5, 50, 50 };
	btnScrollRight.BorderWidth = 2;
	btnScrollRight.ActionCode = EDITOR_SCROLL_BLOCK_END;

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
	texSetDrawMode = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/Draw.png", texSetDrawMode, *srcRect, *destRect);

	texSetBlockdrawMode = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/Blockdraw.png", texSetBlockdrawMode, *srcRect, *destRect);

	texSetBlockscrollStart = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/BlockscrollLeft.png", texSetBlockscrollStart, *srcRect, *destRect);

	texScrollBlockLeft = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/ScrollLeft.png", texScrollBlockLeft, *srcRect, *destRect);

	texScrollBlockRight = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/ScrollRight.png", texScrollBlockRight, *srcRect, *destRect);

	texSetBlockscrollEnd = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect->w, destRect->h);
	CreateWidgetTexture(_renderer, "Resources/icons/BlockscrollRight.png", texSetBlockscrollEnd, *srcRect, *destRect);
}

void UI_Editor::OnLoop()
{
	list<UI_Button>::iterator iter = Buttons.begin();
	iter->IsActive = _activeTool == EDITOR_DRAWMODE;
	advance(iter, 1);
	iter->IsActive = _activeTool == EDITOR_BORDERDRAWMODE;
}

void UI_Editor::OnEvent(SDL_Event* event)
{
	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnEvent(event);
	}
	
	FillColorWidgets.OnEvent(event);
	BorderColorWidgets.OnEvent(event);
	
	if (event->type == EDITOR_EVENT_TYPE)
	{
		if (event->user.code == EDITOR_DRAWMODE || event->user.code == EDITOR_BORDERDRAWMODE)
		{
			_activeTool = (EDITOR_ACTION) event->user.code;
		}
	}

	if (event->type == UI_EDITOR_EVENT_TYPE)
	{		
		SDL_Event newEvent;
		SDL_zero(newEvent);

		Userdata u = *(Userdata*)event->user.data2;

		newEvent.type = EDITOR_EVENT_TYPE;
		newEvent.user.data1 = this;
		newEvent.user.data2 = event->user.data2;
		newEvent.user.code = event->user.code;

		SDL_PushEvent(&newEvent);
	}

}

void UI_Editor::OnRender()
{
	// Background and a border please...
	SDL_RenderSetClipRect(_renderer, &DisplayRect);
	SDL_RenderSetDrawColor(_renderer, FillColor);
	SDL_RenderFillRect(_renderer, &DisplayRect);
	SDL_RenderDrawBorder(_renderer, DisplayRect, 2, &BorderColor);

	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnRender();
	}

	FillColorWidgets.OnRender();
	BorderColorWidgets.OnRender();
}

void UI_Editor::OnPostRender()
{
	SDL_DestroyTexture(tex);
}

void UI_Editor::OnCleanup()
{

}

void UI_Editor::FireEvent(void* widget, void* data)
{

}

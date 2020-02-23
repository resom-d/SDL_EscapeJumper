#include "UI_Editor.h"

UI_Editor::UI_Editor()
{}

void UI_Editor::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;

	SDL_Texture* orgTex = SDL_GetRenderTarget(_renderer);


	SDL_Rect srcRect = { 0,0,48,48};
	SDL_Rect destRect = { 0,0, 48,48 };

	SDL_Surface* surf;
	SDL_Texture* tex;
	
	surf = IMG_Load("Resources/icons/Draw.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texSetDrawMode = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texSetDrawMode, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texSetDrawMode);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);
	

	surf = IMG_Load("Resources/icons/Erase.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texSetEraseMode = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texSetEraseMode, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texSetEraseMode);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);

	surf = IMG_Load("Resources/icons/ScrollLeft.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texScrollLeft = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texScrollLeft, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texScrollLeft);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);

	surf = IMG_Load("Resources/icons/ScrollRight.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texScrollRight = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texScrollRight, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texScrollRight);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);

	surf = IMG_Load("Resources/icons/BlockscrollLeft.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texScrollBlockLeft = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texScrollBlockLeft, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texScrollBlockLeft);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);

	surf = IMG_Load("Resources/icons/BlockscrollRight.png");
	tex = SDL_CreateTextureFromSurface(_renderer, surf);
	texScrollBlockRight = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 48, 48);
	SDL_SetTextureBlendMode(texScrollBlockRight, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(_renderer, texScrollBlockRight);
	SDL_RenderCopy(_renderer, tex, &srcRect, &destRect);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);

	SDL_RenderSetScale(_renderer, 1.0, 1.0);
	SDL_SetRenderTarget(_renderer, orgTex);

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	ConfigureWidgets();
}

void UI_Editor::ConfigureWidgets()
{
	btnSetDrawMode.OnInit(_renderer, texSetDrawMode, "DRAW");
	btnSetDrawMode.DisplayRect = { 5,5, 50, 50 };
	btnSetDrawMode.BorderWidth = 2;
	btnSetDrawMode.ActionCode = EDITOR_DRAWMODE;

	btnSetEraseMode.OnInit(_renderer, texSetEraseMode, "ERASE");
	btnSetEraseMode.DisplayRect = { 65, 5, 50, 50 };
	btnSetEraseMode.BorderWidth = 2;
	btnSetEraseMode.ActionCode = EDITOR_ERASEMODE;

	btnScrollBlockLeft.OnInit(_renderer, texScrollBlockLeft, "<<");
	btnScrollBlockLeft.DisplayRect = { 125, 5, 50, 50 };
	btnScrollBlockLeft.BorderWidth = 2;
	btnScrollBlockLeft.ActionCode = EDITOR_SCROLL_BLOCK_LEFT;

	btnScrollBlockRight.OnInit(_renderer, texScrollBlockRight, ">>");
	btnScrollBlockRight.DisplayRect = { 185, 5, 50, 50 };
	btnScrollBlockRight.BorderWidth = 2;
	btnScrollBlockRight.ActionCode = EDITOR_SCROLL_BLOCK_RIGHT;

	btnScrollLeft.OnInit(_renderer, texScrollLeft, "<");
	btnScrollLeft.DisplayRect = { 245, 5, 50, 50 };
	btnScrollLeft.BorderWidth = 2;
	btnScrollLeft.ActionCode = EDITOR_SCROLL_LEFT;

	btnScrollRight.OnInit(_renderer, texScrollRight, ">");
	btnScrollRight.DisplayRect = { 305, 5, 50, 50 };
	btnScrollRight.BorderWidth = 2;
	btnScrollRight.ActionCode = EDITOR_SCROLL_RIGHT;

	Buttons.push_back(btnSetDrawMode);
	Buttons.push_back(btnSetEraseMode);
	Buttons.push_back(btnSetBorderDrawmode);
	Buttons.push_back(btnScrollLeft);
	Buttons.push_back(btnScrollRight);
	Buttons.push_back(btnScrollBlockLeft);
	Buttons.push_back(btnScrollBlockRight);
}

void UI_Editor::OnLoop()
{}

void UI_Editor::OnEvent(SDL_Event* event)
{
	for (_widgetsIter = Buttons.begin(); _widgetsIter != Buttons.end(); _widgetsIter++)
	{
		_widgetsIter->OnEvent(event);
	}

	if (event->type == UI_EDITOR_EVENT_TYPE)
	{
		SDL_Event newEvent;
		SDL_zero(newEvent);

		newEvent.type = UI_EDITOR_EVENT_TYPE + 1;
		newEvent.user.data1 = this;

		switch (event->user.code)
		{
		case EDITOR_DRAWMODE:
			newEvent.user.code = 1;
			SDL_PushEvent(&newEvent);
			break;

		case EDITOR_ERASEMODE:
			newEvent.user.code = 2;
			SDL_PushEvent(&newEvent);
			break;

		case EDITOR_SCROLL_LEFT:
			newEvent.user.code = 3;
			SDL_PushEvent(&newEvent);
			break;

		case EDITOR_SCROLL_RIGHT:
			newEvent.user.code = 4;
			SDL_PushEvent(&newEvent);
			break;

		case EDITOR_SCROLL_BLOCK_LEFT:
			newEvent.user.code = 5;
			SDL_PushEvent(&newEvent);
			break;

		case EDITOR_SCROLL_BLOCK_RIGHT:
			newEvent.user.code = 6;
			SDL_PushEvent(&newEvent);
			break;
		}
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

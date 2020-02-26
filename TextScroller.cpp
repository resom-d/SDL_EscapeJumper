#include "TextScroller.h"


TextScroller::TextScroller()
{}


void TextScroller::OnInit(SDL_Renderer* rend, string msg, TTF_Font* font, SDL_Color col, Uint16 speed)
{
	_renderer = rend;
	_theMessage = msg;

	_font = font;
	_speed = speed;	
	_fontColor = col;
	_currentPosition = 0;

	SDL_Surface* txtSurface = TTF_RenderText_Solid(_font, _theMessage.c_str(), _fontColor);
	_texMessage = SDL_CreateTextureFromSurface(_renderer, txtSurface);
	
	_surfaceSize.x = txtSurface->w;
	_surfaceSize.y = txtSurface->h;
	DestRect.h = txtSurface->h;

	SDL_FreeSurface(txtSurface);
		
}

void TextScroller::OnLoop()
{
	_currentPosition += _speed;
	if (_currentPosition > DestRect.w + _surfaceSize.x) _currentPosition = 0;
}

void TextScroller::OnRender()
{
	SDL_Rect sRect = { 0,0,0,0 };
	SDL_Rect dRect = { 0,0,0,0 };

	sRect.y = 0;
	dRect.y = DestRect.y;
	sRect.h = _surfaceSize.y;
	dRect.h = _surfaceSize.y;
	int remDist = (DestRect.w + _surfaceSize.x) - _currentPosition;

	sRect.w = _currentPosition;
	if (_currentPosition > DestRect.w)
	{
		sRect.w = DestRect.w;
	}

	sRect.x = 0;
	if (_currentPosition > DestRect.w) sRect.x = _currentPosition - DestRect.w;
	if (remDist < DestRect.w)
	{
		sRect.w = remDist;
		sRect.x = _surfaceSize.x - remDist;		
	}
	dRect.w = sRect.w;
	dRect.x = (DestRect.x + DestRect.w) - _currentPosition;
	if (dRect.x < DestRect.x)
	{
		dRect.x = DestRect.x;
	}

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	SDL_RenderSetClipRect(_renderer, &dRect);
	SDL_RenderCopy(_renderer, _texMessage, &sRect, &dRect);

	SDL_RenderSetClipRect(_renderer, nullptr);
	
}

void TextScroller::OnCleanUp()
{
	SDL_DestroyTexture(_texMessage);
}

SDL_Point TextScroller::GetSurfaceSize()
{
	return _surfaceSize;
}

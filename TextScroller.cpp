#include "TextScroller.h"


TextScroller::TextScroller()
{}


void TextScroller::OnInit(SDL_Renderer* rend, const char* msg, TTF_Font* font, SDL_Color col, Uint16 speed, SDL_Rect* destRect)
{
	_renderer = rend;
	theMessage = new char[strlen(msg) + 1];
	strcpy_s(theMessage, strlen(theMessage), msg);
	this->font = font;
	this->speed = speed;
	this->destRect = *destRect;
	fontColor = col;
	currentPosition = 0;

	SDL_Surface* txtSurface = TTF_RenderText_Solid(this->font, theMessage, fontColor);
	txtTexture = SDL_CreateTextureFromSurface(_renderer, txtSurface);
	//txtTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, txtSurface->w + this->destRect.w, txtSurface->h);

	/*SDL_Rect sRect = { 0,0, txtSurface->w, txtSurface->h };
	SDL_Rect dRect = {this->destRect.w, 0, txtSurface->w, txtSurface->h};

	SDL_Texture* oTex = SDL_GetRenderTarget(_renderer);
	SDL_SetRenderTarget(_renderer, txtTexture);
	SDL_RenderCopy(_renderer, tex, &sRect, &dRect);
	SDL_SetRenderTarget(_renderer, oTex);*/

	surfaceSize.w = txtSurface->w;
	surfaceSize.h = txtSurface->h;
	this->destRect.h = txtSurface->h;

	SDL_FreeSurface(txtSurface);
	//SDL_DestroyTexture(tex);

	ticks = SDL_GetTicks();
	ticks_1n = SDL_GetTicks();
}

void TextScroller::OnLoop()
{
	ticks = SDL_GetTicks();
	if (ticks - ticks_1n > 3000)
	{
		isEnabled = true;
		currentPosition +=1;
		if (currentPosition > surfaceSize.w >> 1)
		{
			currentPosition = 0;
			ticks = 0;
			ticks_1n = SDL_GetTicks();
		}
		
		
	}
}

void TextScroller::OnRender()
{
	SDL_Rect srcRect =
	{
		0,
		0,
		surfaceSize.w,
		surfaceSize.h
	};
	SDL_Rect dRect = destRect;
	dRect.x = destRect.x + surfaceSize.w/2 - currentPosition;
	dRect.w = currentPosition << 1;

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_renderer, &destRect);
	SDL_RenderCopy(_renderer, txtTexture, &srcRect, &dRect);

}

void TextScroller::OnCleanUp()
{
	SDL_DestroyTexture(txtTexture);
}

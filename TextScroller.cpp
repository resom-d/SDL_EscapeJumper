#include "TextScroller.h"


TextScroller::TextScroller()
{}


void TextScroller::OnInit(SDL_Renderer* rend, const char* msg, TTF_Font* font, SDL_Color col, Uint16 speed, SDL_Rect *destRect)
{
	renderer = rend;
	theMessage = new char[strlen(msg) + 1];
	strcpy_s(theMessage, strlen(theMessage), msg);
	this->font = font;
	this->speed = speed;
	memcpy(&fontColor, &col, sizeof(col));
	memcpy(&this->destRect,  destRect, sizeof(SDL_Rect));
	currentPosition = this->destRect.w;

	txtSurface = TTF_RenderText_Solid(this->font, theMessage, fontColor);
	txtTexture = SDL_CreateTextureFromSurface(renderer, txtSurface);
	surfaceSize.h = txtSurface->h;
	surfaceSize.w = txtSurface->w;

	ticks = SDL_GetTicks();
	ticks_1n = SDL_GetTicks();
}

void TextScroller::OnLoop()
{
	ticks = SDL_GetTicks();
	if (ticks > 3000)
	{
		isEnabled = true;
		if (ticks - ticks_1n >= speed)
		{
		currentPosition -= 1;
			if (currentPosition < -surfaceSize.w) currentPosition = destRect.w;
			long diff = speed - (ticks - ticks_1n);
			if (diff < 0) diff = 0;
			ticks_1n = ticks + diff;
		}
	}
}

void TextScroller::OnRender()
{
	if (!isEnabled || true) return;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	if (currentPosition > 0)
	{
		srcRect.x = 0;
		srcRect.y = 0;

	}
	else
	{

	}


	SDL_RenderCopy(renderer, txtTexture, &srcRect, &destRect); 

}

void TextScroller::OnCleanUp()
{
	SDL_free(txtSurface);
	SDL_free(txtTexture);
}

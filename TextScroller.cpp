#include "TextScroller.h"


TextScroller::TextScroller()
{}


void TextScroller::OnInit(SDL_Renderer* rend, const char* msg, TTF_Font* font, SDL_Color col, Uint16 speed, Uint16 yPos)
{
	renderer = rend;
	theMessage = new char[strlen(msg) + 1];
	strcpy_s(theMessage, strlen(theMessage), msg);
	this->font = font;
	this->speed = speed;
	std::memcpy(&fontColor, &col, sizeof(col));

	txtSurface = TTF_RenderText_Solid(this->font, theMessage, fontColor);
	txtTexture = SDL_CreateTextureFromSurface(renderer, txtSurface);
	surfaceSize.h = txtSurface->h;
	surfaceSize.w = txtSurface->w;
	y_pos = yPos;
	ticks = SDL_GetTicks();
	ticks_1n = SDL_GetTicks();
	currentPosition = 1600;
}

void TextScroller::OnLoop()
{
	ticks = SDL_GetTicks();
	if (ticks > 3000)
	{
		isEnabled = true;
		if (ticks - ticks_1n >= speed)
		{
		currentPosition -= 4;
			if (currentPosition < -surfaceSize.w)
			{
				currentPosition = 1600;
			}
			long diff = speed - (ticks - ticks_1n);
			if (diff < 0) diff = 0;
			ticks_1n = ticks + diff;
		}
	}
}

void TextScroller::OnRender()
{
	if (!isEnabled) return;
	SDL_Rect sRect =
	{
		0,
		0,
		surfaceSize.w,
		surfaceSize.h
	};
	SDL_Rect dRect =
	{
		currentPosition,
		y_pos,
		surfaceSize.w,
		surfaceSize.h 
	};
	SDL_RenderCopy(renderer, txtTexture, &sRect, &dRect); 
}

void TextScroller::OnCleanUp()
{
	SDL_free(txtSurface);
}

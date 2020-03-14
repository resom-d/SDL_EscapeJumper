#include "MultiScroller.h"

void MultiScroller::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	list<path> files = GetFilesInDirectory("Resources/credits");
	for (auto fi = files.begin(); fi != files.end(); fi++)
	{
		SDL_Surface* surf = IMG_Load(fi->string().c_str());
		SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
		SDL_FreeSurface(surf);
		Textures.push_back(tex);
	}

	Items[0] = { 0, DisplayRect.y };
	Items[1] = { DisplayRect.w, DisplayRect.y};
	_texDisplay[0] = *Textures.begin();
	_texDisplay[1] = *next(Textures.begin(), 1);
	_texturePointer = 1;
}

void MultiScroller::OnLoop()
{
	if (SDL_GetTicks() - _repeatPauseTimeCatch > RepeatPause || !RepeatPauseMode) {
		Items[0].x--;
		if (Items[0].x <= -DisplayRect.w)
		{
			Items[0].x = DisplayRect.w;
			_repeatPauseTimeCatch = SDL_GetTicks();
			_texturePointer++;
			if (_texturePointer > Textures.size()-1) _texturePointer = 0;
			_texDisplay[0] = *next(Textures.begin(), _texturePointer);
		}

		Items[1].x--;
		if (Items[1].x <= -DisplayRect.w)
		{
			Items[1].x = DisplayRect.w;
			_repeatPauseTimeCatch = SDL_GetTicks();
			_texturePointer++;
			if (_texturePointer > Textures.size()-1) _texturePointer = 0;
			_texDisplay[1] = *next(Textures.begin(), _texturePointer);
		}
	}
}

void MultiScroller::OnRender()
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &DisplayRect);

	SDL_Rect sRect ={ 0,0, DisplayRect.w, DisplayRect.h };
	SDL_Rect dRect = { Items[0].x ,DisplayRect.y, DisplayRect.w, DisplayRect.h };
	SDL_RenderCopy(_rend, _texDisplay[0], &sRect, &dRect);
	
	dRect = { Items[1].x ,DisplayRect.y, DisplayRect.w, DisplayRect.h };
	SDL_RenderCopy(_rend, _texDisplay[1], &sRect, &dRect);
}

void MultiScroller::OnCleanup()
{
	for (auto tex = Textures.begin(); tex != Textures.end(); tex++)
	{
		SDL_DestroyTexture(*tex);
	}
}

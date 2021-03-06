#include "TextScroller.h"


TextScroller::TextScroller()
{}


void TextScroller::OnInit(SDL_Renderer* rend, string msg, TTF_Font* font, SDL_Color col, Uint16 speed)
{
	_rend = rend;
	_theMessage = msg;

	_font = font;
	_speed = speed;
	_fontColor = col;
	_currentPosition = 0;
}

void TextScroller::OnLoop()
{
	for (auto loop = 0; loop < _speed; loop++)
	{
		if (++_currentPosition > _lastItemWidth)
		{
			_currentPosition = 0;
			if (_charIndex < _theMessage.length())
			{
				CharItem item = CharItem();
				item.Content = *next(_theMessage.begin(), _charIndex);
				item.ScreenPos = { DisplayRect.x + DisplayRect.w + item.Width, DisplayRect.y };
				SDL_Surface* surf = TTF_RenderText_Solid(_font, &item.Content, _fontColor);
				SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
				SDL_FreeSurface(surf);
				item.Texture = tex;
				int w, h;
				SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
				item.Width = w;
				item.Height = h;
				_lastItemWidth = w;
				_screenItems.push_back(item);
				_charIndex++;
			}
		}
		for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
		{
			item->ScreenPos.x--;
			if (item->ScreenPos.x < DisplayRect.x - item->Width)
			{
				SDL_DestroyTexture(item->Texture);
				item = _screenItems.erase(item);
			}
			if (_screenItems.size() < 1) break;
		}
	}


	if (_screenItems.size() < 1) _charIndex = 0;

}

void TextScroller::OnRender()
{
	SDL_Rect r = DisplayRect;
	r.h = 200;
	r.y -= 100;
	SDL_RenderSetClipRect(_rend, &r);
	for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
	{
		auto a = item->ScreenPos.x / (DisplayRect.w / 900.0);
		auto y = (DisplayRect.y + DisplayRect.h - item->Height) + (sin(a * (M_PI / 180.0)) * 12.0);
		SDL_Rect sRect = { 0,0, item->Width, item->Height };
		SDL_Rect dRect = { item->ScreenPos.x, y, item->Width, item->Height };
		SDL_RenderCopy(_rend, item->Texture, &sRect, &dRect);
	}
}

void TextScroller::OnCleanUp()
{
	for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
	{
		SDL_DestroyTexture(item->Texture);
	}
}

void TextScroller::OnReset(void)
{
	_currentPosition = 0;
	_screenItems.clear();
	_lastItemWidth = 200;
	_charIndex = 0;
}


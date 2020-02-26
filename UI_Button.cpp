#include "UI_Button.h"


UI_Button::UI_Button()
{
	_text = "";
	_texture = nullptr;
	_rend = nullptr;
}

void UI_Button::OnInit(SDL_Renderer* rend, SDL_Texture* tex)
{
	_rend = rend;
	_texture = tex;
}

void UI_Button::OnInit(SDL_Renderer* rend, SDL_Texture* tex, string text)
{
	_rend = rend;
	_texture = tex;
	_text = text;
}

void UI_Button::OnInit(SDL_Renderer* rend, string text)
{
	_rend = rend;
	_text = text;
}

void UI_Button::OnRender(void)
{
	if (_rend == nullptr) return;

	UI_Widget::OnRender();

	if (_texture != nullptr)
	{
		Uint32 f;
		int  a, w, h;
		SDL_QueryTexture(_texture, &f, &a, &w, &h);
		int contentWidth = DisplayRect.w - (BorderWidth << 1);
		int contentHeight = DisplayRect.h - (BorderWidth << 1);
		
		SDL_Rect srcRect = { 0,0,w,h };
		SDL_Rect destRect =
		{
			DisplayRect.x + BorderWidth,
			DisplayRect.y + BorderWidth,
			contentWidth,
			contentHeight
		};

		SDL_RenderSetClipRect(_rend, &DisplayRect);
		SDL_RenderCopy(_rend, _texture, &srcRect, &destRect);

		SDL_RenderSetClipRect(_rend, nullptr);
		
	}

	if (_text.size() != 0)
	{
	}

}

void UI_Button::OnCleanup(void)
{
	SDL_DestroyTexture(_texture);
}

#include "UI_Button.h"


UI_Button::UI_Button()
{
	_text = "";
	_texture = nullptr;
	_renderer = nullptr;
}

void UI_Button::OnInit(SDL_Renderer* rend, SDL_Texture* tex)
{
	_renderer = rend;
	_texture = tex;
}

void UI_Button::OnInit(SDL_Renderer* rend, SDL_Texture* tex, string text)
{
	_renderer = rend;
	_texture = tex;
	_text = text;
}

void UI_Button::OnInit(SDL_Renderer* rend, string text)
{
	_renderer = rend;
	_text = text;
}

void UI_Button::OnRender(void)
{
	if (_renderer == nullptr) return;

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

		SDL_RenderSetClipRect(_renderer, &DisplayRect);
		SDL_RenderCopy(_renderer, _texture, &srcRect, &destRect);

		SDL_RenderSetClipRect(_renderer, nullptr);
		
	}

	if (_text.size() != 0)
	{
	}

}

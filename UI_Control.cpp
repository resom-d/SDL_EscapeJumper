#include "UI_Control.h"

UI_Control::UI_Control()
{
	UI_Widget::UI_Widget();
	_text = "";
	_texture = nullptr;
	_rend = nullptr;
	IsActive = false;
}


void UI_Control::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
}

void UI_Control::OnInit(SDL_Renderer* rend, SDL_Texture* tex)
{
	_rend = rend;
	_texture = tex;
}

void UI_Control::OnInit(SDL_Renderer* rend, string text)
{
	_rend = rend;
	_text = text;
}

void UI_Control::OnRender(void)
{
	if (_rend == nullptr) return;

	UI_Widget::OnRender();

	if (_texture != nullptr)
	{
		Uint32 f;
		int  a, w, h;
		SDL_QueryTexture(_texture, &f, &a, &w, &h);
		int contentWidth = DisplayRect.w - (BorderWidth << 1) - (Padding << 1);
		int contentHeight = DisplayRect.h - (BorderWidth << 1) - (Padding << 1);
		
		SDL_Rect srcRect = { 0,0,w,h };
		SDL_Rect destRect =
		{
			DisplayRect.x + BorderWidth + Padding,
			DisplayRect.y + BorderWidth + Padding,
			contentWidth,
			contentHeight
		};

		SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
		SDL_RenderSetClipRect(_rend, &DisplayRect);
		SDL_RenderCopy(_rend, _texture, &srcRect, &destRect);

		SDL_RenderSetClipRect(_rend, nullptr);
		
	}

	if (_text.size() != 0)
	{
	}

}

void UI_Control::OnCleanup(void)
{
	SDL_DestroyTexture(_texture);
}
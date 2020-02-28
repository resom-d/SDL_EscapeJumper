#include "UI_Control.h"

UI_Control::UI_Control()
{
	_text = "";
	_texture = nullptr;
	_rend = nullptr;
}

void UI_Control::OnInit(SDL_Renderer* rend, SDL_Texture* tex)
{
	_rend = rend;
	_texture = tex;
}

void UI_Control::OnInit(SDL_Renderer* rend, SDL_Texture* tex, string text)
{
	_rend = rend;
	_texture = tex;
	_text = text;
}

void UI_Control::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
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

void UI_Control::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		OnMouseButtonDown(event->button);
		break;

	}
}

void UI_Control::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;

	SDL_zero(Event);
	Event.type = EventType;
	Event.user.code = (Sint32)ActionCode;
	Event.user.data1 = this;
	Event.user.data2 = &UserData;
	SDL_PushEvent(&Event);

}


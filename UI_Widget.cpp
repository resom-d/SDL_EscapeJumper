#include "UI_Widget.h"

UI_Widget::UI_Widget()
{
}

void UI_Widget::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;
	ActionCode = 0;
}

void UI_Widget::OnInit(SDL_Renderer* renderer, Uint16 actionCode)
{
	_renderer = renderer;
	ActionCode = actionCode;

}

void UI_Widget::OnLoop()
{}

void UI_Widget::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		OnMouseButtonDown(event->button);
		break;

	}
}

void UI_Widget::OnRender()
{
	SDL_RenderSetClipRect(_renderer, &DisplayRect);
	if(!IsActive) SDL_SetRenderDrawColor(_renderer, FillColor.r, FillColor.g, FillColor.b, FillColor.a);
	else SDL_SetRenderDrawColor(_renderer, FillColorActive.r, FillColorActive.g, FillColorActive.b, FillColorActive.a);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(_renderer, &DisplayRect);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
	if (!IsActive) SDL_RenderDrawBorder(_renderer, DisplayRect, BorderWidth, &BorderColor);
	else SDL_RenderDrawBorder(_renderer, DisplayRect, BorderWidth, &BorderColorActive);
	SDL_RenderSetClipRect(_renderer, nullptr);
}

void UI_Widget::OnCleanup()
{}

void UI_Widget::SetActiveMode(bool active)
{
	IsActive = active;
}


void UI_Widget::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;
	
	SDL_zero(Event);
	Event.type = EventType;
	Event.user.code = ActionCode;
	Event.user.data1 = this;
	Event.user.data2 = &UserData;
	SDL_PushEvent(&Event);

}

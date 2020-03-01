#include "UI_Widget.h"

UI_Widget::UI_Widget()
{
	FillColor = Control_FillColor;
	FillColorActive = Control_FillColorActive;
	FillColorHover = Control_FillColorHover;
	BorderColor = Control_BorderColor;
	BorderColorActive = Control_BorderColorActive;
	BorderColorHover = Control_BorderColorHover;
}

void UI_Widget::OnInit(SDL_Renderer* renderer)
{
	_rend = renderer;
	ActionCode = UI_ACTION::DRAWMODE;
}

void UI_Widget::OnInit(SDL_Renderer* renderer, UI_ACTION actionCode)
{
	_rend = renderer;
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

	case SDL_MOUSEMOTION:
		OnMouseMove(event->button);
		break;
	}
}

void UI_Widget::OnRender()
{
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	if (!IsActive) SDL_SetRenderDrawColor(_rend, FillColor.r, FillColor.g, FillColor.b, FillColor.a);
	else SDL_SetRenderDrawColor(_rend, FillColorActive.r, FillColorActive.g, FillColorActive.b, FillColorActive.a);
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(_rend, &DisplayRect);
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_NONE);
	if (!IsActive) SDL_Extras::SDL_RenderDrawBorder(_rend, DisplayRect, BorderWidth, BorderColor);
	else SDL_Extras::SDL_RenderDrawBorder(_rend, DisplayRect, BorderWidth, BorderColorActive);
	SDL_RenderSetClipRect(_rend, nullptr);
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
	Event.user.code = (Sint32)ActionCode;
	Event.user.data1 = this;
	Event.user.data2 = &UserData;
	SDL_PushEvent(&Event);

}

void UI_Widget::OnMouseMove(SDL_MouseButtonEvent event)
{
	BorderColor = Control_BorderColor;
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;
	BorderColor = Control_BorderColorActive;
}

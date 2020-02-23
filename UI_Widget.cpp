#include "UI_Widget.h"

UI_Widget::UI_Widget()
{
}

void UI_Widget::OnInit(SDL_Renderer* renderer)
{
	ActionCode = 0;
	_renderer = renderer;
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
	SDL_SetRenderDrawColor(_renderer, FillColor.r, FillColor.g, FillColor.b, FillColor.a);
	SDL_RenderFillRect(_renderer, &DisplayRect);

	SDL_RenderDrawBorder(_renderer, DisplayRect, BorderWidth, &BorderColor);

}

void UI_Widget::OnCleanup()
{}


void UI_Widget::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;

	SDL_zero(_event);
	_event.type =UI_EDITOR_EVENT_TYPE;
	_event.user.code = ActionCode;
	_event.user.data1 = this;
	_event.user.data2 = &event;
	SDL_PushEvent(&_event);

}

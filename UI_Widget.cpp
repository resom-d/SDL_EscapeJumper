#include "UI_Widget.h"

UI_Widget::UI_Widget()
{}

void UI_Widget::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;
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

	SDL_RenderDrawBorder(_renderer, &DisplayRect, BorderWidth, &BorderColor);

}

void UI_Widget::OnCleanup()
{}

void UI_Widget::ConnectEvent(WidgetEventType type, WidgetEventCallback callback)
{
	//_onClickCallbacks.push_back(&callback);
}

void UI_Widget::DisconnectEvent(WidgetEventType type, WidgetEventCallback)
{}

void UI_Widget::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;
	
	for (_onEventIter = _onClickCallbacks.begin(); _onEventIter != _onClickCallbacks.end(); _onEventIter++)
	{
		(*_onEventIter)(this, nullptr);
	}

}

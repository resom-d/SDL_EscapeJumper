#include "UI_Container.h"

UI_Container::UI_Container()
{
	UI_Widget::UI_Widget();
}

UI_Container::~UI_Container()
{}

void UI_Container::OnInit(SDL_Renderer* rend)
{
	UI_Widget::OnInit(rend);

	for (list<UI_Control>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnInit(rend);
	}
}

void UI_Container::OnInit(SDL_Renderer* rend, UI_ACTION actionCode)
{
	UI_Widget::OnInit(rend, actionCode);

	for (list<UI_Control>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnInit(rend, "");
	}
}

void UI_Container::OnRender()
{
	UI_Widget::OnRender();

	SDL_Rect destRect =
	{
		DisplayRect.x + BorderWidth + Padding,
		DisplayRect.y + BorderWidth + Padding,
	};

	for (list<UI_Control>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		if (Orientation == WIDGET_ORIENTATION::HORIZONTAL)
		{
			//destRect.x += iter->Margin;
			iter->DisplayRect.x = destRect.x + iter->Margin;
			iter->DisplayRect.y = destRect.y + iter->Margin;
			destRect.x += iter->DisplayRect.w + iter->Margin;
		}

		if (Orientation == WIDGET_ORIENTATION::VERTICAL)
		{
			destRect.y += iter->Margin;
			iter->DisplayRect.x = destRect.x + Margin;
			iter->DisplayRect.y = destRect.y;
			destRect.y += iter->DisplayRect.w + iter->Margin;
		}
		DisplayRect.w = destRect.x - DisplayRect.x + BorderWidth + Padding + iter->Margin;
		iter->OnRender();
	}
}

void UI_Container::OnEvent(SDL_Event* event)
{
	for (list<UI_Control>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnEvent(event);
	}

	switch (event->type)
	{
	case SDL_MOUSEMOTION:
		OnMouseMove(event->button);
		break;
	}
}

void UI_Container::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.x > DisplayRect.x + DisplayRect.w || event.x < DisplayRect.x || event.y < DisplayRect.y || event.y > DisplayRect.y + DisplayRect.h) return;
}

void UI_Container::OnCleanup(void)
{
	for (list<UI_Control>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnCleanup();
	}
	_children.clear();
}

void UI_Container::AddChild(UI_Control widg)
{
	_children.push_back(widg);
}

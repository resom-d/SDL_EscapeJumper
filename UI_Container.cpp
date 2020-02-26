#include "UI_Container.h"

void UI_Container::OnInit(SDL_Renderer* rend)
{
	UI_Widget::OnInit(rend);

	for (list<UI_Button>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnInit(rend, "-Color-");
	}
}

void UI_Container::OnInit(SDL_Renderer* rend, Uint16 actionCode)
{
	UI_Widget::OnInit(rend,  actionCode);

	for (list<UI_Button>::iterator iter = _children.begin(); iter != _children.end(); iter++)
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

	int lastMargin;
	for (list<UI_Button>::iterator iter = next(_children.begin(), 1); iter != _children.end(); iter++)
	{		
		if (Orientation == HORIZONTAL)
		{
			destRect.x += iter->Margin;
			iter->DisplayRect.x = destRect.x;
			iter->DisplayRect.y = destRect.y + iter->Margin;
			destRect.x += iter->DisplayRect.w + iter->Margin;
		}

		if (Orientation == VERTICAL)
		{
			destRect.y += iter->Margin;
			iter->DisplayRect.x = destRect.x + Margin;
			iter->DisplayRect.y = destRect.y;
			destRect.y += iter->DisplayRect.w + iter->Margin;
		}
		lastMargin = iter->Margin;
		
		iter->OnRender();
	}
	DisplayRect.w= destRect.x + lastMargin + Padding;
}

void UI_Container::OnEvent(SDL_Event* event)
{
	for (list<UI_Button>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnEvent(event);
	}
}

void UI_Container::OnCleanUp(void)
{
	for (list<UI_Button>::iterator iter = _children.begin(); iter != _children.end(); iter++)
	{
		iter->OnCleanup();
	}
}

void UI_Container::AddChild(UI_Button widg)
{
	_children.push_back(widg);
}

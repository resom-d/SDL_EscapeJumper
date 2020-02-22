#include "UI_Editor.h"

UI_Editor::UI_Editor()
{}

void UI_Editor::OnInit(SDL_Renderer * renderer)
{
	for (int i = 0; i < 5; i++)
	{
		UI_Widget item;
		item.BorderColor = { 0,0,0,255 };
		item.BorderWidth = 2;
		item.FillColor = { 207, 255, 255,255 };
		item.DisplayRect.x = 10 + (i*52);
		item.DisplayRect.y = 10;
		item.DisplayRect.h = 48;
		item.DisplayRect.w = 48;
		item.State = Visible;
		item.OnInit(renderer);

		//item.ConnectEvent(MouseDown, ButtonDrawmodeClicked);

		Widgets.push_back(item);
	}
}

void UI_Editor::OnLoop()
{}

void UI_Editor::OnEvent(SDL_Event* event)
{	
	for (_widgetsIter = Widgets.begin(); _widgetsIter != Widgets.end(); _widgetsIter++)
	{
		_widgetsIter->OnEvent(event);
	}
}

void UI_Editor::OnRender()
{
	for (_widgetsIter = Widgets.begin(); _widgetsIter != Widgets.end(); _widgetsIter++)
	{
		_widgetsIter->OnRender();
	}
}

void UI_Editor::OnPostRender()
{
	SDL_DestroyTexture(tex);
}

void UI_Editor::OnCleanup()
{}

void UI_Editor::ButtonDrawmodeClicked(void* widget, void* data)
{

}

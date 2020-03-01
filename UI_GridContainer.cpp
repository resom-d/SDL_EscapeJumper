#include "UI_GridContainer.h"

UI_GridContainer::UI_GridContainer(void)
{
	UI_Container::UI_Container();
}

UI_GridContainer::~UI_GridContainer(void)
{}

void UI_GridContainer::OnInit(SDL_Renderer* rend, Uint16 cols, Uint16 rows)
{
	UI_Container::OnInit(rend);
	
	Grid.clear();
	auto rowIter = Grid.begin();
	for (auto c = 0; c < cols; c++)
	{
		GridRow row;
		for (auto r = 0; r < rows; r++)
		{
			UI_Control control;
			row.push_back(control);
		}
		rowIter->push_back(row);
	}
}

void UI_GridContainer::OnRender(void)
{
	UI_Widget::OnRender();
}

void UI_GridContainer::OnCleanUp(void)
{}

void UI_GridContainer::AddChild(UI_Control control, Uint16 col, Uint16 row)
{
	auto rowIter = Grid.begin();;
	advance(rowIter, row);

}


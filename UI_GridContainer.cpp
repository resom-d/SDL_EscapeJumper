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

	_children.clear();
		
}

void UI_GridContainer::OnRender(void)
{
	UI_Widget::OnRender();

	/*for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			if (*item != nullptr)
			{
			}
			(*item)->OnRender();
		}
	}*/
}


void UI_GridContainer::OnEvent(SDL_Event* event)
{
	/*for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			if ((*item)->IsEnabled) (*item)->OnEvent(event);
		}
	}*/
}

void UI_GridContainer::OnCleanUp(void)
{
	/*for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			(*item)->OnCleanup();
		}
	}*/
}

void UI_GridContainer::AddChild(UI_Control* child)
{
	/*auto x = _children.begin();
	auto y = x->begin();
	x->emplace(y, child);
	CalculateLayout();*/
}

void UI_GridContainer::AddChild(UI_Control* child, Uint16 col, Uint16 row)
{
	/*auto x = _children.begin();
	advance(x, col);
	auto y = x->begin();
	advance(y, row);
	x->emplace(y, child);*/
}

void UI_GridContainer::CalculateLayout()
{	
	list<Size2D> maxSizes;

	for (auto x = _children.begin(); x != _children.end(); x++)
	{
		int maxWidth = 0;
		int maxHeight = 0;
		for (auto y = x->begin(); y != x->end(); y++)
		{
			if (*y == nullptr) continue;
			if ((*y)->DisplayRect.w > maxWidth) maxWidth = (*y)->DisplayRect.w + ((*y)->Padding << 1);
			if ((*y)->DisplayRect.h > maxHeight) maxHeight = (*y)->DisplayRect.h + ((*y)->Padding << 1);
		}
		maxSizes.push_back(Size2D(maxWidth, maxHeight));
	}	

	// Set the children DispalyRects
	auto  aSize = maxSizes.begin();
	SDL_Point cPoint = { DisplayRect.x + Margin, DisplayRect.y + Margin };
	for (auto x = _children.begin(); x != _children.end(); x++)
	{
		for (auto y = x->begin(); y != x->end(); y++)
		{
			if (*y == nullptr) continue;
			// todo add coords depending on layout of controls
			(*y)->DisplayRect.x = cPoint.x;
			(*y)->DisplayRect.y = cPoint.y;
		}
		cPoint.x += aSize->w;
		cPoint.x += aSize->h;
		aSize++;
	}
}


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
	for (auto x = 0; x < cols; x++)
	{
		GridColumn gc;
		for (auto y = 0; y < rows; y++)
		{
			UI_Control uc;
			uc.DisplayRect = { 0,0,50,50 };
			uc.OnInit(_rend, nullptr);
			uc.IsVisible = false;
			gc.push_back(uc);

		}
		_children.push_back(gc);
	}
}

void UI_GridContainer::OnRender(void)
{
	UI_Widget::OnRender();

	for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			if (item->IsVisible) item->OnRender();
		}
	}
}


void UI_GridContainer::OnEvent(SDL_Event* event)
{
	for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			if (item->IsEnabled) item->OnEvent(event);
		}
	}
}

void UI_GridContainer::OnCleanup(void)
{
	for (auto col = _children.begin(); col != _children.end(); col++)
	{
		for (auto item = col->begin(); item != col->end(); item++)
		{
			item->OnCleanup();
		}
	}
}

void UI_GridContainer::AddChild(UI_Control child)
{
	auto x = _children.begin();
	auto y = x->begin();
	auto e = x->erase(y);
	x->insert(e, child);
	CalculateLayout();
}

void UI_GridContainer::AddChild(UI_Control child, Uint16 col, Uint16 row)
{
	auto x = _children.begin();
	advance(x, col);
	auto y = x->begin();
	advance(y, row);
	auto e = x->erase(y);
	x->insert(e, child);
	CalculateLayout();
}

void UI_GridContainer::CalculateLayout()
{
	list<Uint16> maxRowWidth;
	list<Uint16> maxRowHeight;
	list<list<Uint16>> maxHL;
	for (auto x = _children.begin(); x != _children.end(); x++)
	{
		int maxWidth = 0;
		int maxHeight = 0;
		list<Uint16> maxH;
		for (auto y = x->begin(); y != x->end(); y++)
		{
			if (y->DisplayRect.w > maxWidth) maxWidth = y->DisplayRect.w + (y->Margin << 1) + (Padding << 1);
			if (y->DisplayRect.h > maxHeight) maxHeight = y->DisplayRect.h + (y->Margin << 1) + (Padding << 1);
			if (y->IsVisible) maxH.push_back(maxHeight);
			else maxH.push_back(0);
		}
		maxRowWidth.push_back(maxWidth);
		maxHL.push_back(maxH);
	}

	for (auto loop = 0; loop < _children.begin()->size(); loop++)
	{
		auto max = 0;
		for (auto pool = 0; pool < _children.size(); pool++)
		{
			auto col = next(maxHL.begin(), pool);
			auto val = *next(col->begin(), loop);
			if (val > max) max = val;
		}
		maxRowHeight.push_back(max);
	}


	// Set the children DisplayRects
	auto  aWidth = maxRowWidth.begin();
	auto  aHeight = maxRowHeight.begin();
	SDL_Point cPoint = { DisplayRect.x, DisplayRect.y };
	for (auto x = _children.begin(); x != _children.end(); x++)
	{
		for (auto y = x->begin(); y != x->end(); y++)
		{
			if (!y->IsVisible) continue;
			// todo add coords depending on layout of controls
			y->DisplayRect.x = cPoint.x + y->Margin + Padding;
			y->DisplayRect.y = cPoint.y + y->Margin + Padding;
			cPoint.y += *aHeight;
		}
		cPoint.x += *aWidth;
		cPoint.y = DisplayRect.y;
		aWidth++;
		aHeight++;
	}

	// Calculate the width and height for the grid
	int width = 0, height = 0;
	for (auto maxIter = maxRowWidth.begin(); maxIter != maxRowWidth.end(); maxIter++)
	{
		width += *maxIter;
	}
	for (auto mIter = maxRowHeight.begin(); mIter != maxRowHeight.end(); mIter++)
	{
		height += *mIter;
	}

	DisplayRect.w = width;
	DisplayRect.h = height;
}


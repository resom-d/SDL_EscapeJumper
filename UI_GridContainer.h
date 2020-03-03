#pragma once
#include "UI_Types.h"
#include "UI_Container.h"
#include "Size2D.h"

typedef list<UI_Control*> GridColumn;
typedef list<GridColumn> GridControlList;

class UI_GridContainer : UI_Container
{
public:
	UI_GridContainer(void);
	~UI_GridContainer(void);

	void OnInit(SDL_Renderer* rend, Uint16 cols, Uint16 rows);
	void OnRender(void);
	void OnEvent(SDL_Event* event);
	void OnCleanUp(void);
	void AddChild(UI_Control*);
	void AddChild(UI_Control* child, Uint16 column, Uint16 row);

protected:
	GridControlList _children;
	//list<list<Uint16, Uint16>> _gridSizes;
	list<Size2D> _cellSizes;
	void CalculateLayout();

private:

};


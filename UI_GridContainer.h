#pragma once
#include "UI_Types.h"
#include "UI_Container.h"


typedef list<UI_Control> GridRow;
typedef list<list<GridRow>> GridControlList;

class UI_GridContainer : UI_Container
{
public:
	UI_GridContainer(void);
	~UI_GridContainer(void);

	virtual void OnInit(SDL_Renderer* rend, Uint16 cols, Uint16 rows);
	virtual void OnRender(void);
	virtual void OnCleanUp(void);
	virtual void AddChild(UI_Control, Uint16 column, Uint16 row);

protected:
	GridControlList Grid;
private:

};


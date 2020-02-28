#pragma once
#include "UI_Types.h"

class UI_GridContainer
{
public:
	UI_GridContainer(void);
	~UI_GridContainer(void);

	virtual void OnRender(void);
	virtual void OnCleanUp(void);
	virtual void AddChild(UI_Control);

protected:
	GridControlList Grid;
private:

};


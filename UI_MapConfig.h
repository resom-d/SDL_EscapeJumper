#pragma once
#include "GameComponentBase.h"
#include "GameTypes.h"
#include "UI_Control.h"
#include "UI_Container.h"
#include "UI_GridContainer.h"
#include "UI_TextInput.h"
#include "SDL_Extras.h"
class UI_MapConfig : public GameComponentBase
{
public: 
	string sFilename = "Level-001",
		sCols = "500", 
		sRows= "200", 
		sDCols = "30", 
		sDRows = "14",
		sBSize = "50", 
		sBSpace = "0";

	void OnInit(SDL_Renderer* rend);
	void OnLoop(void);
	void OnRender(void);
	void OnEvent(SDL_Event* event);
	void OnCleanup(void);
	
private:
	list<UI_TextInput> _txtInputs;
	UI_Container _bbox;
	CharacterTextureMap _charMap;
	SDL_Point _gridUpperLeft;

	void CreateControls();
};


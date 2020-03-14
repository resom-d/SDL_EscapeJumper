#pragma once
#include "SDL_Extras.h"
#include "UI_Container.h"
#include "UI_Control.h"
#include "UI_TextInput.h"

class UI_NewMapDialog
{
public:
	UI_NewMapDialog();
	~UI_NewMapDialog();

	SDL_Rect DisplayRect;

	void OnInit(SDL_Renderer* rend);
	void OnLoop(void);
	void OnRender(void);
	void OnEvent(SDL_Event* event);
	void OnCleanup(void);

private:
	SDL_Renderer* _rend;
	UI_Container _bbox;
	UI_TextInput _txtFilename;
	UI_TextInput _txtCols;
	CharacterTextureMap _charMap;
	string _filename="Neuer Level", _mapCols="500";

};


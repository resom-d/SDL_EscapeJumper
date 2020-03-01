#pragma once
#include "UI_Types.h"
#include "UI_Widget.h"

class UI_TextInput :	public UI_Widget
{
public:

	UI_TextInput();

	string* TextBuffer;

	virtual void OnInit(SDL_Renderer* rend, CharacterTextureMap charMap, string* text);
	virtual void OnEvent(SDL_Event* event);
	virtual void OnRender(void);
	virtual void OnCleanup(void);

private:
	SDL_Renderer* _rend;
	bool _hasFocus;
	CharacterTextureMap _charMap;

	virtual void OnLeftMouseDown(int mx, int my);

};


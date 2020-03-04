#pragma once
#include "UI_Types.h"
#include "UI_Control.h"

class UI_TextInput :	public UI_Control
{
public:
	UI_TextInput();

	string* TextBuffer;
	string Vocabular;
	Uint16 MaxSize;

	virtual void OnInit(SDL_Renderer* rend, CharacterTextureMap charMap);
	virtual void OnEvent(SDL_Event* event);
	void OnRender(void);
	virtual void OnCleanup(void);

private:
	SDL_Renderer* _rend;
	bool _hasFocus;
	CharacterTextureMap _charMap;

	virtual void OnLeftMouseDown(int mx, int my);

};


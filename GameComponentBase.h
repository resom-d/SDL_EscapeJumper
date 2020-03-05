#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include "UI_Types.h"
using namespace std;

class GameComponentBase
{
public :
	GameComponentBase(void);
	~GameComponentBase(void);

	SDL_Rect DisplayRect;

	virtual void OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap);
	virtual void OnLoop(void)=0;
	virtual void OnEvent(SDL_Event* event)=0;
	virtual void OnRender(void);
	virtual void OnCleanup(void)=0;

protected:
	SDL_Renderer* _rend;
	CharacterTextureMap* _charMap;
};


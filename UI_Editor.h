#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include "UI_Abstract.h"
using namespace std;

class UI_Editor
{
public:
	UI_Editor();

	SDL_Rect DisplayRect;
	list<UI_Widget> Widgets;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
	TTF_Font* _fontGameOversize;

	void OnInit(SDL_Renderer* renderer);
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender();
	void OnPostRender();
	void OnCleanup();

	
private:
	list<UI_Widget>::iterator _widgetsIter;
	SDL_Renderer* _renderer;
	SDL_Surface* surf;
	SDL_Texture* tex;

	void ButtonDrawmodeClicked(void*, void*);

};
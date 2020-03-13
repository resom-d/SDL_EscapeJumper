#pragma once
#include "GameComponentBase.h"

class SimpleScroller : public GameComponentBase
{
public:
	list<SDL_Point> Items;
	SDL_Texture* Tex;
	SDL_Point TexSize;
	SDL_Point Scrollposition;
	void OnInit(SDL_Renderer* rend, path image);
	void OnLoop();
	void OnRender();
	void OnCleanup();
	void OnEvent(SDL_Event* event);

private:

};


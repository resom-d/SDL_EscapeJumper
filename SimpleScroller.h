#pragma once
#include "GameComponentBase.h"

class SimpleScroller : public GameComponentBase
{
public:
	list<SDL_Point> Items;
	SDL_Texture* Tex;
	SDL_Point TexSize;
	bool RepeatPauseMode = true;
	Uint32 RepeatPause = 5000;

	void OnInit(SDL_Renderer* rend, path image);
	void OnLoop();
	void OnRender();
	void OnCleanup();
	void OnEvent(SDL_Event* event);

private:
	Uint32 _repeatPauseTimeCatch;

};


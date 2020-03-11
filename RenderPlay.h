#pragma once
#include "GameComponentBase.h"

typedef struct ScreenItem
{
	SDL_Point OPoint;
	SDL_Texture* Tex;
	Size2D TexSize;
};

class RenderPlay : public GameComponentBase
{
public:
	list<ScreenItem> Items;
	SDL_Point Scrollposition;

	void OnInit(SDL_Renderer* rend, path image);
	void OnLoop();
	void OnRender();
	void OnCleanup();
	void OnEvent(SDL_Event* event);

private:

};


#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
using namespace std;

class GameStart
{
public:
	GameStart();
	~GameStart();

	SDL_Rect DisplayRect;
	bool Done = false;

	void OnInit(SDL_Renderer* rend);
	void OnLoop(void);
	void OnRender(void);
	void OnCleanup(void);

private:
	SDL_Renderer* _rend;
	TTF_Font* _font;
	Uint32 _timeCatch;
};


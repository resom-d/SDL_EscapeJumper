#include "SDL_Extras.h"

class MultiScroller
{
public:

	SDL_Rect DisplayRect;
	list<SDL_Texture*> Textures;
	SDL_Point Items[2] = { {0,0}, {0,0} };
	bool RepeatPauseMode = true;
	Uint32 RepeatPause = 5000;

	void OnInit(SDL_Renderer* rend);
	void OnLoop();
	void OnRender();
	void OnCleanup();

private:
	SDL_Renderer* _rend;
	Uint32 _repeatPauseTimeCatch;
	Uint16 _texturePointer = 0;
	SDL_Texture* _texDisplay[2];

};


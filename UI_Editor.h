#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include "UI_Abstract.h"
#include "UI_Button.h"

using namespace std;

typedef enum EDITOR_ACTION
{
	EDITOR_DRAWMODE = 0,
	EDITOR_BORDERDRAWMODE,
	EDITOR_ERASEMODE,
	EDITOR_SCROLL_RIGHT,
	EDITOR_SCROLL_LEFT,
	EDITOR_SCROLL_BLOCK_LEFT = 5,
	EDITOR_SCROLL_BLOCK_RIGHT
};

class UI_Editor
{
public:
	UI_Editor();

	SDL_Rect DisplayRect;
	SDL_Color BorderColor = { 0,0,0, 255 };
	SDL_Color FillColor = { 255, 254, 219, 255 };
	list<UI_Button> Buttons;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
	TTF_Font* _fontGameOversize;

	void OnInit(SDL_Renderer* renderer);
	void ConfigureWidgets();
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender();
	void OnPostRender();
	void OnCleanup();

	
private:

	list<UI_Button>::iterator _widgetsIter;
	SDL_Renderer* _renderer;
	SDL_Surface* surf;
	SDL_Texture* tex;
	UI_Button btnSetDrawMode;
	UI_Button btnSetBorderDrawmode;
	UI_Button btnSetEraseMode;
	UI_Button btnScrollLeft;
	UI_Button btnScrollRight;
	UI_Button btnScrollBlockLeft;
	UI_Button btnScrollBlockRight;
	SDL_Texture* texSetDrawMode;
	SDL_Texture* texSetEraseMode;
	SDL_Texture* texSetSelectMode;
	SDL_Texture* texScrollLeft;
	SDL_Texture* texScrollRight;
	SDL_Texture* texScrollBlockLeft;
	SDL_Texture* texScrollBlockRight;

	void FireEvent(void*, void*);

};
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include "UI_Abstract.h"
#include "UI_Button.h"
#include "UI_Container.h"
#include "UI_Types.h"
using namespace std;


class UI_Editor
{
public:
	UI_Editor();

	SDL_Rect DisplayRect;
	SDL_Color BorderColor = { 0,0,0, 255 };
	SDL_Color FillColor = { 255, 254, 219, 255 };
	list<UI_Button> Buttons;
	UI_Container FillColorWidgets;
	UI_Container BorderColorWidgets;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
	TTF_Font* _fontGameOversize;

	void OnInit(SDL_Renderer* renderer, CharacterMap charMap, ColorPalette colors);
	void LoadTextures(const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void CreateWidgetTexture(SDL_Renderer* rend, string filePath, SDL_Texture* destTex, SDL_Rect srcRect, SDL_Rect destRect);
	void ConfigureWidgets(SDL_Rect* srcRect, SDL_Rect* destRect);
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender(Uint16 colPos, Uint16 rowPos);
	void OnPostRender();
	void OnCleanup();

	
private:
	SDL_Renderer* _rend;
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_Texture* texSetDrawMode;
	SDL_Texture* texSetBlockdrawMode;
	SDL_Texture* texScrollLeft;
	SDL_Texture* texScrollRight;
	SDL_Texture* texScrollBlockLeft;
	SDL_Texture* texScrollBlockRight;
	SDL_Texture* texSetBlockscrollStart;
	SDL_Texture* texSetBlockscrollEnd;

	CharacterMap _charmap;
	list<UI_Button>::iterator _widgetsIter;
	ColorPalette _colorPalette;
	ColorPalette::iterator _colorPaletteIter;
	list<UI_Widget>::iterator _colorWidgetsIter;
	UI_Button btnSetDrawMode;
	UI_Button btnSetBorderDrawmode;
	UI_Button btnSetBlockdrawMode;
	UI_Button btnScrollLeft;
	UI_Button btnScrollRight;
	UI_Button btnScrollBlockLeft;
	UI_Button btnScrollBlockRight;
	EDITOR_ACTION _activeTool;
	void FireEvent(void*, void*);

};
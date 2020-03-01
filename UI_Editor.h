#pragma once
#include "UI_Types.h"
#include "UI_Control.h"
#include "UI_Container.h"
#include "GameMap.h"
using namespace std;


class UI_Editor
{
public:
	UI_Editor();

	SDL_Rect DisplayRect;
	SDL_Color BorderColor = { 0,0,0, 255 };
	SDL_Color FillColor = { 255, 254, 219, 255 };
	list<UI_Control> Buttons;
	UI_Container FillColorWidgets;
	UI_Container BorderColorWidgets;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
	TTF_Font* _fontGameOversize;

	void OnInit(SDL_Renderer* renderer, GameMap map, CharacterTextureMap charMap, ColorPalette colors);
	void LoadTextures(const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void CreateWidgetTexture(SDL_Renderer* rend, string filePath, SDL_Texture* destTex, SDL_Rect srcRect, SDL_Rect destRect);
	void ConfigureWidgets(SDL_Rect* srcRect, SDL_Rect* destRect);
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender(Uint16 colPos, Uint16 rowPos);
	void OnPostRender();
	void OnCleanup();

protected:
	
	
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

	GameMap _map;
	CharacterTextureMap _charmap;
	list<UI_Control>::iterator _widgetsIter;
	ColorPalette _colorPalette;
	ColorPalette::iterator _colorPaletteIter;
	list<UI_Widget>::iterator _colorWidgetsIter;
	SDL_Point _tileResourceDPoint;

	SDL_Event _setTileIdEvent;
	Userdata _setTileData;

	UI_Control btnSetDrawMode;
	UI_Control btnSetBorderDrawmode;
	UI_Control btnSetBlockdrawMode;
	UI_Control btnScrollLeft;
	UI_Control btnScrollRight;
	UI_Control btnScrollBlockLeft;
	UI_Control btnScrollBlockRight;
	UI_ACTION _activeTool;
	
	void RenderTileResource(Uint16 index, SDL_Point dispPoint);
};
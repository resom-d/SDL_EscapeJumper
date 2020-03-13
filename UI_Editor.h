#pragma once
#include "UI_Types.h"
#include "UI_Control.h"
#include "UI_Container.h"
#include "UI_TextInput.h"
#include "GameMap.h"
#include "UI_TextureResourceSelector.h"
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
	UI_TextInput txtFilename;
	string FilenameSave = "";
	int ResourceIndex=0;
	int ResourceIndex_1n = 0;
	int TileIndex = 0;
	bool ConfigShown = false;
	bool ShowGrid = true;

	void OnInit(SDL_Renderer* renderer, GameMap* map, CharacterTextureMap charMap, ColorPalette colors);
	void ConfigureWidgets(SDL_Rect* srcRect, SDL_Rect* destRect);
	void OnLoop();
	void OnEvent(SDL_Event*);
	void OnRender(Uint16 colPos, Uint16 rowPos);
	void OnPostRender();
	void OnCleanup();

protected:
	
	
private:
	UI_TextureResourceSelector _confScreen;
	GameMap* _map;
	CharacterTextureMap _charmap;
	list<UI_Control>::iterator _widgetsIter;
	ColorPalette _colorPalette;
	ColorPalette::iterator _colorPaletteIter;
	list<UI_Widget>::iterator _colorWidgetsIter;
	SDL_Point _tileResourceDPoint;
	UI_ACTION _activeTool;
	SDL_Renderer* _rend;
	SDL_Surface* surf;
	SDL_Texture* tex;
	TTF_Font* _fontBig;
	TTF_Font* _fontMedium;
	TTF_Font* _fontSmall;
	TTF_Font* _fontGameOversize;

	SDL_Event _setTileIdEvent;
	Userdata _setTileData;
	
	void RenderTileResource(Uint16 index, SDL_Point dispPoint);
};
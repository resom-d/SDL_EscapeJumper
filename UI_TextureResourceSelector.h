#pragma once
#include "GameComponentBase.h"
#include "SDL_Extras.h"
#include "UI_Container.h"
#include "UI_Control.h"

class UI_TextureResourceSelector :	public GameComponentBase
{
public:
	virtual void OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap);
	virtual void OnLoop(void);
	virtual void OnEvent(SDL_Event* event);
	virtual void OnRender(void);
	virtual void OnCleanup(void);

private:
	list<path> _tilemaps;
	UI_Container _toolbar;
	TileMapTextureResource _resource;
	SDL_Texture* _texture = nullptr;
	int _tilemapIndex;
};


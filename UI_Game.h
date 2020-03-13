#pragma once
#include "GameComponentBase.h"
#include "SDL_Extras.h"
#include "UI_Types.h"
#include "UI_Container.h"
#include "JumperPlayer.h"

class UI_Game : public GameComponentBase
{
public:
	UI_Game();
	~UI_Game();

	SDL_Texture* TexBackgnd;
	
	void OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap);	
	void OnLoop(void);
	void OnEvent(SDL_Event* event);
	void OnCleanup(void);
	void OnRender(bool gameOver, JumperPlayer* player);

private:
	TTF_Font* _font;
};

